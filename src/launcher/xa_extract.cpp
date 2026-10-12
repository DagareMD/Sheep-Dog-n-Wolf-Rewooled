// Disc image installer: the PC or the PSX release, from a .bin (raw 2352-byte sectors, Mode 1
// or Mode 2) or an .iso (2048-byte sectors), read through its ISO9660 file system (with the
// Joliet names when the disc has them).
// A disc without .XA files is the PC release: the folders the game reads (PC_GAME_FOLDERS) are
// copied to <outputDir> as they are.
// A disc with .XA files is the PSX release, which needs a raw .bin (an .iso has no XA audio):
// every *.XA file is found, its XA channels de-interleaved and decoded to WAV.
// Each channel is further split into separate tracks: known sector ranges (matched
// against the PC release) get their PC names, the rest is split at silence gaps.
// Output goes to <outputDir>/Musics and <outputDir>/Voices (see XA_OUTPUT_FOLDERS).
// The level data files (LEVELS/*/*.DAV/.WAR/.SND/.MLT) are copied to <outputDir>/Levels,
// with the .SND sound banks converted from the PSX to the PC format, and the PC release's
// text bank and clip bank the game needs, which the PSX disc has none of, are written to
// <outputDir>/References/GREET_txt.BSM and MusicVoice.BSV.
//
// The job runs in two passes over the disc: a counting pass that decodes without
// writing (so the exact number of files is known up front) and the extraction pass.

#include "xa_extract.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Gives the extraction write access to the text fields of ExtractionSettings.
struct ExtractionProgress {
    static void setCurrentFile(ExtractionSettings& s, std::string file) {
        std::lock_guard<std::mutex> lock(s.textMutex);
        s.currentFile_ = std::move(file);
    }
    static void setError(ExtractionSettings& s, std::string error) {
        std::lock_guard<std::mutex> lock(s.textMutex);
        s.error_ = std::move(error);
    }
};

namespace {

const size_t RAW_SECTOR_SIZE = 2352;
const size_t SUBHEADER_OFFSET = 16;   // after 12 sync + 4 header bytes
const size_t DATA_OFFSET = 24;        // after subheader (8 bytes)

const uint8_t SM_AUDIO = 0x04, SM_FORM2 = 0x20, SM_EOF = 0x80;

const double MIN_TRACK_SECONDS = 0.25;

const int32_t K0[4] = { 0, 60, 115, 98 };
const int32_t K1[4] = { 0,  0, -52, -55 };

// ---------------------------------------------------------------- disc access

// A disc image in any of the layouts below, found by where the ISO9660 volume descriptor
// ("CD001" in sector 16) is.
class Disc {
public:
    explicit Disc(const fs::path& path) : file(path, std::ios::binary) {
        // .iso: user data only; .bin: whole sectors, the data after a Mode 2 Form 1 or a Mode 1 header
        static const struct { size_t sectorSize, dataOffset; } layouts[] = {
            { 2048, 0 }, { RAW_SECTOR_SIZE, DATA_OFFSET }, { RAW_SECTOR_SIZE, 16 },
        };
        for (const auto& l : layouts) {
            sectorSize = l.sectorSize;
            dataOffset = l.dataOffset;
            uint8_t pvd[2048];
            if (readData(16, pvd) && std::memcmp(pvd + 1, "CD001", 5) == 0) return;
        }
        sectorSize = 0; // none: not a CD image
    }
    bool ok() const { return file.good(); }
    bool isIso9660() const { return sectorSize != 0; }
    bool isRaw() const { return sectorSize == RAW_SECTOR_SIZE; } // has the XA audio sectors

    // a whole 2352-byte sector: raw images only
    bool readRaw(uint32_t lba, uint8_t* out) {
        if (!isRaw()) return false;
        file.clear();
        file.seekg(static_cast<std::streamoff>(lba) * RAW_SECTOR_SIZE);
        return static_cast<bool>(file.read(reinterpret_cast<char*>(out), RAW_SECTOR_SIZE));
    }

    // a data sector's 2048 bytes of user data
    bool readData(uint32_t lba, uint8_t* out) {
        if (!sectorSize) return false;
        file.clear();
        file.seekg(static_cast<std::streamoff>(lba) * sectorSize + static_cast<std::streamoff>(dataOffset));
        return static_cast<bool>(file.read(reinterpret_cast<char*>(out), 2048));
    }

private:
    std::ifstream file;
    size_t sectorSize = 0, dataOffset = 0;
};

struct IsoFile {
    std::string path;
    uint32_t lba;
    uint32_t size;
};

uint32_t le32(const uint8_t* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (uint32_t(p[3]) << 24); }

// a Joliet name (UCS-2, big-endian) as UTF-8
std::string jolietName(const uint8_t* p, size_t bytes) {
    std::string out;
    for (size_t i = 0; i + 1 < bytes; i += 2) {
        uint32_t c = (uint32_t(p[i]) << 8) | p[i + 1];
        if (c < 0x80) {
            out += char(c);
        } else if (c < 0x800) {
            out += char(0xC0 | (c >> 6));
            out += char(0x80 | (c & 0x3F));
        } else {
            out += char(0xE0 | (c >> 12));
            out += char(0x80 | ((c >> 6) & 0x3F));
            out += char(0x80 | (c & 0x3F));
        }
    }
    return out;
}

void walkDirectory(Disc& disc, uint32_t lba, uint32_t size, const std::string& prefix,
                          std::vector<IsoFile>& out, bool joliet, int depth = 0) {
    if (depth > 16) return;
    uint32_t sectors = (size + 2047) / 2048;
    std::vector<uint8_t> buf(sectors * 2048);
    for (uint32_t i = 0; i < sectors; ++i)
        if (!disc.readData(lba + i, &buf[i * 2048])) return;

    size_t pos = 0;
    while (pos < buf.size()) {
        uint8_t len = buf[pos];
        if (len == 0) { pos = (pos / 2048 + 1) * 2048; continue; } // records never span sectors
        const uint8_t* rec = &buf[pos];
        uint32_t extent = le32(rec + 2);
        uint32_t extSize = le32(rec + 10);
        uint8_t flags = rec[25];
        uint8_t nameLen = rec[32];
        pos += len;

        if (nameLen == 1 && (rec[33] == 0 || rec[33] == 1)) continue; // "." and ".."
        std::string name = joliet ? jolietName(rec + 33, nameLen)
                                  : std::string(reinterpret_cast<const char*>(rec + 33), nameLen);
        if (auto semi = name.find(';'); semi != std::string::npos) name.resize(semi);

        if (flags & 0x02) walkDirectory(disc, extent, extSize, prefix + name + "/", out, joliet, depth + 1);
        else out.push_back({prefix + name, extent, extSize});
    }
}

// every file on the disc, by its Joliet name when the disc has a Joliet volume (the names as
// the PC release spells them), else by its ISO9660 one
std::vector<IsoFile> listFiles(Disc& disc) {
    std::vector<IsoFile> files;
    uint8_t pvd[2048], vd[2048];
    if (!disc.readData(16, pvd) || std::memcmp(pvd + 1, "CD001", 5) != 0) return files;
    const uint8_t* root = pvd + 156;
    bool joliet = false;
    for (uint32_t lba = 17; lba < 16 + 32 && disc.readData(lba, vd) && vd[0] != 255; ++lba) {
        // a supplementary volume descriptor with a UCS-2 escape sequence: %/@, %/C or %/E
        if (vd[0] == 2 && vd[88] == '%' && vd[89] == '/' && (vd[90] == '@' || vd[90] == 'C' || vd[90] == 'E')) {
            std::memcpy(pvd, vd, sizeof(vd));
            root = pvd + 156;
            joliet = true;
            break;
        }
    }
    walkDirectory(disc, le32(root + 2), le32(root + 10), "", files, joliet);
    return files;
}

// ---------------------------------------------------------------- XA ADPCM

struct ChannelState {
    int32_t hist1 = 0;
    int32_t hist2 = 0;
};

int16_t decodeSample(int32_t raw, int shift, int filter, ChannelState& st) {
    int32_t s = (raw >> shift) + ((st.hist1 * K0[filter] + st.hist2 * K1[filter] + 32) >> 6);
    s = std::clamp(s, -32768, 32767);
    st.hist2 = st.hist1;
    st.hist1 = s;
    return static_cast<int16_t>(s);
}

// Decodes one 128-byte sound group, appending interleaved PCM (L,R,L,R... for stereo).
void decodeSoundGroup(const uint8_t* group, bool stereo, bool eightBit,
                             ChannelState& left, ChannelState& right, std::vector<int16_t>& out) {
    const int units = eightBit ? 4 : 8;
    int16_t unitPcm[8][28];

    for (int u = 0; u < units; ++u) {
        uint8_t param = group[4 + u];
        int shift = param & 0x0F;
        if (shift > 12) shift = 9;
        int filter = (param >> 4) & 0x03;
        ChannelState& st = (stereo && (u & 1)) ? right : left;

        for (int j = 0; j < 28; ++j) {
            int32_t raw;
            if (eightBit) {
                raw = static_cast<int16_t>(group[16 + j * 4 + u] << 8);
            } else {
                uint8_t b = group[16 + j * 4 + (u >> 1)];
                uint8_t nib = (u & 1) ? (b >> 4) : (b & 0x0F);
                raw = static_cast<int16_t>(nib << 12);
            }
            unitPcm[u][j] = decodeSample(raw, shift, filter, st);
        }
    }

    if (stereo) {
        for (int u = 0; u < units; u += 2)
            for (int j = 0; j < 28; ++j) {
                out.push_back(unitPcm[u][j]);
                out.push_back(unitPcm[u + 1][j]);
            }
    } else {
        for (int u = 0; u < units; ++u)
            out.insert(out.end(), unitPcm[u], unitPcm[u] + 28);
    }
}

// ---------------------------------------------------------------- WAV output

// Writes a 16-bit PCM WAV. In dry-run mode nothing touches the disk; only the data
// size is tracked, so the counting pass makes exactly the same decisions.
class WavWriter {
public:
    void open(const fs::path& path, uint32_t rate, uint16_t ch, bool dryRun) {
        sampleRate = rate;
        channels = ch;
        dataBytes = 0;
        dry = dryRun;
        active = true;
        if (dry) return;
        file.open(path, std::ios::binary);
        writeHeader();
        if (!file) throw std::runtime_error("Cannot write " + path.string());
    }
    void write(const int16_t* samples, size_t count) {
        dataBytes += static_cast<uint32_t>(count * sizeof(int16_t));
        if (!dry) file.write(reinterpret_cast<const char*>(samples), count * sizeof(int16_t));
    }
    void close() {
        if (!active) return;
        active = false;
        if (dry) return;
        file.seekp(0);
        writeHeader();
        file.close();
        if (file.fail()) throw std::runtime_error("Write error (disk full?)");
    }
    bool isOpen() const { return active; }
    uint32_t bytes() const { return dataBytes; }

private:
    void put32(uint32_t v) { file.write(reinterpret_cast<const char*>(&v), 4); }
    void put16(uint16_t v) { file.write(reinterpret_cast<const char*>(&v), 2); }
    void writeHeader() {
        file.write("RIFF", 4); put32(36 + dataBytes);
        file.write("WAVEfmt ", 8); put32(16);
        put16(1); put16(channels); put32(sampleRate);
        put32(sampleRate * channels * 2); put16(channels * 2); put16(16);
        file.write("data", 4); put32(dataBytes);
    }

    std::ofstream file;
    uint32_t sampleRate = 37800;
    uint16_t channels = 2;
    uint32_t dataBytes = 0;
    bool dry = false;
    bool active = false;
};

// ---------------------------------------------------------------- known track names

// Tracks identified by matching against the PC release. Positions are counted in
// "channel sectors": the n-th audio sector of one XA channel (sector 0 is the first
// sector of that channel), which is independent of how the channels are interleaved.
// Every sector in [startSector, endSector) is written to the named file, with leading
// and trailing silence trimmed and no further splitting. Sectors outside any known
// range fall back to silence-gap splitting. The extractor prints the range of every
// track it writes (ExtractionSettings::log), so new entries can be read off it.
const int ANY_CHANNEL = -1;   // same layout in every channel (e.g. one language per channel)
const uint32_t END_OF_CHANNEL = UINT32_MAX;

struct KnownTrack {
    const char* xaFile;
    int channel;
    uint32_t startSector;
    uint32_t endSector;
    const char* name;
};

const KnownTrack KNOWN_TRACKS[] = {
    // MUSICS.XA -> PC "Musics" folder (two songs per channel, back to back)
    { "MUSICS.XA", 0,    0,           1518, "DNBASS_v2" },
    { "MUSICS.XA", 0, 1518, END_OF_CHANNEL, "Luney_v2" },
    { "MUSICS.XA", 1,    0,           1462, "FRONZYLAST_v3" },
    { "MUSICS.XA", 1, 1462, END_OF_CHANNEL, "Mix Jami_v2" },
    { "MUSICS.XA", 2,    0,           1631, "JAZZ90_v3" },
    { "MUSICS.XA", 2, 1631, END_OF_CHANNEL, "NEWS1LAST_v2" },
    { "MUSICS.XA", 3,    0,           1256, "JAMI Full" },
    { "MUSICS.XA", 3, 1256, END_OF_CHANNEL, "ROAD MARECAGE_v2" },
    { "MUSICS.XA", 4,    0,           1443, "Jungle_v1" },
    { "MUSICS.XA", 4, 1443, END_OF_CHANNEL, "Trip_v1" },
    { "MUSICS.XA", 5,    0,           1518, "JUNGLERHODES01_v2" },
    { "MUSICS.XA", 5, 1518,           3206, "Zilla_v2" },   // last sector holds a click
    { "MUSICS.XA", 6,    0, END_OF_CHANNEL, "JUNGLETUTORIAL_v2" },

    // VOICES.XA -> PC "Voices/<language>" folder. Every channel is one language and all
    // channels share the same layout, so one range names the clip in every language.
    // Mars_2sec and intro_2sec are full cutscene soundtracks (voices mixed with music);
    // the PC cin_03_sc_rel_* / cin_int_niv_* clips are only voice stems of those.
    { "VOICES.XA", ANY_CHANNEL,     0,  1957, "Mars_2sec" },
    { "VOICES.XA", ANY_CHANNEL,  1957,  2018, "txt_lvl_004" },
    { "VOICES.XA", ANY_CHANNEL,  2018,  2094, "txt_lvl_006" },
    { "VOICES.XA", ANY_CHANNEL,  2094,  2197, "txt_lvl_007" },
    { "VOICES.XA", ANY_CHANNEL,  2197,  2271, "txt_lvl_008" },
    { "VOICES.XA", ANY_CHANNEL,  2271,  2535, "txt_lvl_0010" },
    { "VOICES.XA", ANY_CHANNEL,  2535,  2616, "txt_lvl_0012" },
    { "VOICES.XA", ANY_CHANNEL,  2616,  2956, "txt_lvl_0013" },
    { "VOICES.XA", ANY_CHANNEL,  2956,  3484, "txt_lvl_0014" },
    { "VOICES.XA", ANY_CHANNEL,  3484,  3819, "txt_lvl_0015" },
    { "VOICES.XA", ANY_CHANNEL,  3819,  3997, "txt_lvl_0016" },
    { "VOICES.XA", ANY_CHANNEL,  3997,  4067, "txt_lvl_005" },
    { "VOICES.XA", ANY_CHANNEL,  4067,  4161, "txt_lvl_003" },
    { "VOICES.XA", ANY_CHANNEL,  4161,  4212, "txt_lvl_002" },
    { "VOICES.XA", ANY_CHANNEL,  4212,  4422, "txt_lvl_009" },
    { "VOICES.XA", ANY_CHANNEL,  4422,  4476, "txt_lvl_0011" },
    { "VOICES.XA", ANY_CHANNEL,  4476,  4965, "cin_lvl0601" },
    { "VOICES.XA", ANY_CHANNEL,  4965,  5130, "txt_lvl0602" },
    { "VOICES.XA", ANY_CHANNEL,  5130,  5421, "txt_lvl0603" },
    { "VOICES.XA", ANY_CHANNEL,  5421,  5528, "txt_lvl0604" },
    { "VOICES.XA", ANY_CHANNEL,  5528,  6124, "cinfl_lvl0401" },
    { "VOICES.XA", ANY_CHANNEL,  6124,  6237, "cinfl_lvl0402" },
    { "VOICES.XA", ANY_CHANNEL,  6237,  6335, "ci_lvl_11_lap01b" },
    { "VOICES.XA", ANY_CHANNEL,  6335,  6477, "ci_lvl_11_lap02" },
    { "VOICES.XA", ANY_CHANNEL,  6477,  6604, "ci_lvl_13_cro01" },
    { "VOICES.XA", ANY_CHANNEL,  6604,  6630, "ci_lvl_13_cro02" },
    { "VOICES.XA", ANY_CHANNEL,  6630,  6842, "ci_lvl13_comb_01" },
    { "VOICES.XA", ANY_CHANNEL,  6842,  6930, "txt_rob_01" },
    { "VOICES.XA", ANY_CHANNEL,  6930,  7004, "txt_rob_02" },
    { "VOICES.XA", ANY_CHANNEL,  7004,  7095, "txt_rob_03" },
    { "VOICES.XA", ANY_CHANNEL,  7095,  7651, "cin_lvl_0101" },
    { "VOICES.XA", ANY_CHANNEL,  7651,  7951, "cin_lvl_0102" },
    { "VOICES.XA", ANY_CHANNEL,  7951,  8120, "cin_lvl_0103" },
    { "VOICES.XA", ANY_CHANNEL,  8120,  8196, "cin_lvl_0104" },
    { "VOICES.XA", ANY_CHANNEL,  8196,  8557, "cin_lvl_0105" },
    { "VOICES.XA", ANY_CHANNEL,  8557,  8906, "cin_lvl_0201" },
    { "VOICES.XA", ANY_CHANNEL,  8906,  9008, "cin_lvl_0202" },
    { "VOICES.XA", ANY_CHANNEL,  9008,  9158, "cin_lvl_0203" },
    { "VOICES.XA", ANY_CHANNEL,  9158,  9300, "cin_lvl_0204" },
    { "VOICES.XA", ANY_CHANNEL,  9300,  9433, "cin_lvl_0205" },
    { "VOICES.XA", ANY_CHANNEL,  9433,  9535, "cin_lvl_0206" },
    { "VOICES.XA", ANY_CHANNEL,  9535,  9685, "cin_lvl_0207" },
    { "VOICES.XA", ANY_CHANNEL,  9685,  9738, "cin_lvl_0208" },
    { "VOICES.XA", ANY_CHANNEL,  9738,  9767, "cin_lvl_0209" },
    { "VOICES.XA", ANY_CHANNEL,  9767,  9849, "cin_lvl_0210" },
    { "VOICES.XA", ANY_CHANNEL,  9849,  9972, "cin_lvl_0211b" },
    { "VOICES.XA", ANY_CHANNEL,  9972, 10040, "cin_lvl_0212" },
    { "VOICES.XA", ANY_CHANNEL, 10040, 10258, "cin_lvl_0301" },
    { "VOICES.XA", ANY_CHANNEL, 10258, 10425, "cin_lvl_0302" },
    { "VOICES.XA", ANY_CHANNEL, 10425, 10506, "cin_lvl_0303" },
    { "VOICES.XA", ANY_CHANNEL, 10506, 10627, "cin_lvl_0304" },
    { "VOICES.XA", ANY_CHANNEL, 10627, 10702, "cin_lvl_0307" },
    { "VOICES.XA", ANY_CHANNEL, 10702, 10753, "cin_lvl_0308" },
    { "VOICES.XA", ANY_CHANNEL, 10753, 10864, "cin_lvl_0309" },
    { "VOICES.XA", ANY_CHANNEL, 10864, 11048, "cin_lvl_0310" },
    { "VOICES.XA", ANY_CHANNEL, 11048, 11194, "cin_lvl1401_gb" },
    { "VOICES.XA", ANY_CHANNEL, 11194, 11238, "cin_lvl1402_ja" },
    { "VOICES.XA", ANY_CHANNEL, 11238, 11275, "cin_lvl1403_gbb" },
    { "VOICES.XA", ANY_CHANNEL, 11275, 11341, "cin_lvl1404_ja" },
    { "VOICES.XA", ANY_CHANNEL, 11341, 11624, "cinfe_lvl_0101" },
    { "VOICES.XA", ANY_CHANNEL, 11624, 12200, "cinfe_lvl_0102" },
    { "VOICES.XA", ANY_CHANNEL, 12200, 12313, "cintxt_lvl_0103" },
    { "VOICES.XA", ANY_CHANNEL, 12313, 12561, "cintxt_lvl_0104" },
    { "VOICES.XA", ANY_CHANNEL, 12561, 12665, "cintxt_lvl_0105" },
    { "VOICES.XA", ANY_CHANNEL, 12665, 12698, "cintxtex_lvl_0106" },
    { "VOICES.XA", ANY_CHANNEL, 12698, 12928, "cingr_lvl0801" },
    { "VOICES.XA", ANY_CHANNEL, 12928, 13015, "cingr_lvl0802" },
    { "VOICES.XA", ANY_CHANNEL, 13015, 13078, "txtel_lvl1101" },
    { "VOICES.XA", ANY_CHANNEL, 13078, 13136, "txtel_lvl1102" },
    { "VOICES.XA", ANY_CHANNEL, 13136, 13486, "cin_lvl1601" },
    { "VOICES.XA", ANY_CHANNEL, 13486, 13724, "cin_lvl1602" },
    { "VOICES.XA", ANY_CHANNEL, 13724, 13839, "cintxt_lvl1603" },
    { "VOICES.XA", ANY_CHANNEL, 13839, 13947, "cintxt_lvl1604" },
    { "VOICES.XA", ANY_CHANNEL, 13947, 14096, "cintxt_lvl1605" },
    { "VOICES.XA", ANY_CHANNEL, 14096, 14292, "cinsa_lvl1501" },
    { "VOICES.XA", ANY_CHANNEL, 14292, 14427, "cinsa_lvl1502" },
    { "VOICES.XA", ANY_CHANNEL, 14427, 14488, "cinsa_lvl1503" },
    { "VOICES.XA", ANY_CHANNEL, 14488, 14541, "cin_lvl_0311" },
    { "VOICES.XA", ANY_CHANNEL, 14541, 16341, "intro_2sec" },
    { "VOICES.XA", ANY_CHANNEL, 16341, 16384, "Cin02_sc_reL002" },
    { "VOICES.XA", ANY_CHANNEL, 16384, 16431, "Cin02_sc_reL003" },
    { "VOICES.XA", ANY_CHANNEL, 16431, 16494, "Cin02_sc_reL004" },
    { "VOICES.XA", ANY_CHANNEL, 16494, 16668, "Cin02_sc_reL005" },
    { "VOICES.XA", ANY_CHANNEL, 16668, 16766, "Cin02_sc_reL006" },
    { "VOICES.XA", ANY_CHANNEL, 16766, 16845, "ret_lvl_00" },
    { "VOICES.XA", ANY_CHANNEL, 16845, 16895, "ret_niv_00a" },
    { "VOICES.XA", ANY_CHANNEL, 16895, 16925, "ret_niv_01" },
    { "VOICES.XA", ANY_CHANNEL, 16925, 16996, "ret_niv_02ge" },
    { "VOICES.XA", ANY_CHANNEL, 16996, 17047, "ret_niv_03ge" },
    { "VOICES.XA", ANY_CHANNEL, 17047, 17072, "re_niv_04ge" },
    { "VOICES.XA", ANY_CHANNEL, 17072, 17100, "re_niv_05qu" },
    { "VOICES.XA", ANY_CHANNEL, 17100, 17140, "sc_01" },
    { "VOICES.XA", ANY_CHANNEL, 17140, 17173, "sc_02" },
    { "VOICES.XA", ANY_CHANNEL, 17173, 17221, "be_02" },
    { "VOICES.XA", ANY_CHANNEL, 17221, 17310, "sc_03" },
    { "VOICES.XA", ANY_CHANNEL, 17310, 17375, "txt_lvl_001" },
    { "VOICES.XA", ANY_CHANNEL, 17375, 17400, "cin_lvl_17_ob02a" },
    { "VOICES.XA", ANY_CHANNEL, 17400, 17436, "cin_lvl_17_ob03a" },
    { "VOICES.XA", ANY_CHANNEL, 17436, 17476, "cin_lvl_17_ob04a" },
    { "VOICES.XA", ANY_CHANNEL, 17476, 17529, "cin_04_sc_ret_01" },
    { "VOICES.XA", ANY_CHANNEL, 17529, 18014, "fin1_2sec" },
    { "VOICES.XA", ANY_CHANNEL, 18014, 18642, "fin2_2sec" },
    { "VOICES.XA", ANY_CHANNEL, 18642, 18962, "cin_goal_01" },
};

const KnownTrack* knownTrackAt(const std::string& xaFile, int channel, uint32_t channelSector) {
    for (const KnownTrack& t : KNOWN_TRACKS)
        if (xaFile == t.xaFile && (t.channel == ANY_CHANNEL || t.channel == channel) &&
            channelSector >= t.startSector && channelSector < t.endSector)
            return &t;
    return nullptr;
}

// Output sub-folder per channel, for XA files that store one language per channel.
struct ChannelFolder {
    const char* xaFile;
    int channel;
    const char* folder;
};

const ChannelFolder CHANNEL_FOLDERS[] = {
    { "VOICES.XA", 0, "french" },
    { "VOICES.XA", 1, "english" },
    { "VOICES.XA", 2, "spanish" },
    { "VOICES.XA", 3, "italian" },
    { "VOICES.XA", 4, "ch04_unknown" },
    { "VOICES.XA", 5, "ch05_unknown" },
    { "VOICES.XA", 6, "ch06_english_copy" },   // byte-identical to channel 1
};

fs::path channelFolder(const std::string& xaFile, int channel) {
    for (const ChannelFolder& f : CHANNEL_FOLDERS)
        if (xaFile == f.xaFile && channel == f.channel) return f.folder;
    return {};
}

// Output folder (inside outputDir) per XA file; other XA files use their file name stem.
struct XaOutputFolder {
    const char* xaFile;
    const char* folder;
};

const XaOutputFolder XA_OUTPUT_FOLDERS[] = {
    { "MUSICS.XA", "Musics" },
    { "VOICES.XA", "Voices" },
};

fs::path xaOutputFolder(const std::string& xaPath) {
    std::string name = fs::path(xaPath).filename().string();
    for (const XaOutputFolder& f : XA_OUTPUT_FOLDERS)
        if (name == f.xaFile) return f.folder;
    return fs::path(xaPath).stem();
}

// ---------------------------------------------------------------- track splitting

// Shared state of one pass over the disc.
struct Pass {
    ExtractionSettings& settings;
    fs::path outRoot;
    bool dryRun;
    int filesDone = 0;   // files finished in this pass
};

// Receives the decoded PCM of one XA channel, sector by sector, and cuts it into tracks:
// known ranges become named tracks, everything else is split at silence gaps.
class TrackSplitter {
public:
    TrackSplitter(Pass& pass, fs::path relDir, std::string xaFile, int xaChannel, std::string baseName,
                  uint32_t minGapMs, int silenceLevel)
        : pass(pass), relDir(std::move(relDir)), xaFile(std::move(xaFile)), xaChannel(xaChannel),
          baseName(std::move(baseName)), minGapMs(minGapMs), silenceLevel(silenceLevel) {}

    void setFormat(uint32_t rate, uint16_t ch) {
        if (rate != sampleRate || ch != channels) endTrack();
        sampleRate = rate;
        channels = ch;
    }

    // Feeds the PCM of the next sector of this channel.
    void feed(const std::vector<int16_t>& pcm) {
        const uint32_t sector = channelSector++;
        const KnownTrack* known = knownTrackAt(xaFile, xaChannel, sector);
        if (known != currentKnown) {
            endTrack();
            currentKnown = known;
        }

        const size_t minGapFrames = (minGapMs && !known) ? size_t(sampleRate) * minGapMs / 1000 : SIZE_MAX;
        for (size_t i = 0; i + channels <= pcm.size(); i += channels) {
            bool silent = true;
            for (uint16_t c = 0; c < channels; ++c)
                if (std::abs(pcm[i + c]) > silenceLevel) silent = false;

            if (silent) {
                if (!writer.isOpen()) continue;          // drop leading silence
                pendingSilence.insert(pendingSilence.end(), &pcm[i], &pcm[i] + channels);
                if (pendingSilence.size() / channels >= minGapFrames) endTrack();
                continue;
            }
            if (!writer.isOpen()) startTrack(sector);
            if (!pendingSilence.empty()) {               // pause inside a track: keep it
                writer.write(pendingSilence.data(), pendingSilence.size());
                pendingSilence.clear();
            }
            writer.write(&pcm[i], channels);
        }
        sectorsFed = sector + 1;
    }

    void endTrack() {
        pendingSilence.clear();                         // trim trailing silence
        if (!writer.isOpen()) return;
        double secs = double(writer.bytes()) / (2.0 * channels * sampleRate);
        writer.close();
        if (secs < MIN_TRACK_SECONDS) {                 // decoder blip, not a real track
            if (!pass.dryRun) fs::remove(pass.outRoot / currentRelPath);
            if (!currentKnown) --trackIndex;
            return;
        }
        ++pass.filesDone;
        if (pass.dryRun) return;

        ExtractionSettings& s = pass.settings;
        s.filesWritten = pass.filesDone;
        s.filesLeft = std::max(0, s.filesTotal - pass.filesDone);
        if (s.log) {
            char line[64];
            std::snprintf(line, sizeof(line), "ch%02d sectors %6u-%-6u %7.2fs  ", xaChannel, startSector,
                          sectorsFed, secs);
            s.log(line + currentRelPath.generic_string());
        }
    }

    // Cancellation: drop the partially written file.
    void abort() {
        pendingSilence.clear();
        if (!writer.isOpen()) return;
        writer.close();
        if (!pass.dryRun) fs::remove(pass.outRoot / currentRelPath);
    }

private:
    void startTrack(uint32_t sector) {
        startSector = sector;
        std::string name;
        if (currentKnown) {
            name = std::string(currentKnown->name) + ".wav";
        } else {
            char suffix[16];
            std::snprintf(suffix, sizeof(suffix), "_%03d.wav", trackIndex++);
            name = baseName + suffix;
        }
        currentRelPath = relDir / name;
        if (!pass.dryRun) {
            fs::create_directories(pass.outRoot / relDir);
            ExtractionProgress::setCurrentFile(pass.settings, currentRelPath.generic_string());
        }
        writer.open(pass.outRoot / currentRelPath, sampleRate, channels, pass.dryRun);
    }

    Pass& pass;
    fs::path relDir;              // relative to outRoot
    std::string xaFile;
    int xaChannel;
    std::string baseName;
    uint32_t minGapMs;
    int silenceLevel;
    uint32_t sampleRate = 37800;
    uint16_t channels = 2;
    WavWriter writer;
    std::vector<int16_t> pendingSilence;
    fs::path currentRelPath;
    const KnownTrack* currentKnown = nullptr;
    uint32_t channelSector = 0;   // index of the next sector of this channel
    uint32_t sectorsFed = 0;      // sectors fully processed (end of the current track)
    uint32_t startSector = 0;
    int trackIndex = 0;           // numbering for tracks without a known name
};

// ---------------------------------------------------------------- XA file extraction

// Progress bar split: counting pass, XA extraction, level file copy.
const float COUNTING_SHARE = 0.1f;   // ~10% of the time
const float COPY_SHARE = 0.02f;      // plain copy, very fast
const float XA_SHARE = 1.0f - COUNTING_SHARE - COPY_SHARE;

struct XaJob {
    IsoFile file;
    uint32_t gapMs;
};

// Decodes one XA file. Returns false when cancelled.
bool processXA(Disc& disc, const XaJob& job, Pass& pass, uint64_t& sectorsDone, uint64_t sectorsTotal) {
    ExtractionSettings& s = pass.settings;
    const IsoFile& xa = job.file;
    const fs::path relRoot = xaOutputFolder(xa.path);
    const std::string xaName = fs::path(xa.path).filename().string();
    const uint32_t sectorCount = (xa.size + 2047) / 2048;

    struct Channel {
        std::unique_ptr<TrackSplitter> splitter;
        ChannelState left, right;
    };
    std::map<int, Channel> channels;   // key = (file << 8) | channel
    std::vector<uint8_t> raw(RAW_SECTOR_SIZE);
    std::vector<int16_t> pcm;

    for (uint32_t i = 0; i < sectorCount; ++i) {
        if (s.cancelRequested) {
            for (auto& [key, ch] : channels) ch.splitter->abort();
            return false;
        }
        ++sectorsDone;
        if ((sectorsDone & 255) == 0) {
            float passProgress = float(double(sectorsDone) / double(sectorsTotal));
            s.progress = pass.dryRun ? passProgress * COUNTING_SHARE
                                     : COUNTING_SHARE + passProgress * XA_SHARE;
        }

        if (!disc.readRaw(xa.lba + i, raw.data())) throw std::runtime_error("Read error in " + xa.path);
        const uint8_t* sh = &raw[SUBHEADER_OFFSET];
        uint8_t fileNo = sh[0], chNo = sh[1] & 0x1F, subMode = sh[2], coding = sh[3];
        if ((subMode & (SM_AUDIO | SM_FORM2)) != (SM_AUDIO | SM_FORM2)) continue;

        bool stereo = (coding & 0x03) == 1;
        uint32_t rate = ((coding >> 2) & 0x03) == 1 ? 18900 : 37800;
        bool eightBit = ((coding >> 4) & 0x03) == 1;

        int key = (fileNo << 8) | chNo;
        Channel& ch = channels[key];
        if (!ch.splitter) {
            char base[32];
            std::snprintf(base, sizeof(base), "f%d_ch%02d", fileNo, chNo);
            ch.splitter = std::make_unique<TrackSplitter>(pass, relRoot / channelFolder(xaName, chNo), xaName,
                                                          chNo, base, job.gapMs, 0);
        }
        ch.splitter->setFormat(rate, stereo ? 2 : 1);

        pcm.clear();
        for (int g = 0; g < 18; ++g)
            decodeSoundGroup(&raw[DATA_OFFSET + g * 128], stereo, eightBit, ch.left, ch.right, pcm);
        ch.splitter->feed(pcm);

        if (subMode & SM_EOF) {
            ch.splitter->endTrack();
            ch.left = ch.right = ChannelState{};
        }
    }

    for (auto& [key, ch] : channels) ch.splitter->endTrack();
    return true;
}

// Runs one pass over all XA files. Returns false when cancelled.
bool runPass(Disc& disc, const std::vector<XaJob>& jobs, Pass& pass, uint64_t sectorsTotal) {
    uint64_t sectorsDone = 0;
    for (const XaJob& job : jobs)
        if (!processXA(disc, job, pass, sectorsDone, sectorsTotal)) return false;
    return true;
}

// ---------------------------------------------------------------- level files

// Level data files are copied from LEVELS/<DIR>/<DIR>.<EXT> on the disc to
// <outputDir>/Levels/<Folder>/<Folder>.<EXT> (sound banks converted to the PC format,
// see convertSnd), using the PC release's names:
// LEVnn -> Lvl-nn, the other folders through LEVEL_FOLDERS.
const char* const LEVEL_EXTENSIONS[] = { ".DAV", ".WAR", ".SND", ".MLT" };

struct LevelFolder {
    const char* psxDir;
    const char* folder;
    const char* fileStem;
};

const LevelFolder LEVEL_FOLDERS[] = {
    { "ENDING", "Ending", "Ending" },
    { "FEND",   "Fend",   "Fend" },
    { "INTRO",  "Intro",  "Intro" },
    { "SCENE",  "Scene",  "Scene" },
    { "WHEEL",  "Wheel",  "wheel" },
};

struct LevelJob {
    IsoFile file;
    fs::path relPath;   // relative to outputDir
};

// Output path (relative to outputDir) of a level data file, or empty if the disc file is not one.
fs::path levelOutputPath(const std::string& isoPath) {
    std::string upper = isoPath;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    const std::string prefix = "LEVELS/";
    if (upper.compare(0, prefix.size(), prefix) != 0) return {};

    size_t slash = upper.find('/', prefix.size());
    if (slash == std::string::npos || upper.find('/', slash + 1) != std::string::npos) return {};
    std::string dir = upper.substr(prefix.size(), slash - prefix.size());
    std::string ext = fs::path(upper).extension().string();
    if (std::find_if(std::begin(LEVEL_EXTENSIONS), std::end(LEVEL_EXTENSIONS),
                     [&](const char* e) { return ext == e; }) == std::end(LEVEL_EXTENSIONS))
        return {};

    std::string folder, stem;
    if (dir.size() == 5 && dir.compare(0, 3, "LEV") == 0 && std::isdigit(uint8_t(dir[3])) &&
        std::isdigit(uint8_t(dir[4]))) {
        folder = stem = "Lvl-" + dir.substr(3);
    } else {
        for (const LevelFolder& f : LEVEL_FOLDERS)
            if (dir == f.psxDir) { folder = f.folder; stem = f.fileStem; }
        if (folder.empty()) {   // unknown folder: "FOO" -> "Foo"
            folder = dir;
            std::transform(folder.begin() + 1, folder.end(), folder.begin() + 1, ::tolower);
            stem = folder;
        }
    }
    return fs::path("Levels") / folder / (stem + ext);
}

// ---------------------------------------------------------------- SND conversion

// Sound banks (.SND) are converted from the PSX layout to the PC layout:
//
//   header   PSX: [hash][ "v1.0" ][count]       PC: [hash][ "vdx7" ][count]
//   entry    PSX: [nameHash][id][size][hash2]   PC: [id][size][loop]
//            followed by a VAGp (SPU ADPCM)      followed by a RIFF WAV (mono 16-bit PCM)
//
// The header hash is a CRC-32 whose input ends with the 8 bytes "tag + count", so the
// PC hash is the PSX hash XOR the CRC (zero init, no final xor) of the bytes that differ.
// Loops always span the whole sample; a sound loops when its VAG has loop markers.

const char SND_PSX_TAG[4] = { 'v', '1', '.', '0' };
const char SND_PC_TAG[4] = { 'v', 'd', 'x', '7' };

const int32_t SPU_K0[5] = { 0, 60, 115, 98, 122 };
const int32_t SPU_K1[5] = { 0,  0, -52, -55, -60 };

uint32_t be32(const uint8_t* p) { return (uint32_t(p[0]) << 24) | (p[1] << 16) | (p[2] << 8) | p[3]; }

void putLe32(std::vector<uint8_t>& out, uint32_t v) {
    for (int i = 0; i < 4; ++i) out.push_back(uint8_t(v >> (8 * i)));
}
void putLe16(std::vector<uint8_t>& out, uint16_t v) {
    out.push_back(uint8_t(v));
    out.push_back(uint8_t(v >> 8));
}
void putTag(std::vector<uint8_t>& out, const char* tag) { out.insert(out.end(), tag, tag + 4); }

// CRC-32 (reflected 0xEDB88320) without initial value or final xor.
uint32_t rawCrc32(const uint8_t* data, size_t size) {
    uint32_t c = 0;
    for (size_t i = 0; i < size; ++i) {
        c ^= data[i];
        for (int k = 0; k < 8; ++k) c = (c >> 1) ^ ((c & 1) ? 0xEDB88320u : 0u);
    }
    return c;
}

// Decodes a VAGp file to 16-bit PCM. Drops the leading silent block and the end
// terminator block, as the PC samples do not have them.
void decodeVag(const uint8_t* vag, size_t size, std::vector<int16_t>& pcm, bool& loops) {
    pcm.clear();
    loops = false;
    int32_t hist1 = 0, hist2 = 0;
    for (size_t pos = 48; pos + 16 <= size; pos += 16) {
        const uint8_t* block = vag + pos;
        uint8_t flags = block[1];
        if (flags == 7) break;                                   // end terminator, no audio
        if ((flags & 0x03) == 0x03 || flags == 6) loops = true;  // loop start / loop end markers
        bool leadingSilence = pos == 48 && std::all_of(block, block + 16, [](uint8_t b) { return b == 0; });
        if (!leadingSilence) {
            int shift = block[0] & 0x0F;
            if (shift > 12) shift = 9;
            int filter = std::min(block[0] >> 4, 4);
            for (int j = 0; j < 28; ++j) {
                uint8_t nib = (block[2 + j / 2] >> ((j & 1) * 4)) & 0x0F;
                int32_t smp = (static_cast<int16_t>(nib << 12) >> shift) +
                              ((hist1 * SPU_K0[filter] + hist2 * SPU_K1[filter] + 32) >> 6);
                smp = std::clamp(smp, -32768, 32767);
                hist2 = hist1;
                hist1 = smp;
                pcm.push_back(static_cast<int16_t>(smp));
            }
        }
        if (flags & 0x01) break;                                 // last block (one-shot end or loop jump)
    }
}

// Converts a PSX sound bank to the PC layout. PC banks are returned unchanged.
std::vector<uint8_t> convertSnd(const std::vector<uint8_t>& in, const std::string& name) {
    auto bad = [&](const char* what) { return std::runtime_error(name + ": " + what); };
    if (in.size() < 12) throw bad("file too small for a sound bank");
    if (std::memcmp(&in[4], SND_PC_TAG, 4) == 0) return in;
    if (std::memcmp(&in[4], SND_PSX_TAG, 4) != 0) throw bad("unknown sound bank version");

    const uint32_t count = le32(&in[8]);
    uint8_t headerDiff[8] = {};
    for (int i = 0; i < 4; ++i) headerDiff[i] = uint8_t(SND_PSX_TAG[i] ^ SND_PC_TAG[i]);

    std::vector<uint8_t> out;
    out.reserve(in.size() * 4);
    putLe32(out, le32(&in[0]) ^ rawCrc32(headerDiff, sizeof(headerDiff)));
    putTag(out, SND_PC_TAG);
    putLe32(out, count);

    std::vector<int16_t> pcm;
    size_t pos = 12;
    for (uint32_t e = 0; e < count; ++e) {
        if (pos + 16 > in.size()) throw bad("truncated entry header");
        const uint32_t id = le32(&in[pos + 4]);
        const uint32_t size = le32(&in[pos + 8]);
        pos += 16;
        if (pos + size > in.size() || size < 48 || std::memcmp(&in[pos], "VAGp", 4) != 0)
            throw bad("entry is not a VAGp sample");
        const uint32_t sampleRate = be32(&in[pos + 16]);

        bool loops = false;
        decodeVag(&in[pos], size, pcm, loops);
        pos += size;

        const uint32_t dataBytes = uint32_t(pcm.size() * sizeof(int16_t));
        putLe32(out, id);
        putLe32(out, 44 + dataBytes);   // size of the RIFF that follows
        putLe32(out, loops ? 1 : 0);
        putTag(out, "RIFF"); putLe32(out, 36 + dataBytes);
        putTag(out, "WAVE");
        putTag(out, "fmt "); putLe32(out, 16);
        putLe16(out, 1); putLe16(out, 1); putLe32(out, sampleRate);
        putLe32(out, sampleRate * 2); putLe16(out, 2); putLe16(out, 16);
        putTag(out, "data"); putLe32(out, dataBytes);
        for (int16_t smp : pcm) putLe16(out, uint16_t(smp));
    }
    return out;
}

// ---------------------------------------------------------------- level file output

// Reads one disc file (2048 bytes of user data per Mode 2 Form 1 sector) and writes it
// to the output folder, converting sound banks to the PC format. Returns false when cancelled.
bool copyLevelFile(Disc& disc, const LevelJob& job, ExtractionSettings& s, uint64_t& sectorsDone,
                   uint64_t sectorsTotal) {
    const fs::path outPath = s.outputDir / job.relPath;
    ExtractionProgress::setCurrentFile(s, job.relPath.generic_string());

    std::vector<uint8_t> content(job.file.size);
    uint8_t data[2048];
    for (uint32_t i = 0, done = 0; done < job.file.size; ++i) {
        if (s.cancelRequested) return false;
        if (!disc.readData(job.file.lba + i, data)) throw std::runtime_error("Read error in " + job.file.path);
        uint32_t n = std::min<uint32_t>(job.file.size - done, 2048);
        std::memcpy(&content[done], data, n);
        done += n;
        ++sectorsDone;
    }

    const bool isSnd = job.relPath.extension() == ".SND";
    if (isSnd) content = convertSnd(content, job.file.path);

    fs::create_directories(outPath.parent_path());
    std::ofstream out(outPath, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot write " + outPath.string());
    out.write(reinterpret_cast<const char*>(content.data()), std::streamsize(content.size()));
    out.close();
    if (out.fail()) throw std::runtime_error("Write error (disk full?)");

    s.progress = COUNTING_SHARE + XA_SHARE + COPY_SHARE * float(double(sectorsDone) / double(sectorsTotal));
    s.filesWritten = s.filesWritten + 1;
    s.filesLeft = std::max(0, s.filesTotal - s.filesWritten);
    if (s.log) s.log((isSnd ? "converted " : "copied ") + job.file.path + " -> " + job.relPath.generic_string());
    return true;
}


// ---------------------------------------------------------------- References/GREET_txt.BSM

// The PC release's text bank (GREETINGV1.0): the game's pause menu labels and a few of its messages are read from it at
// run time, and the game does not start without the file. The PSX disc has no such bank, so the extraction writes one.
// Only the strings the game reads are kept (the PC release's file also holds its old Windows launcher's), word for word
// from the PC release, in its 7 languages. Text is Windows-1252. key = group << 24 | index << 8 | language bit.
struct GreetString {
    uint32_t key;
    const char* text;
};

const GreetString GREET_STRINGS[] = {
        // group 0x1, index 0x10: "Sheep, Dog'n'Wolf" WARNING
        {0x01001001, "\"Sheep, Dog'n'Wolf: Rewooled\" WARNING"}, // English
        {0x01001002, "INFO de \"Une Faim de Loup: Rewooled\""}, // French
        {0x01001004, "\"Sheep, Dog'n'Wolf: Rewooled\" WARNUNG"}, // German
        {0x01001008, "WAARSCHUWING \"Sheep, Dog'n'Wolf: Rewooled\""}, // Dutch
        {0x01001010, "ADVERTENCIA de Looney Tunes \"Perro & Lobo: Rewooled\""}, // Spanish
        {0x01001020, "NOTE su \"Ralph Il Lupo All'Attacco: Rewooled\""}, // Italian
        {0x01001040, "AVISO de \"Sheep, Dog'n'Wolf: Rewooled\""}, // Portuguese
        // group 0x1, index 0x400: Control device
        {0x01040001, "Control device"}, // English
        {0x01040002, "Contr\364leur"}, // French
        {0x01040004, "Controller"}, // German
        {0x01040008, "Controller"}, // Dutch
        {0x01040010, "Mando"}, // Spanish
        {0x01040020, "Periferica"}, // Italian
        {0x01040040, "Comando"}, // Portuguese
        // group 0x1, index 0x800: Display settings
        {0x01080001, "Display settings"}, // English
        {0x01080002, "Options d'affichage"}, // French
        {0x01080004, "Anzeigeoptionen"}, // German
        {0x01080008, "Beeldopties"}, // Dutch
        {0x01080010, "Opciones de Pantalla"}, // Spanish
        {0x01080020, "Impostazione Grafica"}, // Italian
        {0x01080040, "Op\347\365es de tela"}, // Portuguese
        // group 0x1, index 0x1000: Fog
        {0x01100001, "Fog"}, // English
        {0x01100002, "Brouillard"}, // French
        {0x01100004, "Nebel"}, // German
        {0x01100008, "Mist"}, // Dutch
        {0x01100010, "Niebla"}, // Spanish
        {0x01100020, "Nebbia"}, // Italian
        {0x01100040, "N\351voa"}, // Portuguese
        // group 0x1, index 0x2000: Sounds Enabled
        {0x01200001, "Sounds Enabled"}, // English
        {0x01200002, "Sons Activ\351s"}, // French
        {0x01200004, "Sound aktiviert"}, // German
        {0x01200008, "Geact. Geluiden"}, // Dutch
        {0x01200010, "Sonidos Activados"}, // Spanish
        {0x01200020, "Suoni Attivati"}, // Italian
        {0x01200040, "Som ativado"}, // Portuguese
        // group 0x2, index 0x10: Quit...
        {0x02001001, "Quit..."}, // English
        {0x02001002, "Quitter..."}, // French
        {0x02001004, "Beenden..."}, // German
        {0x02001008, "Stoppen..."}, // Dutch
        {0x02001010, "Salir..."}, // Spanish
        {0x02001020, "Smetti..."}, // Italian
        {0x02001040, "Abandonar..."}, // Portuguese
        // group 0x8, index 0x40: Unable to initialize Direct3D
        {0x08004001, "Unable to initialize Direct3D"}, // English
        {0x08004002, "Echec de l'initialisation de Direct3D"}, // French
        {0x08004004, "Initialisierung von Direct3D fehlgeschlagen"}, // German
        {0x08004008, "Fout bij initialisatie Direct3D"}, // Dutch
        {0x08004010, "Imposible inicializar Direct3D"}, // Spanish
        {0x08004020, "Impossibile inizializzare Direct3D"}, // Italian
        {0x08004040, "N\343o \351 poss\355vel iniciar o Direct3D"}, // Portuguese
        // group 0x8, index 0x80: Unable to initialize DirectSound
        {0x08008001, "Unable to initialize DirectSound"}, // English
        {0x08008002, "Echec de l'initialisation de DirectSound"}, // French
        {0x08008004, "Initialisierung von DirectSound fehlgeschlagen"}, // German
        {0x08008008, "Fout bij initialisatie DirectSound"}, // Dutch
        {0x08008010, "Imposible inicializar DirectSound"}, // Spanish
        {0x08008020, "Impossibile inizializzare DirectSound"}, // Italian
        {0x08008040, "N\343o \351 poss\355vel iniciar o DirectSound"}, // Portuguese
        // group 0x8, index 0x200: This button is reserved. Please choose another one.
        {0x08020001, "This button is reserved. Please choose another one."}, // English
        {0x08020002, " Ce bouton est r\351serv\351. Veuillez en choisir un autre."}, // French
        {0x08020004, "Diese Taste ist vorbelegt. Andere Taste w\344hlen."}, // German
        {0x08020008, "Deze toets is gereserveerd. Kies een andere."}, // Dutch
        {0x08020010, "Este bot\363n est\341 asignado. Elige otro."}, // Spanish
        {0x08020020, "Questo pulsante \350 riservato. Scegline un altro."}, // Italian
        {0x08020040, "Tecla exclusiva. Escolha outra."}, // Portuguese
        // group 0x8, index 0x400: Configuration saved!
        {0x08040001, "Configuration saved!"}, // English
        {0x08040002, "Configuration enregistr\351e !"}, // French
        {0x08040004, "Konfiguration gespeichert!"}, // German
        {0x08040008, "Configuratie opgeslagen!"}, // Dutch
        {0x08040010, "\241Configuraci\363n guardada!"}, // Spanish
        {0x08040020, "Configurazione salvata!"}, // Italian
        {0x08040040, "Salvou configura\347\343o!"}, // Portuguese
        // group 0x10, index 0x1: Paused
        {0x10000101, "Paused"}, // English
        {0x10000102, "En pause"}, // French
        {0x10000104, "Pause"}, // German
        {0x10000108, "Pauze"}, // Dutch
        {0x10000110, "En pausa"}, // Spanish
        {0x10000120, "Pausa"}, // Italian
        {0x10000140, "Em pausa"}, // Portuguese
        // group 0x10, index 0x2: Joy.+Keyboard
        {0x10000201, "Joy.+Keyboard"}, // English
        {0x10000202, "Joy.+Clavier"}, // French
        {0x10000204, "Joy.+Tastatur"}, // German
        {0x10000208, "Joy.+Toetsenbord"}, // Dutch
        {0x10000210, "Joy.+Teclado"}, // Spanish
        {0x10000220, "JOY.+Tastiera"}, // Italian
        {0x10000240, "Joy.+Teclado"}, // Portuguese
        // group 0x10, index 0x4: Keyboard only
        {0x10000401, "Keyboard only"}, // English
        {0x10000402, "Clavier uniquement"}, // French
        {0x10000404, "Nur Tastatur"}, // German
        {0x10000408, "Alleen Toetsenbord"}, // Dutch
        {0x10000410, "S\363lo Teclado"}, // Spanish
        {0x10000420, "Solo Tastiera"}, // Italian
        {0x10000440, "S\363 teclado"}, // Portuguese
        // group 0x10, index 0x8: Edit config.
        {0x10000801, "Edit config. "}, // English
        {0x10000802, "Voir config. "}, // French
        {0x10000804, "Konfig. \344ndern"}, // German
        {0x10000808, "Conf. Wijzigen"}, // Dutch
        {0x10000810, "Editar config."}, // Spanish
        {0x10000820, "Cambia conf."}, // Italian
        {0x10000840, "Editar config."}, // Portuguese
        // group 0x10, index 0x10: Up
        {0x10001001, "Up"}, // English
        {0x10001002, "Haut"}, // French
        {0x10001004, "Oben"}, // German
        {0x10001008, "Omhoog"}, // Dutch
        {0x10001010, "Arriba"}, // Spanish
        {0x10001020, "Su"}, // Italian
        {0x10001040, "p/Cima"}, // Portuguese
        // group 0x10, index 0x20: Right
        {0x10002001, "Right"}, // English
        {0x10002002, "Droite"}, // French
        {0x10002004, "Rechts"}, // German
        {0x10002008, "Rechts"}, // Dutch
        {0x10002010, "Dcha"}, // Spanish
        {0x10002020, "Destra"}, // Italian
        {0x10002040, "p/Direita"}, // Portuguese
        // group 0x10, index 0x40: Down
        {0x10004001, "Down"}, // English
        {0x10004002, "Bas"}, // French
        {0x10004004, "Unten"}, // German
        {0x10004008, "Omlaag"}, // Dutch
        {0x10004010, "Abajo"}, // Spanish
        {0x10004020, "Gi\371"}, // Italian
        {0x10004040, "p/Baixo"}, // Portuguese
        // group 0x10, index 0x80: Left
        {0x10008001, "Left"}, // English
        {0x10008002, "Gauche"}, // French
        {0x10008004, "Links"}, // German
        {0x10008008, "Links"}, // Dutch
        {0x10008010, "Izda"}, // Spanish
        {0x10008020, "Sinistra"}, // Italian
        {0x10008040, "p/Esquerda"}, // Portuguese
        // group 0x10, index 0x100: Rotate Cam Right
        {0x10010001, "Rotate Cam Right"}, // English
        {0x10010002, "Tourner Cam Droite"}, // French
        {0x10010004, "Kamera nach rechts"}, // German
        {0x10010008, "Camera naar rechts"}, // Dutch
        {0x10010010, "Girar c\341mara dcha"}, // Spanish
        {0x10010020, "Ruota visuale a ds."}, // Italian
        {0x10010040, "Girar c\342mera dir."}, // Portuguese
        // group 0x10, index 0x200: Rotate Cam Left
        {0x10020001, "Rotate Cam Left"}, // English
        {0x10020002, "Tourner Cam Gauche"}, // French
        {0x10020004, "Kamera nach links"}, // German
        {0x10020008, "Camera naar links"}, // Dutch
        {0x10020010, "Girar c\341mara izda"}, // Spanish
        {0x10020020, "Ruota visuale a sin."}, // Italian
        {0x10020040, "Girar c\342mera esq."}, // Portuguese
        // group 0x10, index 0x400: View map
        {0x10040001, "View map"}, // English
        {0x10040002, "Voir la carte"}, // French
        {0x10040004, "Karte anzeigen"}, // German
        {0x10040008, "Kaart bekijken"}, // Dutch
        {0x10040010, "Ver el mapa"}, // Spanish
        {0x10040020, "Osserva mappa"}, // Italian
        {0x10040040, "V\352r mapa"}, // Portuguese
        // group 0x10, index 0x800: Save config
        {0x10080001, "Save config"}, // English
        {0x10080002, "Sauvegarder config"}, // French
        {0x10080004, "Konfig. speichern"}, // German
        {0x10080008, "Conf. Opslaan"}, // Dutch
        {0x10080010, "Guardar Config."}, // Spanish
        {0x10080020, "Salva Configurazione"}, // Italian
        {0x10080040, "Salvar config."}, // Portuguese
        // group 0x10, index 0x1000: All
        {0x10100001, "All"}, // English
        {0x10100002, "Tous"}, // French
        {0x10100004, "Alle"}, // German
        {0x10100008, "Alle"}, // Dutch
        {0x10100010, "Todos"}, // Spanish
        {0x10100020, "Tutti"}, // Italian
        {0x10100040, "Todos"}, // Portuguese
        // group 0x10, index 0x2000: None
        {0x10200001, "None"}, // English
        {0x10200002, "Aucun"}, // French
        {0x10200004, "Kein"}, // German
        {0x10200008, "Geen"}, // Dutch
        {0x10200010, "Ninguno"}, // Spanish
        {0x10200020, "Nessuno"}, // Italian
        {0x10200040, "Nenhum"}, // Portuguese
        // group 0x10, index 0x4000: Sfx only
        {0x10400001, "Sfx only"}, // English
        {0x10400002, "Sfx uniquement"}, // French
        {0x10400004, "Nur Sfx"}, // German
        {0x10400008, "Alleen SFX"}, // Dutch
        {0x10400010, "S\363lo efectos sonido"}, // Spanish
        {0x10400020, "Solo effetti"}, // Italian
        {0x10400040, "S\363 efeitos"}, // Portuguese
};

const char* const GREET_PATH = "References/GREET_txt.BSM";

// Writes one of the generated files to <outputDir>/<relPath> and counts it.
void writeGeneratedFile(ExtractionSettings& s, const char* relPath, const std::vector<uint8_t>& content) {
    const fs::path outPath = s.outputDir / relPath;
    ExtractionProgress::setCurrentFile(s, relPath);
    fs::create_directories(outPath.parent_path());
    std::ofstream out(outPath, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot write " + outPath.string());
    out.write(reinterpret_cast<const char*>(content.data()), std::streamsize(content.size()));
    out.close();
    if (out.fail()) throw std::runtime_error("Write error (disk full?)");

    s.filesWritten = s.filesWritten + 1;
    s.filesLeft = std::max(0, s.filesTotal - s.filesWritten);
    if (s.log) s.log(std::string("wrote ") + relPath);
}

// "GREETINGV1.0", the entry count, then per entry its key, its length (with the terminating 0) and its text, all
// little-endian (src/objects/textresbank.cpp reads it).
void writeGreetBsm(ExtractionSettings& s) {
    std::vector<uint8_t> content;
    const char magic[] = "GREETINGV1.0";
    content.insert(content.end(), magic, magic + 12);
    putLe32(content, uint32_t(std::size(GREET_STRINGS)));
    for (const GreetString& g : GREET_STRINGS) {
        const uint32_t len = uint32_t(std::strlen(g.text)) + 1;
        putLe32(content, g.key);
        putLe32(content, len);
        content.insert(content.end(), g.text, g.text + len);
    }
    writeGeneratedFile(s, GREET_PATH, content);
}

// ---------------------------------------------------------------- References/MusicVoice.BSV

// The PC release's clip bank: the game plays music and voice lines by their 1-based position in this list (VOICE_*
// in sdw_enums.h, g_levelMusicClip), each a file name in Musics/ or Voices/<language>/, the names the extraction gives
// the PSX tracks it knows (KNOWN_TRACKS). The PSX disc has no such list. The game reads exactly these 116.
const char* const MUSIC_VOICE_CLIPS[] = {
    "be_02.wav", "ci_lvl_11_lap01b.wav", "ci_lvl_11_lap02.wav", "ci_lvl_13_cro01.wav", "ci_lvl_13_cro02.wav",
    "ci_lvl13_comb_01.wav", "cin_04_sc_ret_01.wav", "cin_goal_01.wav", "cin_lvl_0101.wav", "cin_lvl_0102.wav",
    "cin_lvl_0103.wav", "cin_lvl_0104.wav", "cin_lvl_0105.wav", "cin_lvl_0201.wav", "cin_lvl_0202.wav",
    "cin_lvl_0203.wav", "cin_lvl_0204.wav", "cin_lvl_0205.wav", "cin_lvl_0206.wav", "cin_lvl_0207.wav",
    "cin_lvl_0208.wav", "cin_lvl_0209.wav", "cin_lvl_0210.wav", "cin_lvl_0211b.wav", "cin_lvl_0212.wav",
    "cin_lvl_0301.wav", "cin_lvl_0302.wav", "cin_lvl_0303.wav", "cin_lvl_0304.wav", "cin_lvl_0307.wav",
    "cin_lvl_0308.wav", "cin_lvl_0309.wav", "cin_lvl_0310.wav", "cin_lvl_0311.wav", "cin_lvl_17_ob02a.wav",
    "cin_lvl_17_ob03a.wav", "cin_lvl_17_ob04a.wav", "cin_lvl0601.wav", "cin_lvl1401_gb.wav", "cin_lvl1402_ja.wav",
    "cin_lvl1403_gbb.wav", "cin_lvl1404_ja.wav", "cin_lvl1601.wav", "cin_lvl1602.wav", "cin_mar_01.wav",
    "Cin02_sc_reL002.wav", "Cin02_sc_reL004.wav", "Cin02_sc_reL005.wav", "Cin02_sc_reL006.wav",
    "cinfe_lvl_0101.wav", "cinfe_lvl_0102.wav", "cinfl_lvl0401.wav", "cinfl_lvl0402.wav", "cingr_lvl0801.wav",
    "cingr_lvl0802.wav", "cinsa_lvl1501.wav", "cinsa_lvl1502.wav", "cinsa_lvl1503.wav", "cintxt_lvl_0103.wav",
    "cintxt_lvl_0104.wav", "cintxt_lvl_0105.wav", "cintxt_lvl1603.wav", "cintxt_lvl1604.wav", "cintxt_lvl1605.wav",
    "cintxtex_lvl_0106.wav", "fin1_2sec.wav", "fin2_2sec.wav", "intro_2sec.wav", "Mars_2sec.wav", "re_niv_04ge.wav",
    "re_niv_05qu.wav", "ret_lvl_00.wav", "ret_niv_00a.wav", "ret_niv_01.wav", "ret_niv_02ge.wav",
    "ret_niv_03ge.wav", "sc_01.wav", "sc_02.wav", "sc_03.wav", "txt_lvl_001.wav", "txt_lvl_0010.wav",
    "txt_lvl_0011.wav", "txt_lvl_0012.wav", "txt_lvl_0013.wav", "txt_lvl_0014.wav", "txt_lvl_0015.wav",
    "txt_lvl_0016.wav", "txt_lvl_002.wav", "txt_lvl_003.wav", "txt_lvl_004.wav", "txt_lvl_005.wav",
    "txt_lvl_006.wav", "txt_lvl_007.wav", "txt_lvl_008.wav", "txt_lvl_009.wav", "txt_lvl0602.wav",
    "txt_lvl0603.wav", "txt_lvl0604.wav", "txt_rob_01.wav", "txt_rob_02.wav", "txt_rob_03.wav", "txtel_lvl1101.wav",
    "txtel_lvl1102.wav", "DNBASS_v2.wav", "FRONZYLAST_v3.wav", "JAMI Full.wav", "JAZZ90_v3.wav", "Jungle_v1.wav",
    "JUNGLERHODES01_v2.wav", "JUNGLETUTORIAL_v2.wav", "Luney_v2.wav", "Mix Jami_v2.wav", "NEWS1LAST_v2.wav",
    "ROAD MARECAGE_v2.wav", "Trip_v1.wav", "Zilla_v2.wav",
};
static_assert(std::size(MUSIC_VOICE_CLIPS) == 116, "StreamPlayer::Load_MusicVoiceBank reads 116 entries");

const char* const MUSIC_VOICE_PATH = "References/MusicVoice.BSV";

// "BSV_FILEV2.5", then per clip its name's length (with the terminating 0) and the name, little-endian
// (src/engine/stream_player.cpp reads it).
void writeMusicVoiceBsv(ExtractionSettings& s) {
    std::vector<uint8_t> content;
    const char magic[] = "BSV_FILEV2.5";
    content.insert(content.end(), magic, magic + 12);
    for (const char* name : MUSIC_VOICE_CLIPS) {
        const uint32_t len = uint32_t(std::strlen(name)) + 1;
        putLe32(content, len);
        content.insert(content.end(), name, name + len);
    }
    writeGeneratedFile(s, MUSIC_VOICE_PATH, content);
}

// ---------------------------------------------------------------- PC release

// The folders the game reads from its game folder (src/app/app_main.cpp), copied from a PC disc.
const char* const PC_GAME_FOLDERS[] = { "Levels", "Musics", "Voices", "References", "Bonus" };

// the disc file's path in the game folder, or empty when it is not in one of PC_GAME_FOLDERS
fs::path pcOutputPath(const std::string& isoPath) {
    size_t slash = isoPath.find('/');
    if (slash == std::string::npos) return {};
    auto sameLetter = [](char a, char b) { return std::toupper(uint8_t(a)) == std::toupper(uint8_t(b)); };
    for (const char* folder : PC_GAME_FOLDERS)
        if (slash == std::strlen(folder) && std::equal(isoPath.begin(), isoPath.begin() + slash, folder, sameLetter))
            return fs::path(isoPath);
    return {};
}

// Copies the PC release's game folders, as they are on the disc. Returns false when cancelled.
bool copyPcGame(Disc& disc, const std::vector<IsoFile>& files, ExtractionSettings& s) {
    std::vector<std::pair<const IsoFile*, fs::path>> jobs;
    uint64_t bytesTotal = 0, bytesDone = 0;
    for (const IsoFile& f : files) {
        fs::path rel = pcOutputPath(f.path);
        if (rel.empty()) continue;
        jobs.push_back({&f, rel});
        bytesTotal += f.size;
    }
    if (jobs.empty()) throw std::runtime_error("Neither .XA files (the PSX release) nor a Levels folder (the PC release) on the disc");

    s.filesTotal = int(jobs.size());
    s.filesLeft = s.filesTotal.load();
    s.status = ExtractionStatus::Extracting;
    uint8_t data[2048];
    for (const auto& job : jobs) {
        const IsoFile& f = *job.first;
        const fs::path outPath = s.outputDir / job.second;
        ExtractionProgress::setCurrentFile(s, job.second.generic_string());
        fs::create_directories(outPath.parent_path());
        std::ofstream out(outPath, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot write " + outPath.string());
        for (uint32_t i = 0, done = 0; done < f.size; ++i) {
            if (s.cancelRequested) return false;
            if (!disc.readData(f.lba + i, data)) throw std::runtime_error("Read error in " + f.path);
            uint32_t n = std::min<uint32_t>(f.size - done, 2048);
            out.write(reinterpret_cast<const char*>(data), n);
            done += n;
            bytesDone += n;
            s.progress = bytesTotal ? float(double(bytesDone) / double(bytesTotal)) : 1.0f;
        }
        out.close();
        if (out.fail()) throw std::runtime_error("Write error (disk full?)");
        s.filesWritten = s.filesWritten + 1;
        s.filesLeft = std::max(0, s.filesTotal - s.filesWritten);
        if (s.log) s.log("copied " + f.path);
    }
    return true;
}
} // namespace

bool extractXaAudio(ExtractionSettings* settings) {
    if (!settings) return false;
    ExtractionSettings& s = *settings;

    s.progress = 0.0f;
    s.filesTotal = 0;
    s.filesWritten = 0;
    s.filesLeft = 0;
    ExtractionProgress::setCurrentFile(s, {});
    ExtractionProgress::setError(s, {});

    auto fail = [&](const std::string& message) {
        ExtractionProgress::setCurrentFile(s, {});
        ExtractionProgress::setError(s, message);
        s.status = ExtractionStatus::Failed;
        return false;
    };
    auto cancelled = [&] {
        ExtractionProgress::setCurrentFile(s, {});
        s.status = ExtractionStatus::Cancelled;
        return false;
    };
    auto done = [&] {
        ExtractionProgress::setCurrentFile(s, {});
        s.progress = 1.0f;
        s.status = ExtractionStatus::Done;
        return true;
    };

    try {
        s.status = ExtractionStatus::Counting;
        if (s.outputDir.empty()) return fail("No output folder set");

        Disc disc(s.binPath);
        if (!disc.ok()) return fail("Cannot open " + s.binPath.string());
        if (!disc.isIso9660()) return fail("Not a CD image: no ISO9660 file system found");

        std::vector<IsoFile> files = listFiles(disc);
        if (files.empty()) return fail("No files found on the disc");

        std::vector<XaJob> jobs;
        uint64_t sectorsTotal = 0;
        for (const IsoFile& f : files) {
            std::string upper = f.path;
            std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
            if (upper.size() < 3 || upper.compare(upper.size() - 3, 3, ".XA") != 0) continue;
            bool isVoice = upper.find("VOICE") != std::string::npos;
            jobs.push_back({f, isVoice ? s.voiceGapMs : s.musicGapMs});
            sectorsTotal += (f.size + 2047) / 2048;
        }
        if (jobs.empty()) { // no XA audio: the PC release
            if (!copyPcGame(disc, files, s)) return cancelled();
            return done();
        }
        if (!disc.isRaw())
            return fail("This is the PSX release: its audio is only in a raw .bin image (with its .cue), not in an .iso");

        std::vector<LevelJob> levelJobs;
        uint64_t levelSectorsTotal = 0;
        for (const IsoFile& f : files) {
            fs::path rel = levelOutputPath(f.path);
            if (rel.empty()) continue;
            levelJobs.push_back({f, rel});
            levelSectorsTotal += (f.size + 2047) / 2048;
        }

        Pass counting{s, s.outputDir, true};
        if (!runPass(disc, jobs, counting, sectorsTotal)) return cancelled();
        s.filesTotal = counting.filesDone + int(levelJobs.size()) + 2; // + GREET_txt.BSM and MusicVoice.BSV
        s.filesLeft = s.filesTotal.load();

        s.status = ExtractionStatus::Extracting;
        fs::create_directories(s.outputDir);
        Pass extracting{s, s.outputDir, false};
        if (!runPass(disc, jobs, extracting, sectorsTotal)) return cancelled();

        uint64_t levelSectorsDone = 0;
        for (const LevelJob& job : levelJobs)
            if (!copyLevelFile(disc, job, s, levelSectorsDone, levelSectorsTotal)) return cancelled();
        writeGreetBsm(s);
        writeMusicVoiceBsv(s);
    } catch (const std::exception& e) {
        return fail(e.what());
    }

    return done();
}
