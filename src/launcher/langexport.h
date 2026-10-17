/* The launcher's Language Export page's work (popup_langexport.cpp): the languages of the game's files (a game folder,
 * or a disc image read in place: xa_extract.h's DiscFiles), and writing one out as a language pack
 * (docs/language-packs.md). The game's own code is not part of it. */
#ifndef SDW_LAUNCHER_LANGEXPORT_H
#define SDW_LAUNCHER_LANGEXPORT_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int16_t s16;
typedef int32_t s32;

/* where the exporter's notes go (stderr until set), and one written, printf-style */
void Export_SetLog(void (*log)(const char *text));
void Export_Logf(const char *fmt, ...);

/* the game's files from `path`, a game folder or a disc image (.iso, .bin) read in place, and its languages
 * (Export_Init's). False with the reason in error. */
bool Export_OpenSource(const char *path, char *error, u32 errorSize);

/* reads the languages of the game's files at gameDir (Export_OpenSource's): one for every text block its level text
 * (.MLT) has, in the blocks' order. The `lang` below is one's place (Export_Count's). */
void Export_Init(const char *gameDir);
s32 Export_Count();
const char *Export_NameUtf8(s32 lang); /* the name shown */

/* language lang's text written into a new pack folder at outDir (which must not be there yet): text/ with every
 * level's text, Menus.txt and Saves.txt, all of them, ready to translate; withVoices: also its voices, when the game's
 * files have them, copied into voices/. No language.txt (Export_Language writes it). False with the reason in error. */
bool Export_WritePack(s32 lang, const char *outDir, const char *name, bool withVoices, char *error, u32 errorSize);
/* the Language Export page's Export: language lang written out as a pack folder at outDir (which must not be there
 * yet): Export_WritePack's text (and voices, withVoices), wheel.png, font/, and with withTextures textures/ with the
 * game's lettered pictures (langexport_lettered.h), each once, as the game's files have them; language.txt last (shown
 * as `name`, its voices its own or the language written from), so the game lists the folder only once it is whole. On
 * a failure the folder it made is removed. False with the reason in error. */
bool Export_Language(s32 lang, const char *outDir, const char *name, bool withVoices, bool withTextures, char *error,
                     u32 errorSize);
/* a level folder's name as the PC release has it (a pack's text/ uses it): a PlayStation disc's LEV00 is Lvl-00,
 * INTRO is Intro...; a PC release's as it is */
const char *Export_LevelName(const char *folder);

#endif
