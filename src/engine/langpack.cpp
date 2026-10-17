/* Language packs: see langpack.h and docs/language-packs.md. */
#include "langpack.h"
#include "crc32.h"
#include "psx_dav.h"
#include "sdw_enums.h"
#include "../platform/save_store.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

/* sdw_classes.h's StringBank (that header cannot be included beside the C library's: its own FILE), the same layout */
struct StringBank {
    u16 listCount;
    char **lists;
};

extern const char *g_mcardStrings[4][51]; /* src/objects/mcard.cpp: the save messages, by config language */
/* the game's own languages: its table's names and language numbers (src/objects/scene_wheel.cpp), the .MLT block it
 * reads for a language number (src/engine/jpeg_mlt.cpp), the folder it plays its voices from
 * (src/engine/stream_player.cpp) */
extern const char *g_sceneWheelLanguageNames[];
extern u8 g_sceneWheelLanguageIds[];
extern const u32 g_sceneWheelLanguageCount;
void Mlt_MapLanguageToBlock(u16 *lang);
const char *StreamPlayer_VoiceFolder(s32 language);
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

/* ---- the game's text encoding: Windows-1252, every byte kept (the five bytes 1252 leaves out are U+0081...) ---- */

const char32_t kCp1252High[32] = {0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160,
                                  0x2039, 0x0152, 0x008D, 0x017D, 0x008F, 0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022,
                                  0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};

char32_t ByteToChar(unsigned char b)
{
    return b >= 0x80 && b < 0xA0 ? kCp1252High[b - 0x80] : (char32_t)b;
}

/* the decoded characters of UTF-8 text (a bad byte is taken as Windows-1252) */
std::u32string DecodeUtf8(const std::string &s)
{
    std::u32string out;
    size_t i = 0;
    while (i < s.size()) {
        unsigned char c = (unsigned char)s[i];
        int n = c < 0x80 ? 0 : (c & 0xE0) == 0xC0 ? 1 : (c & 0xF0) == 0xE0 ? 2 : (c & 0xF8) == 0xF0 ? 3 : -1;
        char32_t cp = n == 0 ? c : n == 1 ? (c & 0x1F) : n == 2 ? (c & 0x0F) : (c & 0x07);
        bool ok = n >= 0 && i + n < s.size();
        for (int k = 1; ok && k <= n; k++) {
            unsigned char d = (unsigned char)s[i + k];
            ok = (d & 0xC0) == 0x80;
            cp = (cp << 6) | (d & 0x3F);
        }
        if (!ok) {
            out += ByteToChar(c);
            i++;
        } else {
            out += cp;
            i += n + 1;
        }
    }
    return out;
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

/* ---- the languages ---- */

struct Language {
    std::string id;       /* its key in the text cache: a pack's folder, a baked-in language's name (never compared) */
    std::string name;     /* shown (UTF-8) */
    std::string shown;    /* the name in the game's encoding */
    std::string dir;      /* a pack's folder, UTF-8; "" for a baked-in language */
    /* a baked-in language: one of the game's own (scene_wheel.cpp: g_sceneWheelLanguageNames), as the game reads it */
    s32 configLang = -1;  /* its language number (Progress.language), or -1 for a pack */
    u16 mltBlock = 0;     /* its .MLT block (Mlt_MapLanguageToBlock) */
    std::string voiceFolder; /* its folder under Voices/ (StreamPlayer_VoiceFolder) */
    s32 wheelSlot = -1;   /* its place in the game's table: the wheel model's picture it shows; -1 for a pack */
    /* a pack, as its language.txt names them */
    std::string voices;   /* the voices it starts with ("": its own) */
    bool hasVoices = false;
    /* a pack's text files, read when first needed: file name -> header -> text (UTF-8) */
    std::map<std::string, std::map<std::string, std::string>> text;
    std::map<std::string, bool> textRead;
};

std::vector<Language> s_langs; /* the languages, built at start-up (LangPack_Init): LangPack_Count's, in order */
/* the active text language's font (a pack's font/), when it has one: font.png, and its cells' characters */
std::string s_fontPng;
std::map<char32_t, unsigned char> s_fontCodes;
char32_t s_cellChars[256]; /* the active pack font's cells' characters: 0 for a cell left out (a button picture) */
bool s_levelFont = true;   /* the loaded level's game font is the game's grid (langpack.h: LangPack_LoadLevelText) */
std::string s_gameDir;
s32 s_chosenText = 0, s_activeText = 0, s_chosenVoices = -1;
std::map<std::string, std::string> s_encodedCache; /* save and menu strings handed out: kept alive */

const char *kCfgKey = "CfgGame";
const unsigned kRegSz = 1;

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

/* the entry of `parent` named `name` in any case ("" when there is none) */
std::string FindEntry(const std::string &parent, const std::string &name)
{
    std::error_code ec;
    for (fs::directory_iterator it(U8Path(parent), ec), end; !ec && it != end; it.increment(ec)) {
        std::string n = PathU8(it->path().filename());
        if (n.size() == name.size() &&
            std::equal(n.begin(), n.end(), name.begin(), [](char a, char b) { return tolower((unsigned char)a) == tolower((unsigned char)b); }))
            return PathU8(it->path());
    }
    return "";
}

/* FindEntry for folders read often (the voice folders: a clip's file is found each time it plays): each folder listed
 * once, its entries by lower-case name */
std::map<std::string, std::map<std::string, std::string>> s_listings;

std::string Lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
    return s;
}

std::string FindEntryListed(const std::string &parent, const std::string &name)
{
    auto at = s_listings.find(parent);
    if (at == s_listings.end()) {
        at = s_listings.emplace(parent, std::map<std::string, std::string>()).first;
        std::error_code ec;
        for (fs::directory_iterator it(U8Path(parent), ec), end; !ec && it != end; it.increment(ec))
            at->second.emplace(Lower(PathU8(it->path().filename())), PathU8(it->path()));
    }
    auto e = at->second.find(Lower(name));
    return e == at->second.end() ? "" : e->second;
}

/* path with each part spelled as on disk, where it is spelled in another case ("Voices/english" for Voices/English);
 * as given when it is there, or cannot be found */
std::string ResolveCase(const std::string &path)
{
    std::error_code ec;
    if (fs::exists(U8Path(path), ec))
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
            if (!fs::exists(U8Path(next), ec)) {
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
    std::ifstream f(U8Path(path), std::ios::binary);
    if (!f)
        f.open(path.c_str(), std::ios::binary); /* a path in the system's own encoding (the game folder on Windows) */
    if (!f)
        return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    out = ss.str();
    if (out.size() >= 3 && (unsigned char)out[0] == 0xEF && (unsigned char)out[1] == 0xBB && (unsigned char)out[2] == 0xBF)
        out.erase(0, 3); /* a byte-order mark */
    return true;
}

/* a page file as the PC's VDX7 layout: a PC release's as it is, a PlayStation release's converted as the game converts
 * it on load (psx_dav.h; the conversion is kept, so the level loader's own costs nothing more); *psx says which. False
 * when it cannot be read or is neither. */
bool ReadPageFile(const std::string &davPath, std::string &vdx7, bool *psx)
{
    if (!ReadFile(ResolveCase(davPath), vdx7))
        return false;
    *psx = false;
    if (vdx7.size() >= 4 && memcmp(vdx7.data(), "VDX7", 4) == 0)
        return true;
    if (!PsxDav_Is(vdx7.data(), (u32)vdx7.size()))
        return false;
    u32 size = 0;
    char *converted = PsxDav_ToVdx7(vdx7.data(), (u32)vdx7.size(), &size);
    if (!converted)
        return false;
    vdx7.assign(converted, size);
    delete[] converted;
    *psx = true;
    return true;
}

std::string Trim(const std::string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
    return a == std::string::npos ? "" : s.substr(a, b - a + 1);
}

u32 Le32(const unsigned char *p)
{
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((u32)p[3] << 24);
}

/* a text file: "@header" lines, each followed by its text up to the next header; trailing blank lines dropped,
 * "@@" at a line's start is a text line starting with "@" */
std::map<std::string, std::string> ParseTextFile(const std::string &data)
{
    std::map<std::string, std::string> out;
    std::string key, body;
    bool in = false;
    auto flush = [&]() {
        if (!in)
            return;
        while (!body.empty() && (body.back() == '\n' || body.back() == '\r'))
            body.pop_back();
        out[key] = body;
    };
    size_t pos = 0;
    while (pos <= data.size()) {
        size_t eol = data.find('\n', pos);
        std::string line = data.substr(pos, eol == std::string::npos ? std::string::npos : eol - pos);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.size() >= 2 && line[0] == '@' && line[1] != '@') {
            flush();
            key = Trim(line.substr(1));
            body.clear();
            in = true;
        } else if (in) {
            if (line.size() >= 2 && line[0] == '@')
                line.erase(0, 1);
            body += line;
            body += '\n';
        }
        if (eol == std::string::npos)
            break;
        pos = eol + 1;
    }
    flush();
    return out;
}

bool SameName(const std::string &a, const std::string &b)
{
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), [](char x, char y) {
               return tolower((unsigned char)x) == tolower((unsigned char)y);
           });
}

/* the language a name means (a pack's voices, a saved choice): the first of the list with that name (case ignored; never
 * a folder name); -1 when there is none */
s32 Resolve(const std::string &name)
{
    for (size_t i = 0; i < s_langs.size(); i++)
        if (SameName(s_langs[i].name, name))
            return (s32)i;
    return -1;
}

/* the game's language number of a language: a baked-in language's own; -1 for a pack, which has none */
s32 GameLanguageOf(s32 lang)
{
    return lang >= 0 && lang < (s32)s_langs.size() && s_langs[lang].dir.empty() ? s_langs[lang].configLang : -1;
}

/* the voices a language starts with: what its language.txt names, else its own */
s32 VoicesOf(s32 lang)
{
    const Language &l = s_langs[lang];
    if (l.voices.empty())
        return lang;
    s32 v = Resolve(l.voices);
    return v >= 0 ? v : lang;
}

/* a character's byte in the game's encoding (Windows-1252, every byte kept), or -1 */
int Cp1252Byte(char32_t c)
{
    if (c < 0x80 || (c >= 0xA0 && c < 0x100))
        return (int)c;
    for (int k = 0; k < 32; k++)
        if (kCp1252High[k] == c)
            return 0x80 + k;
    return -1;
}

/* a letter's capital, for a font that draws one case only (as the game's does): Latin-1, Latin Extended-A, Greek,
 * Cyrillic; else the character itself */
char32_t Capital(char32_t c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFE && c != 0xF7))
        return c - 0x20;
    if (c >= 0x100 && c <= 0x17F && c != 0x131 && c != 0x138 && c != 0x149 && c != 0x17F)
        return (c >= 0x139 && c <= 0x148) || (c >= 0x179 && c <= 0x17E) ? (c & 1 ? c : c - 1) : (c & 1 ? c - 1 : c);
    if (c >= 0x3B1 && c <= 0x3C9 && c != 0x3C2)
        return c - 0x20;
    if (c >= 0x430 && c <= 0x44F)
        return c - 0x20;
    if (c >= 0x450 && c <= 0x45F)
        return c - 0x50;
    return c;
}

/* the cell of character c in the active pack font: the cell of its Windows-1252 byte when that cell has it (so Latin
 * text keeps the game's own bytes), else its first; then its capital's; then for a quotation mark the plain one; or -1 */
int FontCell(char32_t c)
{
    for (char32_t t : {c, Capital(c)}) {
        int b = Cp1252Byte(t);
        if (b >= 0x20 && s_cellChars[b] == t)
            return b;
        auto it = s_fontCodes.find(t);
        if (it != s_fontCodes.end())
            return it->second;
    }
    if (c == 0x2018 || c == 0x2019)
        return FontCell('\'');
    if (c == 0x201C || c == 0x201D)
        return FontCell('"');
    return -1;
}

/* text is drawn with the active pack's font: it has one, and the level's game font is the game's grid of 16 x 14
 * cells (not Intro's own lettering, which the pack's font does not replace) */
bool FontActive()
{
    return !s_fontCodes.empty() && s_levelFont;
}

/* the characters as the game's text: the cells of the active pack's font where it is drawn (FontActive), else the
 * game's encoding (Windows-1252); a character with no cell is '?' (logged). The game's markup is kept as it is in
 * either: $...$ tokens, a '%' and the character after it, line breaks. */
std::string EncodeGame(const std::string &utf8)
{
    std::string out;
    std::u32string text = DecodeUtf8(utf8);
    bool font = FontActive();
    for (size_t i = 0; i < text.size(); i++) {
        char32_t c = text[i];
        if (c == '\r')
            continue;
        if (font && (c == '$' || c == '%' || c == '\n')) {
            size_t end = c == '$' ? text.find('$', i + 1) : c == '%' ? i + 1 : i;
            if (end == std::u32string::npos || end >= text.size())
                end = text.size() - 1;
            for (; i <= end; i++) {
                int b = Cp1252Byte(text[i]);
                out += (char)(b >= 0 ? b : '?');
            }
            i = end;
            continue;
        }
        int b = font ? FontCell(c) : Cp1252Byte(c);
        if (b < 0) {
            Log("LangPack: the font has no character U+%04X: shown as '?'", (unsigned)c);
            b = font ? FontCell('?') : '?';
            if (b < 0)
                b = '?';
        }
        out += (char)b;
    }
    return out;
}

/* the game's text as UTF-8 */
std::string DecodeGame(const std::string &bytes)
{
    std::u32string out;
    for (unsigned char b : bytes)
        out += ByteToChar(b);
    return EncodeUtf8(out);
}

/* a pack's text file, read once */
const std::map<std::string, std::string> &PackText(Language &l, const std::string &file)
{
    if (!l.textRead[file]) {
        l.textRead[file] = true;
        std::string text = FindEntry(l.dir, "text"), data;
        std::string path = text.empty() ? "" : FindEntry(text, file);
        if (!path.empty() && ReadFile(path, data))
            l.text[file] = ParseTextFile(data);
    }
    return l.text[file];
}

void SaveChoice(const char *name, const std::string &value)
{
    SaveKey *key = Save_OpenKey(kCfgKey);
    if (!key)
        return;
    Save_SetValue(key, name, kRegSz, value.c_str(), (unsigned)value.size() + 1);
    Save_CloseKey(key);
}

std::string LoadChoice(const char *name)
{
    char buf[512];
    unsigned type = 0, size = sizeof(buf) - 1;
    SaveKey *key = Save_OpenKey(kCfgKey);
    if (!key)
        return "";
    int r = Save_QueryValue(key, name, &type, buf, &size);
    Save_CloseKey(key);
    if (r != SAVE_OK || type != kRegSz)
        return "";
    buf[size < sizeof(buf) ? size : sizeof(buf) - 1] = 0;
    return buf;
}

/* the number of languages the game folder's level text has: the Wheel level's .MLT's blocks (7 in the PC releases, 6
 * in the PAL PlayStation release's, which has no Brazilian Portuguese). The one level, not the first one found: the
 * PAL PlayStation release's Fend has a seventh block that its other levels do not. 7 when it is not found. */
u16 TextBlocks()
{
    std::string wheel = FindEntry(FindEntry(s_gameDir, "Levels"), "Wheel");
    std::error_code ec;
    if (!wheel.empty())
        for (fs::directory_iterator it(U8Path(wheel), ec), end; !ec && it != end; it.increment(ec)) {
            std::string ext = Lower(PathU8(it->path().extension()));
            std::ifstream f(it->path(), std::ios::binary);
            unsigned char head[12];
            if (ext == ".mlt" && f.read((char *)head, sizeof(head)))
                return (u16)(head[8] | (head[9] << 8));
        }
    return 7;
}

/* the game's own languages, its table (scene_wheel.cpp), as the language list's baked-in languages: each one whose
 * level text the game folder's .MLT files have, and whose name no pack has (a pack of its name is that language) */
void AddBakedIn(u16 blocks)
{
    for (u32 k = 0; k < g_sceneWheelLanguageCount; k++) {
        Language l;
        l.name = DecodeGame(g_sceneWheelLanguageNames[k]);
        l.id = l.name;
        l.configLang = g_sceneWheelLanguageIds[k];
        u16 block = (u16)l.configLang;
        Mlt_MapLanguageToBlock(&block);
        l.mltBlock = block;
        l.voiceFolder = StreamPlayer_VoiceFolder(l.configLang);
        if (!l.voiceFolder.empty() && l.voiceFolder.back() == '/')
            l.voiceFolder.pop_back();
        l.hasVoices = !FindEntry(s_gameDir + "/Voices", l.voiceFolder).empty();
        l.wheelSlot = (s32)k;
        if (Resolve(l.name) >= 0) {
            Log("LangPack: the game's %s is the pack of its name", l.name.c_str());
            continue;
        }
        if (block >= blocks) {
            Log("LangPack: %s: the game's level text has no %s", s_gameDir.c_str(), l.name.c_str());
            continue;
        }
        s_langs.push_back(l);
    }
}

} /* namespace */

void LangPack_Init(const char *gameDir)
{
    s_langs.clear();
    s_listings.clear();
    s_gameDir = gameDir ? gameDir : ".";
    /* the list, built here: the language packs first, by folder name (a pack of a name already listed is not); then
     * the game's own languages, but those a pack has the name of (langpack.h) */
    std::string languages = FindEntry(s_gameDir, "Languages");
    std::vector<std::string> folders;
    std::error_code ec;
    if (!languages.empty())
        for (fs::directory_iterator it(U8Path(languages), ec), end; !ec && it != end; it.increment(ec))
            if (it->is_directory(ec))
                folders.push_back(PathU8(it->path().filename()));
    std::sort(folders.begin(), folders.end());
    for (const std::string &folder : folders) {
        Language l;
        l.id = folder;
        l.dir = languages + "/" + folder;
        std::string desc = FindEntry(l.dir, "language.txt"), data;
        if (desc.empty() || !ReadFile(desc, data)) {
            Log("LangPack: %s has no language.txt: not a language pack", l.dir.c_str());
            continue;
        }
        std::istringstream lines(data);
        std::string line;
        while (std::getline(lines, line)) {
            size_t eq = line.find('=');
            if (Trim(line).empty() || Trim(line)[0] == '#' || eq == std::string::npos)
                continue;
            std::string k = Trim(line.substr(0, eq)), v = Trim(line.substr(eq + 1));
            if (k == "name")
                l.name = v;
            else if (k == "voices")
                l.voices = v;
        }
        if (l.name.empty())
            l.name = folder;
        l.hasVoices = !FindEntry(l.dir, "voices").empty();
        s32 same = Resolve(l.name);
        if (same >= 0) {
            Log("LangPack: %s has the name of %s, listed before it: not listed", l.dir.c_str(), s_langs[same].dir.c_str());
            continue;
        }
        s_langs.push_back(l);
        Log("LangPack: %s (%s%s)", l.name.c_str(), l.dir.c_str(), l.hasVoices ? ", with voices" : "");
    }
    AddBakedIn(TextBlocks());
    for (Language &l : s_langs)
        l.shown = EncodeGame(l.name);

    /* the saved choices, by name */
    std::string saved = LoadChoice("TextLanguage");
    s32 text = saved.empty() ? -1 : Resolve(saved);
    s_chosenText = s_activeText = text >= 0 ? text : 0;
    saved = LoadChoice("VoiceLanguage");
    s32 v = saved.empty() ? -1 : Resolve(saved);
    s_chosenVoices = v >= 0 && s_langs[v].hasVoices ? v : -1;
}

s32 LangPack_Count()
{
    return (s32)s_langs.size();
}

const char *LangPack_Name(s32 lang)
{
    return lang >= 0 && lang < (s32)s_langs.size() ? s_langs[lang].shown.c_str() : "";
}

s32 LangPack_HasVoices(s32 lang)
{
    return lang >= 0 && lang < (s32)s_langs.size() && s_langs[lang].hasVoices;
}

s32 LangPack_ChosenText()
{
    return s_chosenText;
}

s32 LangPack_ActiveText()
{
    return s_activeText;
}

void LangPack_ChooseText(s32 lang)
{
    if (lang < 0 || lang >= (s32)s_langs.size())
        return;
    s_chosenText = lang;
    SaveChoice("TextLanguage", s_langs[lang].name);
}

void LangPack_ApplyChosen()
{
    s_encodedCache.clear();
    s_activeText = s_chosenText;
    /* its font: a pack's own font/, when it has one */
    s_fontPng.clear();
    s_fontCodes.clear();
    memset(s_cellChars, 0, sizeof(s_cellChars));
    if (s_langs.empty())
        return;
    const Language &l = s_langs[s_activeText];
    std::string font = l.dir.empty() ? "" : FindEntry(l.dir, "font");
    std::string png = font.empty() ? "" : FindEntry(font, "font.png");
    std::string chars = font.empty() ? "" : FindEntry(font, "characters.txt"), data;
    if (!png.empty() && !chars.empty() && ReadFile(chars, data)) {
        unsigned code = 0x20;
        for (char32_t c : DecodeUtf8(data)) {
            if (c == '\n' || c == '\r')
                continue;
            if (code > 0xFF)
                break;
            if (!s_fontCodes.count(c)) /* a space: its own cell, the first; other spaces are cells left out */
                s_fontCodes[c] = (unsigned char)code;
            if (c != ' ' || s_fontCodes[c] == code)
                s_cellChars[code] = c;
            code++;
        }
        s_fontPng = png;
        Log("LangPack: font %s (%u cells)", png.c_str(), code - 0x20);
    }
    for (Language &l : s_langs) /* the names, in the font they are shown in */
        l.shown = EncodeGame(l.name);
}

s32 LangPack_FontActive()
{
    return FontActive();
}

u8 LangPack_GlyphVariant(u8 cell, u8 variant)
{
    return !FontActive() || s_cellChars[cell] == s_cellChars[variant] ? variant : cell;
}

const char *LangPack_FontPicture()
{
    return s_fontPng.c_str();
}

namespace {

/* a picture's texture: its file, and where the picture is in it (w 0: it is the whole of it) */
struct TexturePlace {
    std::string path;
    u32 x = 0, y = 0, w = 0, h = 0;
};

/* the game's signs drawn in several pictures, which a pack has as one texture each (textures/<name>.png): its name and
 * size, and its pieces, each by its fingerprint and its place in the texture (fingerprint 0: no more) */
struct JoinedPiece {
    u64 fingerprint;
    u16 x, y;
};

struct JoinedSign {
    const char *name;
    u16 w, h;
    JoinedPiece pieces[6];
};

const JoinedSign kJoinedSigns[] = {
    {"studio-logo", 192, 126,
     {{0xfb434bc17bda2106ull, 0, 0}, {0x47ad412be57a8e2eull, 64, 0}, {0x8182437fb64a95e3ull, 128, 0},
      {0x7b67c7ca99461829ull, 0, 63}, {0x4122be0d122e94bbull, 64, 63}, {0x6bab2a11e3ef7526ull, 128, 63}}},
    {"acme-training-zone", 256, 64,
     {{0xd21bf50e76464f0full, 0, 0}, {0x8126701ac437a638ull, 64, 0}, {0xad98a6a7a75d6cdcull, 128, 0},
      {0xab8299a671473bb1ull, 192, 0}}},
    {"acme-training-zone-hub", 128, 31,
     {{0xc8a16adf3743e22dull, 0, 0}, {0x8dc0cfd1fa6fd6ccull, 32, 0}, {0x4c6ba3b71c245a65ull, 64, 0},
      {0x82e6a71b550760feull, 96, 0}}},
    {"acme-training-zone-playstation", 128, 32,
     {{0x559e0bfac030aba3ull, 0, 0}, {0x609c3ae193021dd4ull, 32, 0}, {0x7c2f894302bc49f7ull, 64, 0},
      {0x63f88ce9742a0d5full, 96, 0}}},
    {"acme-snowboard", 128, 64, {{0x8a099f62d4968b5full, 0, 0}, {0xbb24820d2035524full, 64, 0}}},
    {"acme-snowboard-playstation", 64, 32, {{0xdb2ab369d388264dull, 0, 0}, {0xd36b8ca68bfb2ae2ull, 32, 0}}},
    {"acme-studio", 128, 63, {{0xcac7fb685bb7fa80ull, 0, 0}, {0x218b579053e5d67full, 64, 0}}},
    {"base", 128, 63, {{0x4bba7094842cd150ull, 0, 0}, {0x794aa48636b97b53ull, 64, 0}}},
    {"game-over-playstation", 112, 64, {{0xa6ad69dd630e283eull, 0, 0}, {0x14fc70a31a491814ull, 56, 0}}},
};

/* the pieces of the signs the pack has (kJoinedSigns), into places where they have none yet */
void JoinedTextures(const std::string &dir, std::map<u64, TexturePlace> &places)
{
    for (const JoinedSign &sign : kJoinedSigns) {
        std::string file = FindEntry(dir, std::string(sign.name) + ".png");
        if (file.empty())
            continue;
        for (const JoinedPiece &piece : sign.pieces)
            if (piece.fingerprint && !places.count(piece.fingerprint))
                places[piece.fingerprint] = {file, piece.x, piece.y, sign.w, sign.h};
    }
}

} /* namespace */

void LangPack_Textures(void (*found)(u64 fingerprint, const char *path, u32 x, u32 y, u32 w, u32 h, void *user),
                       void *user)
{
    if (s_langs.empty())
        return;
    std::map<u64, TexturePlace> places; /* the active pack's own */
    const Language &l = s_langs[s_activeText];
    std::string dir = l.dir.empty() ? "" : FindEntry(l.dir, "textures");
    if (!dir.empty()) {
        std::error_code ec;
        for (fs::directory_iterator it(U8Path(dir), ec), end; !ec && it != end; it.increment(ec)) {
            std::string name = Lower(PathU8(it->path().filename()));
            unsigned long long fingerprint = 0;
            int used = 0;
            if (name.size() == 20 && name.compare(16, 4, ".png") == 0 &&
                name.find_first_not_of("0123456789abcdef") == 16 &&
                sscanf(name.c_str(), "%16llx%n", &fingerprint, &used) == 1 && used == 16 && !places.count(fingerprint))
                places[fingerprint] = {PathU8(it->path())};
        }
        JoinedTextures(dir, places);
    }
    for (const auto &kv : places)
        found(kv.first, kv.second.path.c_str(), kv.second.x, kv.second.y, kv.second.w, kv.second.h, user);
}

s32 LangPack_ChosenVoices()
{
    return s_chosenVoices;
}

void LangPack_ChooseVoices(s32 lang)
{
    s_chosenVoices = LangPack_HasVoices(lang) ? lang : -1;
    SaveChoice("VoiceLanguage", s_chosenVoices >= 0 ? s_langs[s_chosenVoices].name : "");
}

s32 LangPack_GameLanguage()
{
    return s_langs.empty() ? (s32)GAME_LANG_ENGLISH : GameLanguageOf(s_activeText);
}

namespace {

/* a level's text in language lang, as UTF-8: the .MLT's block of its base (the game's encoding, decoded), the packs'
 * lines over it */
bool LevelLists(const char *mltPath, s32 lang, std::vector<std::vector<std::string>> &lists)
{
    std::string data;
    if (!ReadFile(ResolveCase(mltPath), data))
        return false;
    /* {u32 crc; header; u32 crc; payload}, each CRC over what follows it, as the game's reader (File_ReadChecked) checks */
    const unsigned char *d = (const unsigned char *)data.data();
    if (data.size() < 12 || Crc32(d + 4, 8) != Le32(d) ||
        (data.size() > 16 && Crc32(d + 16, (int)(data.size() - 16)) != Le32(d + 12))) {
        Log("LangPack: %s is damaged (its checksums do not match): not loaded", mltPath);
        return false;
    }
    if (data.size() <= 16) { /* no payload: an empty bank, as the game's reader leaves it */
        lists.clear();
        return true;
    }
    u16 blockCount, listCount;
    memcpy(&blockCount, data.data() + 8, 2);
    memcpy(&listCount, data.data() + 10, 2);
    /* a baked-in language's: its own block; a pack's: its own lines only (its text file for the level) */
    bool pack = !s_langs.empty() && !s_langs[lang].dir.empty();
    s32 block = s_langs.empty() ? MLT_BLOCK_ENGLISH : pack ? -1 : s_langs[lang].mltBlock;
    if (block >= blockCount) {
        Log("LangPack: %s has no text block %d", mltPath, (int)block);
        block = -1;
    }

    /* the game's block's lists */
    lists.assign(listCount, {});
    size_t at = 16;
    for (s32 b = 0; b <= block; b++)
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

    /* a pack's lines */
    std::string level = PathU8(U8Path(mltPath).stem());
    if (pack) {
        Language &l = s_langs[lang];
        for (const auto &kv : PackText(l, level + ".txt")) {
            unsigned list = 0, line = 0;
            if (sscanf(kv.first.c_str(), "%u.%u", &list, &line) != 2 || list > 0xFFFF || line > 0xFF) {
                Log("LangPack: %s/text/%s.txt: \"@%s\" is not @list.line", l.dir.c_str(), level.c_str(),
                             kv.first.c_str());
                continue;
            }
            if (list >= lists.size())
                lists.resize(list + 1);
            if (line >= lists[list].size())
                lists[list].resize(line + 1);
            lists[list][line] = kv.second;
        }
    }
    return true;
}

/* the game font's picture in a page file (VDX7): its number in the file's picture list and its size; false when it
 * has none */
bool FontRectIn(const std::string &dav, u32 *picture, u32 *w, u32 *h)
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
        if ((hd & 0xFFFF) == 0x24 && count) { /* DAV_IDI_IGLTYPO_, the game font's list: its first entry */
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
            *w = u32At(ra + 10 * rect + 2, 2);
            *h = u32At(ra + 10 * rect + 6, 2);
            return true;
        }
        at += 4 + 4 * count;
    }
    return false;
}

/* the same of the page file davPath, a PC or a PlayStation release's */
bool FontRect(const std::string &davPath, u32 *picture, u32 *w, u32 *h)
{
    std::string dav;
    bool psx;
    return ReadPageFile(davPath, dav, &psx) && FontRectIn(dav, picture, w, h);
}

/* whether the game font of the level whose page file is davPath is the game's grid, 16 x 14 cells of 16 (256 x 224):
 * every level's but Intro's, which has lettering of its own; true when the file cannot be read */
bool LevelFontIsGrid(const std::string &davPath)
{
    u32 picture, w, h;
    return !FontRect(davPath, &picture, &w, &h) || (w == 256 && h == 224);
}

/* text handed out to the game, kept alive until the next level */
const char *Kept(const std::string &key, const std::string &bytes)
{
    return s_encodedCache.emplace(key, bytes).first->second.c_str();
}

} /* namespace */

s32 LangPack_LoadLevelText(const char *mltPath, StringBank *bank)
{
    bank->listCount = 0;
    bank->lists = 0;

    /* the level's game font: the pack's is drawn in it unless it is Intro's own lettering */
    std::string dav = mltPath;
    if (dav.size() > 4)
        dav.replace(dav.size() - 4, 4, ".DAV");
    bool grid = LevelFontIsGrid(dav);
    if (!grid && !s_fontCodes.empty())
        Log("LangPack: %s has lettering of its own for its font: the pack's font is not used there", dav.c_str());
    if (grid != s_levelFont) {
        s_levelFont = grid;
        s_encodedCache.clear();
        for (Language &l : s_langs)
            l.shown = EncodeGame(l.name);
    }

    std::vector<std::vector<std::string>> lists;
    if (!LevelLists(mltPath, s_activeText, lists))
        return -1;
    for (auto &list : lists)
        for (auto &line : list)
            line = EncodeGame(line);

    /* the game's layout: one buffer of {u8 count; strings}, lists[i] pointing into it, lists[0] its start */
    size_t total = 0;
    for (const auto &list : lists) {
        total += 1;
        for (const auto &s : list)
            total += s.size() + 1;
    }
    bank->listCount = (u16)lists.size();
    if (lists.empty())
        return 0;
    char *buf = (char *)malloc(total);
    char **table = (char **)malloc(lists.size() * sizeof(char *));
    char *p = buf;
    for (size_t i = 0; i < lists.size(); i++) {
        table[i] = p;
        *p++ = (char)lists[i].size();
        for (const auto &s : lists[i]) {
            memcpy(p, s.c_str(), s.size() + 1);
            p += s.size() + 1;
        }
    }
    bank->lists = table;
    return 0;
}

namespace {

/* a menu string (GREET_txt.BSM group and index) of language `of`, UTF-8: a pack's, from its Menus.txt ("" when it has
 * none of it); false for a baked-in language (the game's own, GREET_txt.BSM, is used). *from: the pack's id, and the
 * key it is under. */
bool MenuText(s32 of, u16 group, u16 index, std::string &utf8, std::string *from)
{
    if (s_langs.empty() || s_langs[of].dir.empty())
        return false;
    Language &l = s_langs[of];
    char key[32];
    const auto &t = PackText(l, "Menus.txt");
    for (const char *fmt : {"0x%X.0x%X", "%u.%u", "0x%x.0x%x"}) {
        snprintf(key, sizeof(key), fmt, (unsigned)group, (unsigned)index);
        auto it = t.find(key);
        if (it != t.end()) {
            utf8 = it->second;
            if (from)
                *from = l.id + "|M|" + key;
            return true;
        }
    }
    utf8.clear();
    snprintf(key, sizeof(key), "0x%X.0x%X", (unsigned)group, (unsigned)index);
    if (from)
        *from = l.id + "|M|" + key;
    return true;
}

/* a save-game message by MCardString id of language `of`: a pack's, from its Saves.txt (UTF-8, *fromPack true, *from
 * its key; "" when it has none of it); else the baked-in language's own (its bytes) */
std::string SaveText(s32 of, u32 id, bool *fromPack, std::string *from)
{
    id &= 0xFF;
    *fromPack = false;
    if (!s_langs.empty() && !s_langs[of].dir.empty()) {
        Language &l = s_langs[of];
        char key[16];
        snprintf(key, sizeof(key), "%u", (unsigned)id);
        const auto &t = PackText(l, "Saves.txt");
        auto it = t.find(key);
        *fromPack = true;
        if (from)
            *from = l.id + "|S|" + key;
        return it != t.end() ? it->second : "";
    }
    if (id >= LANGPACK_SAVE_STRINGS)
        return "";
    return g_mcardStrings[(s_langs.empty() ? 0 : s_langs[of].configLang) & 3][id];
}

} /* namespace */

const char *LangPack_MenuString(u16 group, u16 index)
{
    std::string utf8, key;
    if (!MenuText(s_activeText, group, index, utf8, &key))
        return 0;
    auto c = s_encodedCache.find(key);
    return c != s_encodedCache.end() ? c->second.c_str() : Kept(key, EncodeGame(utf8));
}

const char *LangPack_GameText(const char *bytes)
{
    if (!bytes || !FontActive())
        return bytes;
    std::string key = std::string("|G|") + bytes;
    auto c = s_encodedCache.find(key);
    return c != s_encodedCache.end() ? c->second.c_str() : Kept(key, EncodeGame(DecodeGame(bytes)));
}

const char *LangPack_SaveString(u32 id)
{
    /* the chosen text language's, at once, as the original reads its language when the message is shown (the wheel's
     * memory-card screen follows the language just chosen, before the next level load makes it the active one); drawn
     * in the loaded level's font */
    bool fromPack;
    std::string key;
    std::string text = SaveText(s_chosenText, id, &fromPack, &key);
    if (fromPack) {
        auto c = s_encodedCache.find(key);
        return c != s_encodedCache.end() ? c->second.c_str() : Kept(key, EncodeGame(text));
    }
    /* the baked-in language's own */
    id &= 0xFF;
    if (id >= LANGPACK_SAVE_STRINGS)
        return "";
    return LangPack_GameText(g_mcardStrings[(s_langs.empty() ? 0 : s_langs[s_chosenText].configLang) & 3][id]);
}

void LangPack_VoicePath(const char *clip, char *out, u32 size)
{
    s32 voices = s_chosenVoices;
    if (voices < 0 && !s_langs.empty())
        voices = VoicesOf(s_chosenText);
    if (voices < 0) { /* no languages: the game's own folder for English */
        snprintf(out, size, "%s/Voices/%s%s", s_gameDir.c_str(), StreamPlayer_VoiceFolder(GAME_LANG_ENGLISH), clip);
        return;
    }
    /* the voice language's own clip: a baked-in language's folder under Voices/, a pack's voices/; a pack with no voices
     * of its own has the game's English voices */
    const Language &l = s_langs[voices];
    s32 number = l.dir.empty() ? l.configLang : (s32)GAME_LANG_ENGLISH;
    std::string folder = l.dir.empty() ? "" : FindEntryListed(l.dir, "voices");
    bool own = !folder.empty();
    if (!own) {
        std::string name = StreamPlayer_VoiceFolder(number);
        if (!name.empty() && name.back() == '/')
            name.pop_back();
        folder = FindEntryListed(s_gameDir + "/Voices", name);
    }
    std::string file = folder.empty() ? "" : FindEntryListed(folder, clip);
    if (!file.empty())
        snprintf(out, size, "%s", file.c_str());
    else if (!own) /* where the game itself plays it from */
        snprintf(out, size, "%s/Voices/%s%s", s_gameDir.c_str(), StreamPlayer_VoiceFolder(number), clip);
    else
        snprintf(out, size, "%s", "");
}

void LangPack_SetLog(void (*log)(const char *text))
{
    s_log = log ? log : DefaultLog;
}

const char *LangPack_WheelPicture(s32 lang)
{
    static std::string path;
    if (lang < 0 || lang >= (s32)s_langs.size() || s_langs[lang].dir.empty())
        return "";
    path = FindEntry(s_langs[lang].dir, "wheel.png");
    return path.c_str();
}

s32 LangPack_GameLanguageOf(s32 lang)
{
    return GameLanguageOf(lang);
}

s32 LangPack_WheelSlot(s32 lang)
{
    return lang >= 0 && lang < (s32)s_langs.size() ? s_langs[lang].wheelSlot : -1;
}
