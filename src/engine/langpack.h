/* Language packs (docs/language-packs.md): the game's own seven languages, read from its files, and the packs in the
 * game folder's Languages/, each a text language, and those with voices a voice language too.
 *
 * The text language is chosen on the language wheel and in Rewooled Settings; it takes effect when a level loads
 * (LangPack_ApplyChosen, from the level loader), because the game reads a level's text, font and textures then and
 * its objects keep the text they were given. The voice language takes effect at once: every voice clip's path is
 * made when it is played. Both are kept in the save store (CfgGame: TextLanguage, VoiceLanguage). */
#ifndef SDW_ENGINE_LANGPACK_H
#define SDW_ENGINE_LANGPACK_H

#include "sdw_types.h"

struct StringBank;

/* where the module's notes go (the game: its log); stderr until set */
void LangPack_SetLog(void (*log)(const char *text));

/* reads the packs of <gameDir>/Languages and the saved choices; once, at start-up, before the first level */
void LangPack_Init(const char *gameDir);

/* the languages, the list LangPack_Init builds (the wheel and Settings show it): the language packs, by folder name,
 * then the game's own languages (its table, scene_wheel.cpp), those no pack has the name of. A name is one language: a
 * pack of a game language's name is that language. A pack is a language on its own: everything of it is its own. */
s32 LangPack_Count();
const char *LangPack_Name(s32 lang);     /* the name shown, as the game's text (its encoding) */
s32 LangPack_HasVoices(s32 lang);        /* a voice language: its own voices are there */

/* the text language chosen (the next level's) and the one the loaded level uses */
s32 LangPack_ChosenText();
s32 LangPack_ActiveText();
void LangPack_ChooseText(s32 lang);      /* saved; takes effect at the next level load */
void LangPack_ApplyChosen();             /* the level loader: the chosen text language becomes the active one */

/* the voice language: -1 for "the text language's own" (the pack's `voices`), else a language with voices */
s32 LangPack_ChosenVoices();
void LangPack_ChooseVoices(s32 lang);    /* saved; takes effect at once */

/* the game's language number (Progress.language) of the active text language, for the game code that reads it (the
 * level loader sets it): a baked-in language's own; -1 for a pack, which has none (the number is left as it is) */
s32 LangPack_GameLanguage();

/* the level's string bank in the active text language: a baked-in language's .MLT block, a pack's own lines (its text
 * file for the level). Same contract as Load_MLT (0, or -1 when the .MLT cannot be read). */
s32 LangPack_LoadLevelText(const char *mltPath, StringBank *bank);
/* a menu string (GREET_txt.BSM group and index) of the active pack ("" when it has none of it), or 0 for a baked-in
 * language (the game's own) */
const char *LangPack_MenuString(u16 group, u16 index);
/* a save-game message by MCardString id in the chosen text language (at once, as the original reads it) */
const char *LangPack_SaveString(u32 id);
/* the game's own text (its bytes) as the loaded level draws it: as it is, or in the active pack's font */
const char *LangPack_GameText(const char *bytes);
/* text is drawn with the active pack's font (langpack.h: font/), so its cells are the pack's: the glyph shuffle and
 * blink (text.cpp) take a variant cell only where it draws the same character */
s32 LangPack_FontActive();
/* `variant`, the game's variant of cell `cell`, when the active pack's font draws the same at both; else cell */
u8 LangPack_GlyphVariant(u8 cell, u8 variant);

/* the file of voice clip `clip` (a file name as MusicVoice.BSV gives it) in the voice language, into out */
void LangPack_VoicePath(const char *clip, char *out, u32 size);

/* the wheel model's picture a language shows: a game language's place in the game's table, or -1 (a pack) */
s32 LangPack_WheelSlot(s32 lang);

/* the language wheel (scene_wheel.cpp), a stop per language: a pack's wheel.png, "" when it has none (or is a game
 * language); and the game's language number of a language (a baked-in language's own; -1 for a pack) */
const char *LangPack_WheelPicture(s32 lang);
s32 LangPack_GameLanguageOf(s32 lang);
/* the active text language's font.png ("" for the game's font) */
const char *LangPack_FontPicture();
/* the active text language's packs' textures, each for the game's picture it replaces wherever the page files have it,
 * known by its fingerprint (polybatcher.cpp: PolyBatcher_Fingerprint): textures/<fingerprint>.png, the picture's own
 * (16 hex digits; w and h 0: the picture's size), and the pieces of the game's signs drawn in several pictures,
 * textures/<sign's name>.png (langpack.cpp: kJoinedSigns), each picture at x, y of a texture of w x h. The nearest pack's of each fingerprint, each given to found. */
void LangPack_Textures(void (*found)(u64 fingerprint, const char *path, u32 x, u32 y, u32 w, u32 h, void *user),
                       void *user);

#endif
