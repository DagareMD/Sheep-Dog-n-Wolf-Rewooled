/*
 * The .MLT string bank: the localised text of a level.
 *
 * An .MLT file is {u32 crc; MltHeader hdr; u32 crc; payload}, read with two File_ReadChecked calls. The header is {char
 * version[4] ('v1.2' in every shipped file); u16 blockCount (7); u16 listCount (0xa9 in Lvl-14.MLT)}. The payload is
 * blockCount language blocks, each of listCount lists, each list {u8 count; count NUL-terminated strings}. Block 0 is
 * French in the shipped files; Mlt_MapLanguageToBlock maps the config language (Progress.language: 0 English, 1
 * Spanish, 2 Italian, 3 Portuguese) to blocks 1, 2, 3, 6. Load_MLT_ParseBank skips to the chosen block, copies just
 * that block into one malloc'd buffer and returns a table of pointers to its lists; the bank lives at g_pDav+0x14
 * (StringBank).
 */

#include "sdw_types.h"
#include "sdw_enums.h"
#include "sdw_classes.h"
#define SDW_INLINE_PROGRESS_GETLANGUAGE 1
#include "progress_inlines.h"
#undef SDW_INLINE_PROGRESS_GETLANGUAGE

#include "../sdk/crt.h"

/* the JPEG decoders are in jpeg_decode.cpp (they need libjpeg's own header) */

/* ======================================================================================================================
 * the .MLT string bank.
 */

/* ---- the game's own helpers ---- */
u16 Str_Length(const char *s);
#include "file.h"
#include "progress.h"
#include "langpack.h"
void Debug_Printf(const char *fmt, ...); /* an empty stub in the retail build */

void Mlt_MapLanguageToBlock(u16 *lang);

/* free() a block the caller owns and forget it */
#define MLT_FREE(p) \
    if (p) {        \
        free(p);    \
        (p) = 0;    \
    }

/* skips `block` language blocks of `listCount` lists each, copies the next block into one malloc'd buffer and
 * returns a malloc'd table of listCount pointers to its lists (NULL when listCount is 0). No bounds check: a block index
 * past the file's last block walks off the payload. */
char **Load_MLT_ParseBank(char *data, u16 listCount, u16 block)
{
    char *src;
    u32 k;
    u32 j;
    u32 i;
    char *dst;
    u32 total;
    char **table;
    u8 count;

    total = 0;
    if (listCount == 0)
        return 0;
    table = (char **)malloc(listCount * sizeof(char *));
    Mlt_MapLanguageToBlock(&block);
    for (i = 0; i < block; i++) {
        for (j = 0; j < listCount; j++) {
            count = *data++;
            for (k = 0; k < count; k++)
                data += Str_Length(data) + 1;
        }
    }
    src = data;
    for (i = 0; i < listCount; i++) {
        count = *src++;
        total++;
        for (j = 0; j < count; j++) {
            total += Str_Length(src) + 1;
            src += Str_Length(src) + 1;
        }
    }
    dst = (char *)malloc(total);
    memcpy(dst, data, total);
    data = dst;
    for (i = 0; i < listCount; i++) {
        table[i] = data;
        count = *data++;
        for (j = 0; j < count; j++)
            data += Str_Length(data) + 1;
    }
    return table;
}

/* frees a table made by Load_MLT_ParseBank: lists[0] is the start of the one string buffer. */
void StringBank_FreeLists(char **lists, u16 count)
{
    if (lists) {
        if (count)
            MLT_FREE(lists[0]);
        MLT_FREE(lists);
    }
}

/* loads the .MLT at path into bank for the current config language. 0, or -1 on a missing file or a failed
 * (CRC-checked) read. A file with no payload leaves the bank empty. Rewooled: the bank comes from the language packs
 * (langpack.h), which read this file's block of the text language's base and lay the packs' lines over it; the
 * original reader below is kept as it was. */
s32 Load_MLT(char *path, StringBank *bank)
{
    return LangPack_LoadLevelText(path, bank);
}

s32 Load_MLT_Original(char *path, StringBank *bank)
{
    FileHandle file;
    u32 maxSize;
    MltHeader hdr;
    char *data;

    data = 0;
    if (File_Open(path, &file) <= 0) {
        Debug_Printf("Load_MLT: File not found\n");
        return -1;
    }
    maxSize = 40000; /* written, never read */
    if (File_ReadChecked(&file, &hdr, 8) < 0) {
        Debug_Printf("Load_MLT: File Read Error\n");
        File_Close(&file);
        return -1;
    }
    if (file.remaining > 4) {
        data = (char *)malloc(file.remaining - 4);
        if (File_ReadChecked(&file, data, file.remaining - 4) < 0) {
            Debug_Printf("Load_MLT: Loading remainder\n");
            goto failed;
        }
        if (g_pProgress->GetLanguage() >= hdr.blockCount)
            Debug_Printf("\nLoad_MLT: (WARNING) Current language not supported\n");
        bank->listCount = hdr.listCount;
        bank->lists = Load_MLT_ParseBank(data, bank->listCount, g_pProgress->GetLanguage());
        MLT_FREE(data);
    } else {
        bank->listCount = 0;
        bank->lists = 0;
    }
    File_Close(&file);
    return 0;
failed:
    MLT_FREE(data);
    File_Close(&file);
    return -1;
}

/* frees the bank and zeroes it. Always 0. */
s32 Load_FreeMLT(StringBank *bank)
{
    if (bank) {
        StringBank_FreeLists(bank->lists, bank->listCount);
        bank->listCount = 0;
        bank->lists = 0;
    }
    return 0;
}

/* config language (0 English, 1 Spanish, 2 Italian, 3 Portuguese) -> .MLT block (1, 2, 3, 6), in place. */
void Mlt_MapLanguageToBlock(u16 *lang)
{
    switch (*lang) {
        case GAME_LANG_ENGLISH:
            *lang = MLT_BLOCK_ENGLISH;
            break;
        case GAME_LANG_SPANISH:
            *lang = MLT_BLOCK_SPANISH;
            break;
        case GAME_LANG_ITALIAN:
            *lang = MLT_BLOCK_ITALIAN;
            break;
        case GAME_LANG_BRAZILIAN:
            *lang = MLT_BLOCK_BRAZILIAN;
            break;
    }
}
