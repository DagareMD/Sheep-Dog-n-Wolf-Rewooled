/* The launcher's Language Export page's work (langexport.h): the languages of the game's files, read and written out as
 * language packs (docs/language-packs.md), from a game folder or a disc image read in place. */
#include "langexport.h"
#include "langexport_psxdav.h"
#include "langexport_tables.h"
#include "langexport_lettered.h"
#include "xa_extract.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

namespace fs = std::filesystem;

/* the game's language numbers (Progress.language), .MLT blocks and GREET_txt.BSM language bits */
enum {
    GAME_LANG_ENGLISH = 0, GAME_LANG_SPANISH = 1, GAME_LANG_ITALIAN = 2, GAME_LANG_BRAZILIAN = 3,
    MLT_BLOCK_ENGLISH = 1, MLT_BLOCK_SPANISH = 2, MLT_BLOCK_ITALIAN = 3, MLT_BLOCK_BRAZILIAN = 6,
    LANGMASK_ENGLISH = 0x01, LANGMASK_FRENCH = 0x02, LANGMASK_GERMAN = 0x04, LANGMASK_DUTCH = 0x08,
    LANGMASK_SPANISH = 0x10, LANGMASK_ITALIAN = 0x20, LANGMASK_PORTUGUESE = 0x40,
};

#define LANGPACK_SAVE_STRINGS 51

namespace {

void DefaultLog(const char *text)
{
    fprintf(stderr, "%s\n", text);
}
void (*s_log)(const char *) = DefaultLog;

void Log(const char *fmt, ...)
{
    char text[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(text, sizeof(text), fmt, ap);
    va_end(ap);
    s_log(text);
}

/* reflected CRC-32 table, polynomial 0xEDB88320 */
const u32 kCrc32Table[256] = {
    0,          0x77073096, 0xee0e612c, 0x990951ba, 0x76dc419,  0x706af48f, 0xe963a535, 0x9e6495a3, 0xedb8832,
    0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x9b64c2b,  0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a,
    0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3,
    0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab,
    0xb6662d3d, 0x76dc4190, 0x1db7106,  0x98d220bc, 0xefd5102a, 0x71b18589, 0x6b6b51f,  0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0xf00f934,  0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x86d3d2d,  0x91646c97, 0xe6635c01, 0x6b6b51f4,
    0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074,
    0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525,
    0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x3b6e20c,  0x74b1d29a, 0xead54739, 0x9dd277af, 0x4db2615,
    0x73dc1683, 0xe3630b12, 0x94643b84, 0xd6d6a3e,  0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0xa00ae27,  0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76,
    0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6,
    0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7,
    0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x26d930a,  0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x5005713,  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0xcb61b38,  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7,
    0xbdbdf21,  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278,
    0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a, 0x53b39330,
    0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};

/* the CRC-32 the game's files carry: table-driven, with no final inversion */
u32 Crc32(const u8 *data, int len)
{
    u32 crc = 0xFFFFFFFF;
    while (len--) {
        crc = kCrc32Table[(u8)(crc ^ *data)] ^ (crc >> 8);
        data++;
    }
    return crc;
}

/* ---- the game's text encoding: Windows-1252, every byte kept (the five bytes 1252 leaves out are U+0081...) ---- */

const char32_t kCp1252High[32] = {0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160,
                                  0x2039, 0x0152, 0x008D, 0x017D, 0x008F, 0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022,
                                  0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};

char32_t ByteToChar(unsigned char b)
{
    return b >= 0x80 && b < 0xA0 ? kCp1252High[b - 0x80] : (char32_t)b;
}

std::string EncodeUtf8(const std::u32string &s)
{
    std::string out;
    for (char32_t c : s) {
        if (c < 0x80)
            out += (char)c;
        else if (c < 0x800) {
            out += (char)(0xC0 | (c >> 6));
            out += (char)(0x80 | (c & 0x3F));
        } else if (c < 0x10000) {
            out += (char)(0xE0 | (c >> 12));
            out += (char)(0x80 | ((c >> 6) & 0x3F));
            out += (char)(0x80 | (c & 0x3F));
        } else {
            out += (char)(0xF0 | (c >> 18));
            out += (char)(0x80 | ((c >> 12) & 0x3F));
            out += (char)(0x80 | ((c >> 6) & 0x3F));
            out += (char)(0x80 | (c & 0x3F));
        }
    }
    return out;
}

/* ---- the languages: the .MLT blocks are the same languages in the same order in every release, PlayStation PAL's
 * having no block 6 (Brazilian Portuguese); the Portuguese fan translation wrote its European Portuguese over blocks 1,
 * 2, 3 and 6 (English, Spanish, Italian, Brazilian Portuguese), and is known by that (TranslationOverBlock1). Each
 * block's language: its name (a game's own language's the same as the game's, case aside), its menus' GREET_txt.BSM bit, its
 * folder under Voices/, and its save messages: Rewooled's own (kExportSaveMessages, by its language number), for its
 * own languages (-1: none of Rewooled's own: English's are written, for the pack's maker to rewrite). ---- */

struct Block {
    const char *name; /* UTF-8 */
    u16 menuMask;
    const char *voiceFolder;
    s32 saves;
};

const Block kBlocks[] = {
    {"Fran\xC3\xA7" "ais", LANGMASK_FRENCH, "French", -1},
    {"English", LANGMASK_ENGLISH, "English", GAME_LANG_ENGLISH},
    {"Espa\xC3\xB1ol", LANGMASK_SPANISH, "Spanish", GAME_LANG_SPANISH},
    {"Italiano", LANGMASK_ITALIAN, "Italian", GAME_LANG_ITALIAN},
    {"Deutsch", LANGMASK_GERMAN, "German", -1},
    {"Nederlands", LANGMASK_DUTCH, "Dutch", -1},
    {"Portugu\xC3\xAAs Brasileiro", LANGMASK_PORTUGUESE, "Brazilian", GAME_LANG_BRAZILIAN},
};
const u16 kBlockCount = sizeof(kBlocks) / sizeof(kBlocks[0]);
/* the fan translation's language, in block 6 (its Brazilian Portuguese's place); its voices are Voices/port */
const Block kTranslation = {"Portugu\xC3\xAAs", LANGMASK_PORTUGUESE, "port", -1};

/* a language of the game's files: its .MLT block, and what it is */
struct Language {
    std::string name;
    u16 mltBlock = 0;
    u16 menuMask = 0;
    std::string voiceFolder;
    s32 saves = -1;
};

std::vector<Language> s_langs;
std::string s_gameDir;

fs::path U8Path(const std::string &s)
{
#if defined(__cpp_char8_t)
    return fs::path(std::u8string(s.begin(), s.end()));
#else
    return fs::u8path(s);
#endif
}

std::string PathU8(const fs::path &p)
{
#if defined(__cpp_char8_t)
    std::u8string u = p.u8string();
    return std::string(u.begin(), u.end());
#else
    return p.u8string();
#endif
}

/* ---- the game's files: a folder on disk, or a disc image read in place (s_source). Every read of them is made here:
 * a path under s_gameDir is the disc's when s_source is set, else the folder's. Packs and what is written are always
 * on disk. ---- */

std::unique_ptr<DiscFiles> s_source;

/* path's place in the disc image (rel: "" its top), when the game's files are a disc image's and path is one of them */
bool InDisc(const std::string &path, std::string &rel)
{
    if (!s_source)
        return false;
    if (path == s_gameDir) {
        rel.clear();
        return true;
    }
    if (path.size() > s_gameDir.size() && path.compare(0, s_gameDir.size(), s_gameDir) == 0 &&
        path[s_gameDir.size()] == '/') {
        rel = path.substr(s_gameDir.size() + 1);
        return true;
    }
    return false;
}

struct Entry {
    std::string name, path;
    bool folder;
};

/* the entries of a folder; false when there is no such folder */
bool ListFolder(const std::string &folder, std::vector<Entry> &out)
{
    out.clear();
    std::string rel;
    if (InDisc(folder, rel)) {
        std::vector<std::pair<std::string, bool>> entries;
        if (!s_source->list(rel, entries))
            return false;
        for (const auto &e : entries)
            out.push_back({e.first, folder + "/" + e.first, e.second});
        return true;
    }
    std::error_code ec;
    fs::directory_iterator it(U8Path(folder), ec);
    if (ec)
        return false;
    for (fs::directory_iterator end; it != end; it.increment(ec)) {
        if (ec)
            break;
        std::error_code ec2;
        out.push_back({PathU8(it->path().filename()), PathU8(it->path()), it->is_directory(ec2)});
    }
    return true;
}

/* the entry of `parent` named `name` in any case ("" when there is none) */
std::string FindEntry(const std::string &parent, const std::string &name)
{
    std::vector<Entry> entries;
    ListFolder(parent, entries);
    for (const Entry &e : entries)
        if (e.name.size() == name.size() &&
            std::equal(e.name.begin(), e.name.end(), name.begin(),
                       [](char a, char b) { return tolower((unsigned char)a) == tolower((unsigned char)b); }))
            return e.path;
    return "";
}

/* whether the file or folder at path is there, spelled as it is */
bool Exists(const std::string &path)
{
    std::string rel;
    if (InDisc(path, rel)) {
        if (rel.empty())
            return true;
        size_t slash = path.rfind('/');
        std::vector<Entry> entries;
        ListFolder(path.substr(0, slash), entries);
        for (const Entry &e : entries)
            if (e.name == path.substr(slash + 1))
                return true;
        return false;
    }
    std::error_code ec;
    return fs::exists(U8Path(path), ec);
}

std::string Lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
    return s;
}

/* path with each part spelled as on disk, where it is spelled in another case ("Voices/english" for Voices/English);
 * as given when it is there, or cannot be found */
std::string ResolveCase(const std::string &path)
{
    if (Exists(path))
        return path;
    std::string norm = path;
    std::replace(norm.begin(), norm.end(), '\\', '/');
    size_t start = 0;
    std::string at;
    if (!norm.empty() && norm[0] == '/') {
        at = "/";
        start = 1;
    } else if (norm.size() > 2 && norm[1] == ':') {
        at = norm.substr(0, 3);
        start = 3;
    } else
        at = ".";
    while (start <= norm.size()) {
        size_t slash = norm.find('/', start);
        std::string part = norm.substr(start, slash == std::string::npos ? std::string::npos : slash - start);
        if (!part.empty() && part != ".") {
            std::string next = (at == "/" || (at.size() == 3 && at[1] == ':')) ? at + part : at + "/" + part;
            if (!Exists(next)) {
                std::string found = FindEntry(at, part);
                if (found.empty())
                    return path;
                next = found;
            }
            at = next;
        }
        if (slash == std::string::npos)
            break;
        start = slash + 1;
    }
    return at;
}

bool ReadFile(const std::string &path, std::string &out)
{
    std::string rel;
    if (InDisc(path, rel)) {
        if (!s_source->read(rel, out))
            return false;
    } else {
        std::ifstream f(U8Path(path), std::ios::binary);
        if (!f)
            f.open(path.c_str(), std::ios::binary); /* a path in the system's own encoding (the game folder on Windows) */
        if (!f)
            return false;
        std::ostringstream ss;
        ss << f.rdbuf();
        out = ss.str();
    }
    if (out.size() >= 3 && (unsigned char)out[0] == 0xEF && (unsigned char)out[1] == 0xBB && (unsigned char)out[2] == 0xBF)
        out.erase(0, 3); /* a byte-order mark */
    return true;
}

/* a page file as the PC's VDX7 layout: a PC release's as it is, a PlayStation release's converted as the game converts
 * it on load (langexport_psxdav.h); *psx says which. False
 * when it cannot be read or is neither. */
bool ReadPageFile(const std::string &davPath, std::string &vdx7, bool *psx)
{
    if (!ReadFile(ResolveCase(davPath), vdx7))
        return false;
    *psx = false;
    if (vdx7.size() >= 4 && memcmp(vdx7.data(), "VDX7", 4) == 0)
        return true;
    if (!ExportPsxDav_Is(vdx7.data(), (u32)vdx7.size()))
        return false;
    u32 size = 0;
    char *converted = ExportPsxDav_ToVdx7(vdx7.data(), (u32)vdx7.size(), &size);
    if (!converted)
        return false;
    vdx7.assign(converted, size);
    delete[] converted;
    *psx = true;
    return true;
}

u32 Le32(const unsigned char *p)
{
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((u32)p[3] << 24);
}

/* the game's text as UTF-8 */
std::string DecodeGame(const std::string &bytes)
{
    std::u32string out;
    for (unsigned char b : bytes)
        out += ByteToChar(b);
    return EncodeUtf8(out);
}

/* the number of text blocks the game's files' level text has: the Wheel level's .MLT's header (7 in the PC releases, 6
 * in the PAL PlayStation release's). The one level, not the first one found: the PAL PlayStation release's Fend has a
 * seventh block that its other levels do not. 0 when it is not found. */
u16 TextBlocks()
{
    std::vector<Entry> files;
    ListFolder(FindEntry(FindEntry(s_gameDir, "Levels"), "Wheel"), files);
    for (const Entry &file : files) {
        std::string data;
        if (!file.folder && file.name.size() > 4 && Lower(file.name.substr(file.name.size() - 4)) == ".mlt" &&
            ReadFile(file.path, data) && data.size() >= 12)
            return (u16)((unsigned char)data[8] | ((unsigned char)data[9] << 8));
    }
    return 0;
}

/* the first `count` lines of list 0 of block `block` of the Wheel level's text (its pause menu: "pause", "resume",
 * "options"...), as the file has them; false when there is no such file or block */
bool WheelMenuLines(u16 block, u32 count, std::vector<std::string> &lines)
{
    lines.clear();
    std::string wheel = FindEntry(FindEntry(s_gameDir, "Levels"), "Wheel"), data;
    std::vector<Entry> files;
    if (wheel.empty() || !ListFolder(wheel, files))
        return false;
    for (const Entry &file : files) {
        if (file.name.size() <= 4 || Lower(file.name.substr(file.name.size() - 4)) != ".mlt" || !ReadFile(file.path, data) ||
            data.size() <= 16)
            continue;
        u16 blockCount = (u16)((unsigned char)data[8] | ((unsigned char)data[9] << 8));
        u16 listCount = (u16)((unsigned char)data[10] | ((unsigned char)data[11] << 8));
        if (block >= blockCount || !listCount)
            return false;
        size_t at = 16;
        for (u32 b = 0; b <= block; b++)
            for (u32 l = 0; l < listCount; l++) {
                if (at >= data.size())
                    return false;
                unsigned n = (unsigned char)data[at++];
                for (unsigned k = 0; k < n; k++) {
                    size_t end = data.find('\0', at);
                    if (end == std::string::npos)
                        return false;
                    if (b == block && l == 0 && lines.size() < count)
                        lines.push_back(data.substr(at, end - at));
                    at = end + 1;
                }
            }
        return lines.size() == count;
    }
    return false;
}

/* whether the game's files are the Portuguese fan translation's: its European Portuguese is in block 1 as in block 6,
 * where every release has English in the one and Brazilian Portuguese in the other (the Wheel level's pause menu, its
 * first six lines, the same in both) */
bool TranslationOverBlock1()
{
    std::vector<std::string> english, brazilian;
    return WheelMenuLines(1, 6, english) && WheelMenuLines(6, 6, brazilian) && english == brazilian;
}

} /* namespace */

void Export_Init(const char *gameDir)
{
    s_langs.clear();
    s_gameDir = gameDir ? gameDir : ".";
    /* the languages of the blocks the level text has, in the blocks' order; the fan translation's in place of the four
     * it wrote over */
    u16 blocks = TextBlocks();
    bool translation = TranslationOverBlock1();
    auto add = [](const Block &what, u16 block) {
        Language l;
        l.name = what.name;
        l.mltBlock = block;
        l.menuMask = what.menuMask;
        l.voiceFolder = what.voiceFolder;
        l.saves = what.saves;
        s_langs.push_back(l);
        Log("Export: %s (text block %u)", l.name.c_str(), (unsigned)block);
    };
    for (u16 b = 0; b < blocks && b < kBlockCount; b++)
        if (!translation || (b != 1 && b != 2 && b != 3 && b != 6))
            add(kBlocks[b], b);
    if (translation)
        add(kTranslation, 6);
    if (blocks > kBlockCount)
        Log("Export: %s: the level text has %u blocks, past the %u languages known: not listed", s_gameDir.c_str(),
            (unsigned)blocks, (unsigned)kBlockCount);
}

s32 Export_Count()
{
    return (s32)s_langs.size();
}

namespace {

/* a level's text in language lang, as UTF-8: the .MLT's block of it (the game's encoding, decoded) */
bool LevelLists(const char *mltPath, s32 lang, std::vector<std::vector<std::string>> &lists)
{
    std::string data;
    if (!ReadFile(ResolveCase(mltPath), data))
        return false;
    /* {u32 crc; header; u32 crc; payload}, each CRC over what follows it, as the game's reader (File_ReadChecked) checks */
    const unsigned char *d = (const unsigned char *)data.data();
    if (data.size() < 12 || Crc32(d + 4, 8) != Le32(d) ||
        (data.size() > 16 && Crc32(d + 16, (int)(data.size() - 16)) != Le32(d + 12))) {
        Log("Export: %s is damaged (its checksums do not match): not loaded", mltPath);
        return false;
    }
    lists.clear();
    if (data.size() <= 16) /* no payload: an empty bank, as the game's reader leaves it */
        return true;
    u16 blockCount, listCount;
    memcpy(&blockCount, data.data() + 8, 2);
    memcpy(&listCount, data.data() + 10, 2);
    u16 block = s_langs[lang].mltBlock;
    lists.assign(listCount, {});
    if (block >= blockCount) {
        Log("Export: %s has no text block %u: its lines are left empty", mltPath, (unsigned)block);
        return true;
    }
    size_t at = 16;
    for (u32 b = 0; b <= block; b++)
        for (u32 l = 0; l < listCount; l++) {
            if (at >= data.size())
                return false;
            unsigned count = (unsigned char)data[at++];
            for (unsigned k = 0; k < count; k++) {
                size_t end = data.find('\0', at);
                if (end == std::string::npos)
                    return false;
                if (b == block)
                    lists[l].push_back(DecodeGame(data.substr(at, end - at)));
                at = end + 1;
            }
        }
    return true;
}

} /* namespace */

void Export_SetLog(void (*log)(const char *text))
{
    s_log = log ? log : DefaultLog;
}

void Export_Logf(const char *fmt, ...)
{
    char text[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(text, sizeof(text), fmt, ap);
    va_end(ap);
    s_log(text);
}

namespace {

/* one line of a text file: its header, then its text with "@" at a line's start doubled */
void WriteEntry(std::string &out, const std::string &header, const std::string &utf8)
{
    out += "@" + header + "\n";
    size_t pos = 0;
    while (pos <= utf8.size()) {
        size_t eol = utf8.find('\n', pos);
        std::string line = utf8.substr(pos, eol == std::string::npos ? std::string::npos : eol - pos);
        if (!line.empty() && line[0] == '@')
            out += "@";
        out += line + "\n";
        if (eol == std::string::npos)
            break;
        pos = eol + 1;
    }
    out += "\n";
}

bool WriteFile(const fs::path &path, const std::string &data)
{
    std::ofstream f(path, std::ios::binary);
    if (!f)
        return false;
    f.write(data.data(), (std::streamsize)data.size());
    return (bool)f;
}

} /* namespace */

bool Export_WritePack(s32 lang, const char *outDir, const char *name, bool withVoices, char *error, u32 errorSize)
{
    auto fail = [&](const std::string &why) {
        snprintf(error, errorSize, "%s", why.c_str());
        return false;
    };
    if (lang < 0 || lang >= (s32)s_langs.size())
        return fail("No such language.");
    const Language &from = s_langs[lang];
    fs::path out = U8Path(outDir), text = out / "text";
    std::error_code ec;
    if (fs::exists(out, ec))
        return fail(std::string(outDir) + " is already there.");
    fs::create_directories(text, ec);
    if (ec)
        return fail("Cannot make " + PathU8(text) + ": " + ec.message());

    /* its voices, when asked for and the game's files have them: every file of its voice folder, into the pack's
     * voices/; the pack then has its own voices (language.txt names none: a pack with voices/ starts with its own) */
    std::string folder = FindEntry(s_gameDir + "/Voices", from.voiceFolder);
    if (withVoices && !folder.empty()) {
        std::vector<Entry> clips;
        if (!ListFolder(folder, clips))
            return fail("Cannot read the voices of " + from.name + ".");
        fs::create_directories(out / "voices", ec);
        for (const Entry &clip : clips) {
            std::string data;
            if (clip.folder)
                continue;
            if (!ReadFile(clip.path, data) || !WriteFile(out / "voices" / U8Path(clip.name), data))
                return fail("Cannot copy " + clip.path + ".");
        }
    }
    /* language.txt is written last, by Export_Language: the game lists a folder only once it has one */
    (void)name;

    /* every level's text */
    std::vector<Entry> levels, files;
    ListFolder(FindEntry(s_gameDir, "Levels"), levels);
    for (const Entry &level : levels) {
        if (!level.folder || !ListFolder(level.path, files))
            continue;
        for (const Entry &file : files) {
            if (file.name.size() < 4 || Lower(file.name.substr(file.name.size() - 4)) != ".mlt")
                continue;
            std::vector<std::vector<std::string>> lists;
            if (!LevelLists(file.path.c_str(), lang, lists))
                return fail("Cannot read " + file.path + ".");
            std::string data;
            for (size_t l = 0; l < lists.size(); l++)
                for (size_t n = 0; n < lists[l].size(); n++)
                    WriteEntry(data, std::to_string(l) + "." + std::to_string(n), lists[l][n]);
            /* by the level's folder, as the PC release names it (a PlayStation disc's LEV00 is Lvl-00) */
            if (!WriteFile(text / U8Path(std::string(Export_LevelName(level.name.c_str())) + ".txt"), data))
                return fail("Cannot write the text of " + file.name + ".");
        }
    }

    /* the menus: GREET_txt.BSM's entries of its language bit */
    std::string bsm, menus;
    std::string references = FindEntry(s_gameDir, "References");
    std::string greet = references.empty() ? "" : FindEntry(references, "GREET_txt.BSM");
    if (greet.empty() || !ReadFile(greet, bsm))
        bsm = greetCatalogue(); /* a PlayStation disc's files have none: the one the launcher installs it with */
    if (bsm.size() < 16 || bsm.compare(0, 12, "GREETINGV1.0") != 0)
        return fail("Cannot read References/GREET_txt.BSM.");
    u16 mask = from.menuMask;
    u32 count;
    memcpy(&count, bsm.data() + 12, 4);
    size_t at = 16;
    for (u32 i = 0; i < count && at + 8 <= bsm.size(); i++) {
        u32 key, len;
        memcpy(&key, bsm.data() + at, 4);
        memcpy(&len, bsm.data() + at + 4, 4);
        at += 8;
        if (at + len > bsm.size())
            break;
        std::string t = bsm.substr(at, len);
        at += len;
        if ((key & 0xFF) != mask)
            continue;
        u16 group = (u16)(key >> 24), index = (u16)((key >> 8) & 0xFFFF);
        while (!t.empty() && t.back() == 0)
            t.pop_back();
        char header[32];
        snprintf(header, sizeof(header), "0x%X.0x%X", (unsigned)group, (unsigned)index);
        WriteEntry(menus, header, DecodeGame(t));
    }
    /* Rewooled's own menu items, in the catalogue's group of their own (rewooled_menu_strings.h) */
    for (u16 index = 1; index <= EXPORT_REWOOLED_MENU_COUNT; index++) {
        const char *own = ExportTables_RewooledMenuString(mask, index);
        char header[32];
        snprintf(header, sizeof(header), "0x%X.0x%X", (unsigned)EXPORT_REWOOLED_MENU_GROUP, (unsigned)index);
        WriteEntry(menus, header, DecodeGame(own ? own : ""));
    }
    if (!WriteFile(text / "Menus.txt", menus))
        return fail("Cannot write Menus.txt.");

    /* the save messages: Rewooled's own, of its language number; for a language Rewooled has none of, its English ones,
     * for the pack's maker to rewrite */
    std::string saves;
    s32 savesOf = from.saves >= 0 ? from.saves : GAME_LANG_ENGLISH;
    for (u32 id = 0; id < LANGPACK_SAVE_STRINGS; id++)
        WriteEntry(saves, std::to_string(id), DecodeGame(kExportSaveMessages[savesOf & 3][id]));
    if (!WriteFile(text / "Saves.txt", saves))
        return fail("Cannot write Saves.txt.");
    return true;
}

const char *Export_NameUtf8(s32 lang)
{
    return lang >= 0 && lang < (s32)s_langs.size() ? s_langs[lang].name.c_str() : "";
}

const char *Export_LevelName(const char *folder)
{
    static std::string name;
    static const char *const kPs1[][2] = {{"ENDING", "Ending"}, {"FEND", "Fend"},   {"INTRO", "Intro"},
                                          {"SCENE", "Scene"},   {"WHEEL", "Wheel"}, {"CREDITS", "Credits"}};
    name = folder ? folder : "";
    if (name.size() == 5 && name.compare(0, 3, "LEV") == 0 && isdigit((unsigned char)name[3]) &&
        isdigit((unsigned char)name[4]))
        name = "Lvl-" + name.substr(3);
    for (const auto &m : kPs1)
        if (name == m[0])
            name = m[1];
    return name.c_str();
}

bool Export_OpenSource(const char *path, char *error, u32 errorSize)
{
    std::string at = path ? path : "";
    while (at.size() > 1 && (at.back() == '/' || at.back() == '\\'))
        at.pop_back();
    std::error_code ec;
    s_source.reset();
    if (!fs::is_directory(U8Path(at), ec)) {
        std::string why;
        s_source = DiscFiles::open(U8Path(at), why);
        if (!s_source) {
            snprintf(error, errorSize, "%s", why.c_str());
            return false;
        }
    }
    Export_Init(at.c_str());
    if (FindEntry(s_gameDir, "Levels").empty()) {
        snprintf(error, errorSize, "%s has no Levels folder: it is not the game's files.", at.c_str());
        return false;
    }
    return true;
}

/* ---- the rest of an export (Export_Language): the lettered pictures as the pack's textures (langexport_lettered.h) ---- */

namespace {

bool SameText(const char *a, const char *b)
{
    return SDL_strcasecmp(a, b) == 0;
}

u64 Fnv64(const unsigned char *p, size_t n, u64 h = 14695981039346656037ull)
{
    for (size_t i = 0; i < n; i++)
        h = (h ^ p[i]) * 1099511628211ull;
    return h;
}

/* a picture of a page file: its size, its page's pixel format, and its texels as the file has them (16-bit
 * little-endian, row by row) */
struct Picture {
    u32 w = 0, h = 0, format = 0;
    std::string texels;
};

/* a picture's fingerprint (langexport_lettered.h), as the game's page loader computes it */
u64 Fingerprint(const Picture &p)
{
    const unsigned char head[5] = {(unsigned char)(p.format & 3), (unsigned char)p.w, (unsigned char)(p.w >> 8),
                                   (unsigned char)p.h, (unsigned char)(p.h >> 8)};
    return Fnv64((const unsigned char *)p.texels.data(), p.texels.size(), Fnv64(head, 5));
}

/* the pictures of a page file in the PC's layout (VDX7), by number, as PolyBatcher::LoadTexturePages reads its picture
 * list and pages; a picture whose rectangle is not on its page is left empty (w 0). False when it is not laid out so. */
bool ReadPictures(const std::string &dav, std::vector<Picture> &pictures)
{
    const unsigned char *d = (const unsigned char *)dav.data();
    auto u16At = [&](size_t at) { return (u32)(d[at] | (d[at + 1] << 8)); };
    pictures.clear();
    if (dav.size() < 0x18 || memcmp(d, "VDX7", 4) != 0)
        return false;
    u32 header = Le32(d + 0x14);
    if ((size_t)header + 18 > dav.size())
        return false;
    u32 rects = u16At(header + 2), pages = u16At(header + 4), ra = Le32(d + header + 10), pa = Le32(d + header + 14);
    if ((size_t)ra + 10 * (size_t)rects > dav.size())
        return false;
    struct Page {
        size_t at;
        u32 w, h, format;
    };
    std::vector<Page> list;
    size_t at = pa;
    for (u32 i = 0; i < pages; i++) {
        if (at + 10 > dav.size())
            return false;
        u32 w = (u32)abs((int)u16At(at) - (int)u16At(at + 2)), h = (u32)abs((int)u16At(at + 4) - (int)u16At(at + 6));
        u32 format = u16At(at + 8);
        at += 10;
        if (at + (size_t)w * h * 2 > dav.size())
            return false;
        list.push_back({at, w, h, format});
        at += (size_t)w * h * 2;
    }
    pictures.resize(rects);
    for (u32 r = 0; r < rects; r++) {
        size_t rec = ra + 10 * (size_t)r;
        s32 x = (s16)u16At(rec), w = (s16)u16At(rec + 2), y = (s16)u16At(rec + 4), h = (s16)u16At(rec + 6);
        u32 page = u16At(rec + 8);
        if (page >= list.size() || w <= 0 || h <= 0 || x < 0 || y < 0 || (u32)(x + w) > list[page].w ||
            (u32)(y + h) > list[page].h)
            continue;
        Picture &p = pictures[r];
        p.w = (u32)w;
        p.h = (u32)h;
        p.format = list[page].format;
        for (s32 row = 0; row < h; row++)
            p.texels.append(dav, list[page].at + ((size_t)(y + row) * list[page].w + x) * 2, (size_t)w * 2);
    }
    return true;
}

/* a picture's texels (format & 3: RGB565, ARGB1555, ARGB4444) as RGBA with the usual alpha (opacity: the game's is
 * transparency), into rgba at x, y of a canvas `width` pixels wide */
void PictureRgba(const Picture &p, unsigned char *rgba, u32 width, u32 x0, u32 y0)
{
    const unsigned char *t = (const unsigned char *)p.texels.data();
    for (u32 y = 0; y < p.h; y++)
        for (u32 x = 0; x < p.w; x++) {
            u32 c = t[(y * p.w + x) * 2] | (t[(y * p.w + x) * 2 + 1] << 8), r, g, b, clear;
            switch (p.format & 3) {
                case 1:
                    r = (c >> 10 & 31) * 255 / 31, g = (c >> 5 & 31) * 255 / 31, b = (c & 31) * 255 / 31;
                    clear = 255 * (c >> 15);
                    break;
                case 2:
                    r = (c >> 8 & 15) * 17, g = (c >> 4 & 15) * 17, b = (c & 15) * 17, clear = (c >> 12 & 15) * 17;
                    break;
                default:
                    r = (c >> 11 & 31) * 255 / 31, g = (c >> 5 & 63) * 255 / 63, b = (c & 31) * 255 / 31, clear = 0;
            }
            unsigned char *px = rgba + ((size_t)(y0 + y) * width + x0 + x) * 4;
            px[0] = (unsigned char)r, px[1] = (unsigned char)g, px[2] = (unsigned char)b;
            px[3] = (unsigned char)(255 - clear);
        }
}

/* RGBA (w x h) as a PNG */
bool SaveRgbaPng(const std::vector<unsigned char> &rgba, u32 w, u32 h, const fs::path &path)
{
    SDL_Surface *surface = SDL_CreateSurfaceFrom((int)w, (int)h, SDL_PIXELFORMAT_RGBA32, (void *)rgba.data(), (int)w * 4);
    bool ok = surface && SDL_SavePNG(surface, PathU8(path).c_str());
    SDL_DestroySurface(surface);
    return ok;
}

bool WritePicturePng(const Picture &p, const fs::path &path)
{
    std::vector<unsigned char> rgba((size_t)p.w * p.h * 4);
    PictureRgba(p, rgba.data(), p.w, 0, 0);
    return SaveRgbaPng(rgba, p.w, p.h, path);
}

/* a texture's file name: its fingerprint as 16 hex digits */
std::string TextureName(u64 fingerprint)
{
    char name[32];
    snprintf(name, sizeof(name), "%016llx.png", (unsigned long long)fingerprint);
    return name;
}

bool IsLettered(u64 fingerprint)
{
    return std::binary_search(std::begin(kLetteredPictures), std::end(kLetteredPictures), fingerprint);
}

/* the level file's lettered pictures its files re-lettered in place (a translation's): the release whose places for the level fit the
 * file (as many pictures, each lettered one's the same size), the one with the most of them unchanged, says where they
 * are; each changed one into relettered by the fingerprint it had, where it has none yet */
void FindRelettered(const std::string &level, const std::vector<Picture> &pictures, const std::vector<u64> &fingerprints,
                    std::map<u64, Picture> &relettered)
{
    /* the release the file differs least from (fewest of its places changed), then the one with the most unchanged: two
     * releases' files can fit alike (the PlayStation releases' Fend), and the file's own release is the one it has
     * nothing changed from */
    const LetteredLevel *best = 0;
    u32 bestSame = 0, bestChanged = 0;
    for (const LetteredLevel &lv : kLetteredLevels) {
        if (!SameText(lv.level, level.c_str()) || lv.pictures != pictures.size())
            continue;
        bool fits = true;
        u32 same = 0;
        for (u32 i = lv.first; i < (u32)lv.first + lv.count && fits; i++) {
            const LetteredPlace &place = kLetteredPlaces[i];
            fits = pictures[place.picture].w == place.w && pictures[place.picture].h == place.h;
            same += fits && fingerprints[place.picture] == place.fingerprint;
        }
        u32 changed = lv.count - same;
        if (fits && (!best || changed < bestChanged || (changed == bestChanged && same > bestSame)))
            best = &lv, bestSame = same, bestChanged = changed;
    }
    if (!best)
        return;
    for (u32 i = best->first; i < (u32)best->first + best->count; i++) {
        const LetteredPlace &place = kLetteredPlaces[i];
        if (fingerprints[place.picture] != place.fingerprint && !relettered.count(place.fingerprint)) {
            relettered[place.fingerprint] = pictures[place.picture];
            Log("Export: %s picture %u re-lettered (as the %s release's %016llx)", level.c_str(),
                (unsigned)place.picture, best->release, (unsigned long long)place.fingerprint);
        }
    }
}

/* ---- the game's font, as a pack's font/ (docs/language-packs.md: Font): font.png, the font of the game's files'
 * Lvl-00 (every level's but Intro's is the same grid), and characters.txt ---- */

/* the character each cell of the game's font draws, 16 to a row from 0x20, as UTF-8 (looked at cell by cell): the game
 * draws a character in one of three cells, its own and two variants (text.cpp: s_glyphVariantFwdTable,
 * s_glyphVariantRevTable, picked at random), so most characters are in it three times, and the font has capitals only
 * (a small letter is drawn as its capital's variants); a button picture's cells are spaces. The PlayStation releases'
 * font has its letters in the same cells (its buttons are its own). */
const char kFontCharacters[] = " !\xC3\x93\xC3\x83\xC3\x93\xC3\x92\xC3\x94'()\xC3\x95+,-./\n"
                               "0123456789:\xC3\x96\xC3\x8D=\xC3\x8C?\n"
                               "\xC3\x9A" "ABCDEFGHIJKLMNO\n"
                               "PQRSTUVWXYZ\xC3\x89    \n"
                               "\xC3\x99" "ABCDEFGHIJKLMNO\n"
                               "PQRSTUVWXYZ\xC3\x88    \n"
                               "\xC3\x9B" "ABCDEFGHIJKLMNO\n"
                               "PQRSTUVWXYZ\xC3\x8A    \n"
                               "\xC3\x9C\xC2\xA1" "0123456789    \n"
                               "\xC3\x87\xC3\x91" "0123456789\xC3\x80\xC3\x81\xC3\x82\xC2\xBF\n"
                               "\xC3\x80\xC3\x81\xC3\x82\xC3\x83\xC3\x84  \xC3\x87\xC3\x88\xC3\x89\xC3\x8A \xC3\x8C\xC3\x8D  \n"
                               " \xC3\x91\xC3\x92\xC3\x93\xC3\x94\xC3\x95\xC3\x96  \xC3\x99\xC3\x9A\xC3\x9B\xC3\x9C \xC3\x8E \n"
                               "\xC3\x80\xC3\x81\xC3\x82\xC3\x83\xC3\x84  \xC3\x87\xC3\x88\xC3\x89\xC3\x8A \xC3\x8C\xC3\x8D  \n"
                               " \xC3\x91\xC3\x92\xC3\x93\xC3\x94\xC3\x95\xC3\x96  \xC3\x99\xC3\x9A\xC3\x9B\xC3\x9C  \xC3\x84\n";

/* the game font's picture in a page file (VDX7): its number in the file's picture list, the first entry of its list
 * DAV_IDI_IGLTYPO_ (0x24); false when it has none */
bool FontPictureIn(const std::string &dav, u32 *picture)
{
    auto u32At = [&](u32 at, u32 bytes) -> u32 {
        if ((size_t)at + bytes > dav.size())
            return 0xFFFFFFFF;
        unsigned char b[4] = {0, 0, 0, 0};
        memcpy(b, dav.data() + at, bytes);
        return Le32(b);
    };
    if (dav.size() < 0x18 || memcmp(dav.data(), "VDX7", 4) != 0)
        return false;
    u32 header = u32At(0x14, 4), rects = u32At(header + 2, 2), ra = u32At(header + 10, 4), ea = u32At(header + 6, 4);
    u32 entries = u32At(header, 2), la = u32At(header + 18, 4), lists = u32At(la, 4), at = la + 4;
    for (u32 n = 0; n < lists && n < 0x10000; n++) {
        u32 hd = u32At(at, 4), count = hd >> 16;
        if (hd == 0xFFFFFFFF)
            return false;
        if ((hd & 0xFFFF) == 0x24 && count) {
            u32 o = u32At(at + 4, 4), rect;
            if (o >= ra && o < ra + 10 * rects)
                rect = (o - ra) / 10;
            else if (o >= ea && o < ea + 2 * entries)
                rect = u32At(o, 2);
            else
                return false;
            if (rect >= rects)
                return false;
            *picture = rect;
            return true;
        }
        at += 4 + 4 * count;
    }
    return false;
}

/* the game's font into folder (font.png, characters.txt). False with the reason in error. */
bool WriteFont(const fs::path &folder, std::string &error)
{
    std::vector<Entry> levels, files;
    std::string davPath;
    ListFolder(FindEntry(s_gameDir, "Levels"), levels);
    for (const Entry &level : levels)
        if (level.folder && SameText(Export_LevelName(level.name.c_str()), "Lvl-00") && ListFolder(level.path, files))
            for (const Entry &e : files)
                if (!e.folder && e.name.size() > 4 && Lower(e.name.substr(e.name.size() - 4)) == ".dav")
                    davPath = e.path;
    std::string dav;
    bool psx = false;
    std::vector<Picture> pictures;
    u32 picture = 0;
    if (davPath.empty() || !ReadPageFile(davPath, dav, &psx) || !ReadPictures(dav, pictures) ||
        !FontPictureIn(dav, &picture) || picture >= pictures.size() || pictures[picture].w != 256 ||
        pictures[picture].h != 224) {
        error = "The game's font was not found (in Lvl-00's page file).";
        return false;
    }
    std::error_code ec;
    fs::create_directories(folder, ec);
    if (ec || !WritePicturePng(pictures[picture], folder / "font.png")) {
        error = "Cannot write " + PathU8(folder / "font.png") + ".";
        return false;
    }
    if (!WriteFile(folder / "characters.txt", kFontCharacters)) {
        error = "Cannot write " + PathU8(folder / "characters.txt") + ".";
        return false;
    }
    return true;
}

/* ---- the language wheel's pictures: the Wheel level file's, one per language the release's wheel has, by its entry
 * number, in an order of each release's own. A release is known by its files: a PlayStation one's page files are its
 * own layout, PlayStation PAL has six text blocks and NTSC seven, and the PC PAL release's pictures are 180 x 124 where
 * NTSC's are 128 x 128 (the fan translation, built on PAL's wheel, has PAL's). ---- */

struct WheelLayout {
    bool psx;
    u16 blocks;
    u32 w, h;           /* its pictures' size */
    s32 entryOfBlock[7]; /* the entry of each text block's language's picture; -1: none */
};

const WheelLayout kWheelLayouts[] = {
    {false, 7, 180, 124, {-1, 46, 47, 48, -1, -1, 49}}, /* PC PAL: English, Spanish, Italian, Brazilian */
    {false, 7, 128, 128, {47, 46, 49, -1, -1, -1, 48}}, /* PC NTSC: English, French, Brazilian, Spanish */
    {true, 6, 128, 128, {43, 41, 42, 45, 40, 44, -1}},  /* PS1 PAL: German, English, Spanish, French, Dutch, Italian */
    {true, 7, 128, 128, {60, 59, 62, -1, -1, -1, 61}},  /* PS1 NTSC: English, French, Brazilian, Spanish */
};

/* picture `entry` of a page file (VDX7), by its entry table; false when it has none */
bool EntryPicture(const std::string &dav, const std::vector<Picture> &pictures, s32 entry, Picture &out)
{
    const unsigned char *d = (const unsigned char *)dav.data();
    if (entry < 0 || dav.size() < 0x18)
        return false;
    u32 header = Le32(d + 0x14);
    if ((size_t)header + 10 > dav.size())
        return false;
    u32 entries = d[header] | (d[header + 1] << 8), ea = Le32(d + header + 6);
    if ((u32)entry >= entries || (size_t)ea + 2 * (size_t)entries > dav.size())
        return false;
    u32 r = d[ea + 2 * entry] | (d[ea + 2 * entry + 1] << 8);
    if (r >= pictures.size() || !pictures[r].w)
        return false;
    out = pictures[r];
    return true;
}

/* the wheel picture of language lang as wheel.png at path: its own, when the release's wheel has one; else the first of
 * the wheel's (English's), for the pack's maker to draw over. False with the reason in error. */
bool WriteWheelPicture(s32 lang, const fs::path &path, std::string &error)
{
    std::string wheel = FindEntry(FindEntry(s_gameDir, "Levels"), "Wheel"), dav;
    std::vector<Entry> files;
    std::string file;
    if (!wheel.empty() && ListFolder(wheel, files))
        for (const Entry &e : files)
            if (!e.folder && e.name.size() > 4 && Lower(e.name.substr(e.name.size() - 4)) == ".dav")
                file = e.path;
    bool psx = false;
    std::vector<Picture> pictures;
    if (file.empty() || !ReadPageFile(file, dav, &psx) || !ReadPictures(dav, pictures)) {
        error = "Cannot read the Wheel level's pictures.";
        return false;
    }
    u16 blocks = TextBlocks();
    for (const WheelLayout &layout : kWheelLayouts) {
        Picture english;
        if (layout.psx != psx || layout.blocks != blocks || !EntryPicture(dav, pictures, layout.entryOfBlock[1], english) ||
            english.w != layout.w || english.h != layout.h)
            continue;
        u16 block = s_langs[lang].mltBlock;
        Picture own;
        bool found = block < 7 && EntryPicture(dav, pictures, layout.entryOfBlock[block], own);
        if (!found)
            Log("Export: the wheel has no picture of %s: English's written, to draw over", s_langs[lang].name.c_str());
        if (!WritePicturePng(found ? own : english, path)) {
            error = "Cannot write " + PathU8(path) + ".";
            return false;
        }
        return true;
    }
    error = "The Wheel level's pictures are laid out as no release's known.";
    return false;
}

/* the pack's textures, into folder: the lettered pictures of the game's files, each once, as the files have them (those
 * they re-lettered in place too: FindRelettered): a sign in pieces as one texture (<name>.png, langexport_lettered.h:
 * kLetteredJoined, as the game puts it back: langpack.cpp's kJoinedSigns), every other picture as <fingerprint>.png.
 * False with the reason in error. */
bool WriteTextures(const fs::path &folder, std::string &error)
{
    std::map<u64, Picture> lettered, relettered;
    std::vector<Entry> levels, files;
    ListFolder(FindEntry(s_gameDir, "Levels"), levels);
    for (const Entry &level : levels) {
        if (!level.folder || !ListFolder(level.path, files))
            continue;
        for (const Entry &file : files) {
            if (file.folder || file.name.size() < 4 || !SameText(file.name.substr(file.name.size() - 4).c_str(), ".dav"))
                continue;
            std::string dav;
            bool psx = false;
            std::vector<Picture> pictures;
            if (!ReadPageFile(file.path, dav, &psx) || !ReadPictures(dav, pictures)) {
                error = "Cannot read the pictures of " + file.path + ".";
                return false;
            }
            std::vector<u64> fingerprints(pictures.size());
            for (size_t i = 0; i < pictures.size(); i++) {
                if (!pictures[i].w)
                    continue;
                fingerprints[i] = Fingerprint(pictures[i]);
                if (IsLettered(fingerprints[i]) && !lettered.count(fingerprints[i]))
                    lettered[fingerprints[i]] = pictures[i];
            }
            FindRelettered(level.name, pictures, fingerprints, relettered);
        }
    }
    for (const auto &kv : relettered)
        lettered[kv.first] = kv.second;

    std::error_code ec;
    fs::create_directories(folder, ec);
    if (ec) {
        error = "Cannot make " + PathU8(folder) + ": " + ec.message() + ".";
        return false;
    }
    /* the signs in pieces the game's files have, each as one texture */
    for (const LetteredJoined &j : kLetteredJoined) {
        std::string name = std::string(j.name) + ".png";
        std::vector<unsigned char> rgba((size_t)j.w * j.h * 4, 0);
        bool any = false;
        for (u32 i = j.first; i < (u32)j.first + j.count; i++) {
            const LetteredPiece &piece = kLetteredPieces[i];
            auto it = lettered.find(piece.fingerprint);
            if (it == lettered.end())
                continue;
            if (piece.x + it->second.w > j.w || piece.y + it->second.h > j.h) {
                error = "The sign " + name + " cannot hold its piece " + TextureName(piece.fingerprint) + ".";
                return false;
            }
            PictureRgba(it->second, rgba.data(), j.w, piece.x, piece.y);
            lettered.erase(it);
            any = true;
        }
        if (!any)
            continue;
        if (!SaveRgbaPng(rgba, j.w, j.h, folder / U8Path(name))) {
            error = "Cannot write " + PathU8(folder / U8Path(name)) + ".";
            return false;
        }
    }
    /* every other lettered picture, by its fingerprint */
    for (const auto &kv : lettered)
        if (!WritePicturePng(kv.second, folder / U8Path(TextureName(kv.first)))) {
            error = "Cannot write " + PathU8(folder / U8Path(TextureName(kv.first))) + ".";
            return false;
        }
    return true;
}

} /* namespace */

bool Export_Language(s32 lang, const char *outDir, const char *name, bool withVoices, bool withTextures, char *error,
                     u32 errorSize)
{
    fs::path out = U8Path(outDir);
    std::error_code ec;
    if (lang < 0 || lang >= (s32)s_langs.size()) {
        snprintf(error, errorSize, "No such language.");
        return false;
    }
    if (fs::exists(out, ec)) { /* someone's folder: not this export's, so never removed */
        snprintf(error, errorSize, "%s is already there.", outDir);
        return false;
    }
    /* its text, menus and save messages, and its voices when asked for; its wheel picture, the game's font, and the
     * lettered pictures when asked for; language.txt last, so that the game lists the folder only once it is whole */
    std::string why;
    bool ok = Export_WritePack(lang, outDir, name, withVoices, error, errorSize);
    if (ok && (!WriteWheelPicture(lang, out / "wheel.png", why) || !WriteFont(out / "font", why) ||
               (withTextures && !WriteTextures(out / "textures", why)))) {
        snprintf(error, errorSize, "%s", why.c_str());
        ok = false;
    }
    if (ok) {
        const Language &from = s_langs[lang];
        bool voices = fs::exists(out / "voices", ec); /* its own voices, copied: language.txt names none */
        std::string desc = "name = " + std::string(name && *name ? name : from.name.c_str()) + "\n" +
                           (voices ? "" : "voices = " + from.name + "\n");
        if (!WriteFile(out / "language.txt", desc)) {
            snprintf(error, errorSize, "Cannot write language.txt.");
            ok = false;
        }
    }
    if (!ok) /* nothing half-written left behind: the folder is this export's own */
        fs::remove_all(out, ec);
    return ok;
}
