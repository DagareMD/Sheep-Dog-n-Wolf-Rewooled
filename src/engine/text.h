#ifndef SDW_ENGINE_TEXT_H
#define SDW_ENGINE_TEXT_H

/* The functions and globals text.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

struct Font;
struct PadFrame;

extern Font g_fonts[3];
extern Font *g_pCurFont;
extern char g_textScratchBuffer[0x3f0];
void Font_CloneResized(u8 srcFontId, u8 dstFontId, u8 advancePlus1, u8 heightMinus1);
s8 Font_LoadFromRes(u8 fontId, u16 resType, u8 cellGeometry);
u32 Font_SetCellSize(u8, u8);
void Hud_EndBox_stub();
s32 ScrollText_Run(u8 mode, char *text, s32 allowInput); /* (src/engine/text.cpp) */
u16 Str_ParseU16(const char *s);
void Text_ApplyWindow(u32 *layer);
void Text_CenterVertically(s32 lines);
/* HudElement centre reader; reads the window when its first coordinate is mapped. */
void Text_ElementCentre(const void *data, float &x, float &y);
void Text_Disable();
void Text_DrawNoWrap(const char *text, u8 align);
void Text_DrawString(u8 c, s32 x, s32 y, float z, u32 rgb);
void Text_EmitLineThunk(const char *text, s32 n, u8 align);
s32 Text_ExpandButtonToken(const char *tok, char *out, u16 *len);
s32 Text_ExpandColorToken(const char *tok, char *out, u16 *len);
s32 Text_ExpandMemCardToken(const char *tok, char *out, u16 *len);
s32 Text_ExpandNameToken(const char *tok, char *out, u16 *len);
s32 Text_FindNextPageMark(char **start, char **cur);
char *Text_FindPageMark(char **p);
s32 Text_FindPrevPageMark(char **start, char **cur);
s32 Text_GetScrollInput(PadFrame *frame);
s32 Text_LineIsBlank(char **p);
u16 Text_MaxLineLength(const char *s);
u16 Text_MeasureLine(const char *s);
void Text_NewLine(s32 lines);
char *Text_PageStep(s32 mode, char **text, char **next, char **prev);
void Text_PrintFmt(const char *fmt, ...);
void Text_Printf(u8 align, const char *fmt, ...);
void Text_PrintfStyled(u8 align, s32 blink, const char *fmt, ...);
void Text_PutColorCode(char *out, u32 rgb);
void Text_ResetMeasure();
u8 Text_ResetWindow();
void Text_ScrambleGlyphs(char *s);
void Text_SetColor(u32 rgb);
void Text_SetCursor(s16 x, s16 y);
void Text_SetFont(u8 fontId);
void Text_SetNoClipOnce();
void Text_SetWindow(u32 *layer, s32 x, s32 y, s32 w, s32 h, u32 unused);
void Text_SetWindowRect(u32 *layer, const s16 *rect, u32 noClip);
char *Text_Sprintf(char *out, const char *fmt, ...);
void Text_WordWrap(char *text, u8 mode);

#endif
