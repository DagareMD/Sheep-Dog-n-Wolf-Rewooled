/*
 * UiQuad and UiIcon keep their table names as method names; the other classes drop the class prefix (src/README.md).
 * UiFrame_LoadSkin and ScrollList_Update are named after other classes in the tables but their `this` is a UiCursor and
 * a ScrollText (the offsets they touch), so they are defined as members of those.
 */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
#include "../sdk/d3d7.h"
#include "../sdk/win32.h"

#include "../sdk/ddraw.h"
#include "../sdk/crt.h"
class Mat44;

/* A texture stage. */
enum TexStage { TEX_STAGE_0 };

#define SDW_MEMBERS_RenderPoly                                              \
    RenderPoly();                            /* RenderPoly_Ctor */ \
    /* virtual ~RenderPoly() is generated */ /* RenderPoly_Dtor */

#define SDW_MEMBERS_UiIcon        \
    void SetHasExtraQuads(s32 on) \
    {                             \
        hasExtraQuads = on;       \
    }
#define SDW_MEMBERS_Texture \
    long Surface_LockForWrite(DDSURFACEDESC2 *desc);
#define SDW_MEMBERS_D3DApp                                                      \
    void Render_SetStateFlags(u32 flags);              \
    void Render_ClearStateFlags(u32 flags);              \
    void DrawFlatTris(void *verts, u32 count);      /* inline, defined below */ \
    void DrawTexTris(void *verts, u32 count);       /* inline, defined below */ \
    void BindTexture(Texture *tex, TexStage stage); /* inline, defined below */ \
    void ClearStateFlagsInline(u32 flags);          /* inline, defined below */
#define SDW_MEMBERS_PolyBatcher                                              \
    void SubmitPoly(RenderPoly *poly); /* PolyBatcher_SubmitPoly */ \
    void AddPoly(RenderPoly *poly);    /* inline, defined below */
#include "sdw_classes.h"
#define SDW_INLINE_UIQUAD_SETFADELEVEL_U8 1
#include "ui_quad_inlines.h"
#undef SDW_INLINE_UIQUAD_SETFADELEVEL_U8
#define SDW_INLINE_UIQUAD_SETCOLOR_U32 1
#include "ui_quad_inlines.h"
#undef SDW_INLINE_UIQUAD_SETCOLOR_U32
#define SDW_INLINE_UIICON_SETENABLED_S32 1
#define SDW_INLINE_UIICON_SETHIGHLIGHTED_S32 1
#include "ui_icon_inlines.h"
#undef SDW_INLINE_UIICON_SETENABLED_S32
#undef SDW_INLINE_UIICON_SETHIGHLIGHTED_S32
#include "sdw_global_views.h"

typedef void (*MenuHandler)(u8 msg, MenuPage *self);

/* The DAV directory (DavHeader.dir): packed, its pointers sit at +6/+0xa/+0x12 (Load_DAV, src/engine/load_dav.cpp),
 * which the struct generator cannot lay out, so it is declared here. */
#include "sdw_fileptr.h"
#pragma pack(push, 1)
struct DavDirectory {
    u16 indexCount;
    u16 bitmapCount;
    u16 unk04;
    SDW_DAVPTR(u16) indices;
    SDW_DAVPTR(DavBitmapRec) bitmaps; /* 10-byte records */
    u32 fileSize;
    SDW_DAVPTR(u32) idLists;
};
#pragma pack(pop)

/* A bitmap's texel rect as the setup functions keep it: one local, so the four fields sit together in the frame in this
 * order (UiQuad_SetFromBitmap, UiIcon_Setup). */
struct UiBitmapRect {
    s16 u, v, w, h;
};

/* ---- callees ---- */
#include "fixed_math.h"
#include "scn_tools.h"
#include "draw2d.h"
#include "text.h"
#include "load_dav.h"
#include "load_warmeshes.h"
#include "../objects/menu.h"
#include "input.h"
#include "scenaric_loop.h"
#include "id_list.h"
#include "game_state.h"
#include "screen.h"
#include "progress.h"
#include "time.h"
#include "stream_player.h"
extern "C" u32 Rgb24_Lerp(u32 a, u32 b, s16 t);
void Draw2D_TexRect(float z, float x0, float y0, float x1, float y1, s32 texIndex, float uTL, float vTL, u32 cTL,
                    float uBL, float vBL, u32 cBL, float uTR, float vTR, u32 cTR, float uBR, float vBR,
                    u32 cBR);
u16 Text_CountWrappedLines(const char *s);
void Menu_BuildConfirmMenu();
void Menu_BuildList(MenuPage *pages, Menu *menu, s8 count, const MenuHandler *handlers);
void StringBank_RandomiseGlyphs();
char *Text_GetUiString(u8 index);
void Dialogue_SetBoxActive(s32 active);
void Dialogue_StopVoice();
void Dialogue_Reset();
u8 Dialogue_Say(const char *text, s32 voiceId, ScnObject *speaker, u32 arg);
void **Res_FindBitmapGroup(void *bmpRecord, u16 *outCount);
void Ui_DrawPanelFill(u32 colorRGB, s16 *rect);
void Ui_BuildFrameQuads(UiFrame *out, s16 *rect, u16 frameResId, u16 inset);
void Ui_DrawFrameQuads(UiFrame *frame, s32 unusedArg);
void Ui_DrawTextBox(TextBox *box, u16 lineCount);
void Ui_DrawMenuBox(MenuBox *box);
uptr *Res_GetValidatedIdList(u16 resId, u16 *outCount);
s16 Menu_AddItems(MenuPage *page, MenuHandler handler, ...);
s32 Rand_Bounded(s32 bound);
u16 Str_Length(const char *s);
u16 Sound_Play(u16 soundId, void *owner, u16 volume, u8 flags, s32 rate);

/* ---- globals used here, defined elsewhere ---- */
extern u32 *g_screenLayerBase;
extern Wolf *g_pWolf;
extern u32 g_gameFlags;        /* 0x1000 = letterbox / dialogue box on */

extern TextPort g_textPort;
extern TextScroll g_textScroll;
#define g_textCursorY g_textPort.cursorY
#define g_scrollTextBegin g_textScroll.begin
#define g_scrollTextSource g_textScroll.source
extern s32 g_dt;
extern s32 g_dtRawMs;
#define g_dtRawMs (*(s16 *)&g_dtRawMs)
extern s32 g_gameTime;


u32 g_fogPauseColor = 0xa05050;
u32 g_waterColor = 0x600060;
u32 g_uiTintColor = 0xa05050;

const char *g_strYes;
const char *g_strNo;
/* the quad Sprite_Draw fills in; its four static-initialiser thunks start the object. */
RenderPoly g_spriteDrawPoly;
#define g_spritePoly g_spriteDrawPoly /* the table name, used by the bodies below */

void *g_dialogueCurText = 0;                   /* the text the dialogue box shows */
u8 g_dialogueMorePages = 0;                    /* ScrollText_Run's result: nonzero while the text runs */
DialogueShownFlags g_dialogueShownFlags = {0}; /* bit 0: Dialogue_Show printed the text this frame */
UiQuad g_telescopeMaskQuads[8] = {0};
char g_menuFooterText[0x80] = {0};
char *g_uiFooterText = 0;
char *g_strValidate = 0;
u16 *g_resTelescopeMaskOuter = 0;
u16 *g_resTelescopeMaskInner = 0;
s16 g_subtitleRect[4] = {0};
Sprite g_spriteCrayon2 = {0};
s16 g_cannonOriginX = 0;
s16 g_cannonOriginY = 0;
MenuPage g_confirmPageRecord[4] = {0};         /* the confirm menu's node array */
char *g_strEmpty = 0;
s16 g_cannonTileH = 0;
s16 g_cannonTileW = 0;
char *g_strCancel = 0;
s16 g_screenRect[4] = {0};
char *g_strUiString21 = 0;
s16 g_telescopeTileW = 0;
s16 g_telescopeTileH = 0;
s16 g_telescopeOriginX = 0;
u16 g_telescopeOriginY = 0;
u16 *g_resCannonMask = 0;
char *g_strUiString1C = 0;
u8 g_hasTelescopeMask = 0;
Sprite g_spriteTriangle = {0};                 /* the page arrow */
Menu g_confirmPage = {0};
u8 g_hasCannonMask = 0;
char *g_strUiString1E = 0;
UiQuad g_cannonMaskQuads[4] = {0};
/* 80 bytes nothing refers to - the size of four more UiQuads, so the array may really be [8] like
 * the telescope's; opaque, kept as bytes (8-aligned like any item of 64 bytes or more, which is). */
u8 g_interfaceUnref_6de1d0[0x50] = {0};
AnimSprite g_animSpriteCrayon1 = {0};
UiFrame g_uiFrameBox = {0};
s32 g_voicePending = 0;               /* voice line waiting for the letterbox */
s32 g_voicePlaying = 0;               /* voice line playing */
ScnObject *g_voiceOwner_2 = 0;        /* its speaker */
u32 g_uiCursorLastMs = 0;             /* one timestamp for every UiCursor's animation */
s16 g_letterboxTimer = 0;             /* ms, 0..1500 */
char g_emptyString[2] = {0};

#define SDW_INLINE_FREE_GAME_CLEARFLAGS_U32 1
#include "game_state_inlines.h"
#undef SDW_INLINE_FREE_GAME_CLEARFLAGS_U32

/* =================: the small UI pieces ================= */

#define SDW_INLINE_FREE_SCREENWIDTHS16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENWIDTHS16
#define SDW_INLINE_FREE_SCREENHEIGHTS16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENHEIGHTS16

inline void Dialogue_DrawArrow(u32 *layer, s32 x, s32 y, u32 color, u32 flip)
{
    g_spriteTriangle.Draw(layer, x, y, x + g_spriteTriangle.widthMinus1, y + g_spriteTriangle.height, color, flip);
}

/* folds the rectangle over itself on either axis: the far edge becomes the origin and the extent negative. */
void UiQuad::UiQuad_Mirror(s32 flipX, s32 flipY)
{
    if (flipX) {
        x += (s16)(w - 1);
        w = -w;
    }
    if (flipY) {
        y += (s16)(h - 1);
        h = -h;
    }
}

/* places the cursor at a text row, centred on the first frame's height. */
void UiCursor::SetPos(s16 x, s16 y)
{
    this->x = x;
    this->y = y + frames[0].hMinus1 / 2;
}

/* a 0..255 slide turned into an X offset of up to one frame width. */
void UiCursor::SetSlide(u8 t256)
{
    slideX = (u32)(t256 * frames[0].wMinus1) >> 8;
}

/* every 100 ms (one timestamp shared by every cursor) steps the four-frame animation. */
void UiCursor::Animate()
{
    u32 now = g_rawTimeMs;
    if (now - g_uiCursorLastMs > 100) {
        frameIndex = (frameIndex + 1) % 4;
        g_uiCursorLastMs = now;
    }
}

/* the fade level of all three quads. */
void UiIcon::UiIcon_SetFadeLevel(u8 level)
{
    mainQuad.SetFadeLevel(level);
    backQuad.SetFadeLevel(level);
    overlayQuad.SetFadeLevel(level);
}

/* the back (or, when enabled and highlighted, the overlay) quad, the "x%u" count, then the icon itself. */
void UiIcon::UiIcon_Draw(u16 layer)
{
    UiQuad *quad = &backQuad;
    if (enabled && highlighted) {
        quad = &overlayQuad;
        if (quantity > 1) {
            Text_SetFont(FONT_DEBUG);
            Text_SetCursor(mainQuad.x + 0x1c, mainQuad.y);
            Text_PrintFmt("x%u", quantity);
            Text_SetFont(FONT_GAME);
        }
    }
    if (hasExtraQuads)
        quad->UiQuad_Draw(layer);
    mainQuad.UiQuad_Draw(layer);
}

/* MenuHandler of the confirm menu's first entry: on msg 0 (draw) prints "yes", blinking while selected. */
void Menu_DrawYes(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_DRAW:
            Text_PrintfStyled(TEXTALIGN_CENTER, self->selected, g_strYes);
    }
}

/* the same for "no". */
void Menu_DrawNo(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_DRAW:
            Text_PrintfStyled(TEXTALIGN_CENTER, self->selected, g_strNo);
    }
}

/* makes the yes/no menu current: one untitled page with the two entries. */
void Menu_BuildConfirmMenu()
{
    Menu m = {g_confirmPageRecord, 0, 4, 1, 0, 0};
    MenuPage *page;
    g_confirmPage = m;
    Menu_SetCurrent(&g_confirmPage);
    page = Menu_AddPage(0, (void *)"--", 0);
    g_strYes = Text_GetUiString(UISTR_YES);
    g_strNo = Text_GetUiString(UISTR_NO);
    Menu_AddItems(page, Menu_DrawYes, MENU_END);
    Menu_AddItems(page, Menu_DrawNo, MENU_END);
}

/* makes a menu over `pages` current with one untitled page of `count` entries drawn by `handlers`. */
void Menu_BuildList(MenuPage *pages, Menu *menu, s8 count, const MenuHandler *handlers)
{
    Menu m = {pages, 0, 2, 1, 0, 0};
    MenuHandler fn;
    s8 k;
    MenuPage *page;
    for (k = 0; k < count; k++) {
        fn = handlers[k];
        m.capacity++;
    }
    *menu = m;
    Menu_SetCurrent(menu);
    page = Menu_AddPage(0, (void *)"--", 0);
    for (k = 0; k < count; k++) {
        fn = handlers[k];
        Menu_AddItems(page, fn, MENU_END);
    }
}

/* passes every string of every list of the string bank through Text_ScrambleGlyphs. */
void StringBank_RandomiseGlyphs()
{
    char *cur;
    u8 num;
    u8 j;
    StringBank *sb = &g_pDav->strings;
    u16 list;
    for (list = 0; list < sb->listCount; list++) {
        if (sb->lists) {
            cur = sb->lists[list];
            num = *cur;
            cur++;
            for (j = 0; j < num; j++) {
                Text_ScrambleGlyphs(cur);
                cur += Str_Length(cur) + 1;
            }
        }
    }
}

/* a boxed, vertically centred text: dark panel 4 px larger than rect (default g_subtitleRect), frame, text. */
void Ui_DrawSubtitleBox(const char *text, const s16 *rect, u16 frameStyle)
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreRect,
                   rect ? rect : g_subtitleRect);
    u32 rgb = 0x80808;
    s16 box[4];
    if (!rect)
        rect = g_subtitleRect;
    box[0] = rect[0] - 4;
    box[1] = rect[1] - 4;
    box[2] = rect[2] + 4;
    box[3] = rect[3] + 4;
    Ui_DrawPanelFill(rgb, box);
    Ui_BuildFrameQuads(&g_uiFrameBox, box, frameStyle, 8);
    Ui_DrawFrameQuads(&g_uiFrameBox, 0);
    Text_SetFont(FONT_GAME);
    Text_SetWindowRect(g_screenLayerBase + 6, rect, 1);
    Text_CenterVertically(Text_CountWrappedLines(text));
    Text_Printf(TEXTALIGN_CENTER, text);
    Hud_EndBox_stub();
}

/* string `index` of the string bank's list 0 (the UI strings), or an empty string. */
char *Text_GetUiString(u8 index)
{
    char *cur;
    u8 count;
    StringBank *sb = &g_pDav->strings;
    if (sb->lists) {
        cur = sb->lists[0];
        count = *cur;
        cur++;
        if (index >= count)
            return g_emptyString;
        while (index--)
            cur += Str_Length(cur) + 1;
        return cur;
    }
    return &g_emptyString[1];
}

/* the dialogue box (letterbox) on or off; either way the scrolling text starts again. */
void Dialogue_SetBoxActive(s32 active)
{
    if (active)
        g_gameFlags |= GF_LETTERBOX;
    else
        Game_ClearFlags(GF_LETTERBOX);
    if (active && g_letterboxState != LETTERBOX_OPEN)
        g_letterboxState = LETTERBOX_OPENING;
    g_scrollTextSource = 0;
    g_scrollTextBegin = 0;
}

void Dialogue_StopVoice()
{
    if (g_voicePlaying)
        Voice_StopStream(g_voiceOwner_2);
    g_voicePlaying = 0;
    g_voicePending = 0;
}

/* closes the dialogue box at once and stops its voice. */
void Dialogue_Reset()
{
    Game_ClearFlags(GF_LETTERBOX);
    g_letterboxState = LETTERBOX_OPENING;
    g_gameFlags |= GF_TRANSITION_IDLE;
    g_scrollTextSource = 0;
    g_scrollTextBegin = 0;
    Dialogue_StopVoice();
}

/* latches a voice line; it starts (and the speaker gets message 0x76) only once the letterbox is fully in. */
void Dialogue_StartVoice(s32 voiceId, ScnObject *speaker)
{
    g_voicePending = voiceId;
    g_voicePlaying = 0;
    g_voiceOwner_2 = speaker;
    if (g_voicePending && g_letterboxState == LETTERBOX_OPEN) {
        Voice_PlayStream(g_voicePending, g_voiceOwner_2);
        if (speaker)
            speaker->HandleMessage(0, MSG_VOICE_STARTED, 0);
        g_voicePlaying = g_voicePending;
        g_voicePending = 0;
    }
}

/* one frame of the dialogue box: opens it on a new text, prints the page and the page arrows once the
 * letterbox is in; 0 when the text is done (and closes the box), else the run result + the letterbox state. */
u8 Dialogue_Show(void *text, s32 arg)
{
    HudElement hud(HudElement::Centre, HudElement::End, Text_ElementCentre);
    g_dialogueShownFlags.bits.shown = 0;
    if (text != g_dialogueCurText) {
        g_dialogueCurText = text;
        g_dialogueMorePages = 1;
        Dialogue_SetBoxActive(1);
    }
    if (g_letterboxState == LETTERBOX_OPEN) {
        Text_SetFont(FONT_GAME);
        Text_SetWindow(g_screenLayerBase + 4, 0x20, ScreenHeightS16() - 0x2e, ScreenWidthS16() - 0x40,
                       g_pCurFont->lineHeight * 2, 1);
        g_dialogueMorePages = ScrollText_Run(2, (char *)text, arg);
        g_dialogueShownFlags.bits.shown = 1;
        Hud_EndBox_stub();
        if (g_textScroll.flagBits.open && g_pProgress->optionBits.gate) {
            if (g_textScroll.flagBits.hasPrev)
                Dialogue_DrawArrow(g_screenLayerBase + 4, ScreenWidthS16() - 0x1c, ScreenHeightS16() - 0x2e, 0xc0,
                                   SPRFLIP_NONE);
            if (g_textScroll.flagBits.hasNext)
                Dialogue_DrawArrow(g_screenLayerBase + 4, ScreenWidthS16() - 0x1c,
                                   ScreenHeightS16() + g_spriteTriangle.height - 0x2a, 0xc0, SPRFLIP_UV_ROT180);
            if (!(g_textScroll.flagBits.hasPrev | g_textScroll.flagBits.hasNext)) {
                Dialogue_DrawArrow(g_screenLayerBase + 4, 0xe, ScreenHeightS16() + g_spriteTriangle.height - 0x30, 0xc0,
                                   SPRFLIP_ROT270);
                Dialogue_DrawArrow(g_screenLayerBase + 4, ScreenWidthS16() - 0x1c,
                                   ScreenHeightS16() + g_spriteTriangle.height - 0x2f, 0xc0, SPRFLIP_ROT90);
            }
        }
    }
    if (!g_dialogueMorePages || !(g_gameFlags & GF_LETTERBOX)) {
        Dialogue_SetBoxActive(0);
        return 0;
    }
    return g_dialogueMorePages + g_letterboxState;
}

/* Dialogue_Show, first starting the voice line when the text is new. */
u8 Dialogue_Say(const char *text, s32 voiceId, ScnObject *speaker, u32 arg)
{
    if ((void *)text != g_dialogueCurText && voiceId) {
        g_pStreamPlayer->StopVoice();
        Dialogue_StartVoice(voiceId, speaker);
    }
    return Dialogue_Show((void *)text, arg);
}

void ScrollText::Init(char *text, const s16 *rect)
{
    u16 lines;
    this->text = text;
    *(Vec4s *)this->rect = *(const Vec4s *)rect;
    scrollDir = 0;
    topLine = 0;
    Text_SetWindowRect(0, this->rect, 0);
    lines = Text_CountWrappedLines(text);
    maxTopLine = lines < g_pCurFont->rows ? 0 : lines - g_pCurFont->rows;
}

/* smooth line scrolling: dir < 0 / > 0 starts a scroll of one line pitch, then it advances a pixel a frame. */
void ScrollText::ScrollList_Update(s8 dir)
{
    u16 i;
    Text_SetFont(FONT_GAME);
    Text_SetWindowRect(0, rect, 0);
    if (!scrollDir) {
        if (dir < 0 && topLine) {
            scrollDir = dir;
            if (topLine) {
                topLine--;
                scrollPixels = g_pCurFont->lineHeight;
            }
        } else if (dir > 0 && topLine < maxTopLine) {
            scrollDir = dir;
        }
        charOffset = 0;
        for (i = 0; i < topLine; i++)
            charOffset += Text_MeasureLine(text + charOffset);
    }
    if (scrollDir) {
        scrollPixels += scrollDir;
        if (scrollPixels == g_pCurFont->lineHeight || !scrollPixels) {
            if (scrollDir > 0 && topLine < maxTopLine) {
                topLine++;
                charOffset = 0;
                for (i = 0; i < topLine; i++)
                    charOffset += Text_MeasureLine(text + charOffset);
            }
            scrollPixels = 0;
            scrollDir = 0;
        }
    }
}

/* the "more below" arrow, then the visible part of the text. */
void ScrollText::Draw(u8 layerIndex, u8 align)
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreRect, rect);
    if (topLine < maxTopLine)
        Dialogue_DrawArrow(g_screenLayerBase + layerIndex, rect[0] + rect[2] / 2, rect[1] + rect[3] - 2, 0x808080,
                           SPRFLIP_UV_ROT180);
    Text_SetFont(FONT_GAME);
    Text_SetWindowRect(g_screenLayerBase + layerIndex, rect, 1);
    g_textCursorY = -scrollPixels;
    Text_Printf(align, text + charOffset);
    Hud_EndBox_stub();
}

/* texture rect from DAV bitmap *bitmap; at (x, y) scaled by scaleX/Y (1024 = 1), or centred in a box. */
void UiQuad::UiQuad_SetFromBitmap(const u16 *bitmap, s32 x, s32 y, s32 boxW, s32 boxH, s32 scaleX, s32 scaleY)
{
    DavBitmapRec *bm = g_pDav->header->dir->bitmaps;
    UiBitmapRect img;
    texIndex = bm[*bitmap].page;
    img.u = bm[*bitmap].u;
    img.v = bm[*bitmap].v;
    img.w = bm[*bitmap].width;
    img.h = bm[*bitmap].height;
    if (!(boxW + boxH)) {
        this->x = x;
        this->y = y;
        w = (img.w * scaleX) >> 10;
        h = (img.h * scaleY) >> 10;
    } else {
        this->x = x + (boxW + 1 - ((img.w * scaleX) >> 10)) / 2;
        this->y = y + (boxH + 1 - ((img.h * scaleY) >> 10)) / 2;
        w = (img.w * scaleX) >> 10;
        h = (img.h * scaleY) >> 10;
    }
    u = img.u;
    v = img.v;
    wMinus1 = img.w - 1;
    hMinus1 = img.h - 1;
}

/* one textured rectangle at layer z = layer / 120. */
void UiQuad::UiQuad_Draw(u16 layer)
{
    u32 qh; /* declared before c: one bucket, and the later-declared local gets the higher slot */
    u32 c;
    float z;
    u32 u, v, qw;
    c = Color_RgbToBgr(color);
    z = layer / 120.0f;
    u = this->u;
    v = this->v;
    qw = wMinus1;
    qh = hMinus1;
    Draw2D_TexRect(z, g_screen.ScaleX(x), g_screen.ScaleY(y), g_screen.ScaleX(x + w), g_screen.ScaleY(y + h), texIndex,
                   Tex_CornerUV(0, qw, u), Tex_CornerUV(0, qh, v), c, Tex_CornerUV(0, qw, u), Tex_CornerUV(qh, qh, v),
                   c, Tex_CornerUV(qw, qw, u), Tex_CornerUV(0, qh, v), c, Tex_CornerUV(qw, qw, u),
                   Tex_CornerUV(qh, qh, v), c);
}

/* loads the cursor skin: the base bitmap (resource 0x27) and the four frames (0x28..0x2b), at (x, y). */
void UiCursor::UiFrame_LoadSkin(s16 x, s16 y)
{
    DavBitmapRec *bm = g_pDav->header->dir->bitmaps;
    u16 total;
    void **res;
    u16 *id;
    u16 frame;
    u16 u, v, picW, picH;
    res = (void **)Res_GetValidatedIdList(DAV_IDI_IGLVOL0_, &total);
    id = (u16 *)*res;
    baseTexIndex = bm[*id].page;
    u = bm[*id].u;
    v = bm[*id].v;
    picW = bm[*id].width;
    picH = bm[*id].height;
    baseU = u;
    baseV = v;
    baseWMinus1 = (u8)picW - 1;
    baseHMinus1 = (u8)picH - 1;
    for (frame = 0; frame < 4; frame++) {
        res = (void **)Res_GetValidatedIdList(frame + DAV_IDI_IGLVOL1_, &total);
        id = (u16 *)*res;
        frames[frame].texIndex = bm[*id].page;
        u = bm[*id].u;
        v = bm[*id].v;
        picW = bm[*id].width;
        picH = bm[*id].height;
        frames[frame].u = u;
        frames[frame].v = v;
        frames[frame].wMinus1 = (u8)picW - 1; /* the u8 casts: the original subtracts from the low byte */
        frames[frame].hMinus1 = (u8)picH - 1;
    }
    this->x = x;
    this->y = y;
    frameW = picW;
    frameH = picH;
    frameColor = 0x4bccff;
    baseColor = 0x4bccff;
    slideX = 0;
    frameIndex = 0;
}

/* the base quad (slid by slideX), then the current animation frame. */
void UiCursor::Draw(u32 *layer)
{
    float z = g_screen.Draw2D_LayerToZ(layer);
    u32 color = Color_RgbToBgr(baseColor);
    s16 qx = x + slideX;
    s16 qy = y + (frames[0].hMinus1 - baseHMinus1) / 2;
    s16 qw = baseWMinus1;
    s16 qh = baseHMinus1;
    Draw2D_TexRect(z, g_screen.ScaleX(qx), g_screen.ScaleY(qy), g_screen.ScaleX(qx + qw), g_screen.ScaleY(qy + qh),
                   baseTexIndex, Tex_CornerUV(0, baseWMinus1, baseU), Tex_CornerUV(0, baseHMinus1, baseV), color,
                   Tex_CornerUV(0, baseWMinus1, baseU), Tex_CornerUV(baseHMinus1, baseHMinus1, baseV), color,
                   Tex_CornerUV(baseWMinus1, baseWMinus1, baseU), Tex_CornerUV(0, baseHMinus1, baseV), color,
                   Tex_CornerUV(baseWMinus1, baseWMinus1, baseU), Tex_CornerUV(baseHMinus1, baseHMinus1, baseV), color);
    color = Color_RgbToBgr(frameColor);
    qx = frames[frameIndex].u;
    qy = frames[frameIndex].v;
    qw = frames[frameIndex].wMinus1;
    qh = frames[frameIndex].hMinus1;
    Draw2D_TexRect(z, g_screen.ScaleX(x), g_screen.ScaleY(y), g_screen.ScaleX(x + frameW), g_screen.ScaleY(y + frameH),
                   frames[frameIndex].texIndex, Tex_CornerUV(0, qw, qx), Tex_CornerUV(0, qh, qy), color,
                   Tex_CornerUV(0, qw, qx), Tex_CornerUV(qh, qh, qy), color, Tex_CornerUV(qw, qw, qx),
                   Tex_CornerUV(0, qh, qy), color, Tex_CornerUV(qw, qw, qx), Tex_CornerUV(qh, qh, qy), color);
}

/* the icon from up to three bitmaps at (x, y); the back and overlay quads are centred on the main one's
 * size and tinted grey. */
void UiIcon::UiIcon_Setup(const u16 *mainRes, const u16 *overlayRes, const u16 *backRes, s16 x, s16 y, u16 scaleX,
                          u16 scaleY, s32 scaleExtras)
{
    DavBitmapRec *bitmaps = g_pDav->header->dir->bitmaps;
    UiBitmapRect img;
    SetEnabled(0);
    SetHighlighted(0);
    quantity = 0;
    if (mainRes) {
        mainQuad.UiQuad_SetFromBitmap(mainRes, x, y, 0, 0, scaleX, scaleY);
        img.u = bitmaps[*mainRes].u;
        img.v = bitmaps[*mainRes].v;
        img.w = bitmaps[*mainRes].width;
        img.h = bitmaps[*mainRes].height;
    }
    if (backRes) {
        if (scaleExtras)
            backQuad.UiQuad_SetFromBitmap(backRes, x, y, (u8)(((img.w - 1) * scaleX) >> 10),
                                          (u8)(((img.h - 1) * scaleY) >> 10), scaleX, scaleY);
        else
            backQuad.UiQuad_SetFromBitmap(backRes, x, y, (u8)(((img.w - 1) * scaleX) >> 10),
                                          (u8)(((img.h - 1) * scaleY) >> 10), 0x400, 0x400);
        backQuad.SetColor(0x808080);
    }
    if (overlayRes) {
        overlayQuad.UiQuad_SetFromBitmap(overlayRes, x, y, (u8)(((img.w - 1) * scaleX) >> 10),
                                         (u8)(((img.h - 1) * scaleY) >> 10), 0x400, 0x400);
        overlayQuad.SetColor(0x808080);
    }
    if (!overlayRes && !backRes)
        SetHasExtraQuads(0);
    else
        SetHasExtraQuads(1);
}

/* =================: sprites and HUD panels ================= */

#define g_resTelescopeMaskOuter (*(void **)&g_resTelescopeMaskOuter)
#define g_resTelescopeMaskInner (*(void **)&g_resTelescopeMaskInner)
#define g_resCannonMask (*(void **)&g_resCannonMask)
/* The two vertex formats of the batches: untextured (FVF 0xc4, 0x18 bytes) and textured (FVF 0x1c4, 0x20 bytes). */
struct FlatVertex {
    float x, y, z, rhw;
    u32 diffuse, specular;
};
struct TlVertex {
    float x, y, z, rhw;
    u32 diffuse, specular;
    float u, v;
};

/* ---- inline helpers ---- */

/* Draw a full batch of untextured / textured triangles (count = vertices). */
inline void D3DApp::DrawFlatTris(void *verts, u32 count)
{
    if (count)
        SDW_RD(pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR, verts, count,
                                  0);
}
inline void D3DApp::DrawTexTris(void *verts, u32 count)
{
    if (count)
        SDW_RD(pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                  verts, count, 0);
}
inline void D3DApp::BindTexture(Texture *tex, TexStage stage)
{
    SDW_RD(pD3DDevice)->SetTexture(stage, SDW_RDTEX(tex->surface));
}
/* The inline twin of Render_ClearStateFlags. */
#define SDW_INLINE_D3DAPP_CLEARSTATEFLAGSINLINE_U32 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_CLEARSTATEFLAGSINLINE_U32

/* Queue one triangle: type 1 into the untextured batch, 2 / 3 into the sorted list, >= 4 into its texture page's batch
 * (an immediate page) or the sorted list; a full batch is drawn at once. */
inline void PolyBatcher::AddPoly(RenderPoly *poly)
{
    switch (poly->type) {
        case RPOLY_OPAQUE:
            if (flatBatchCount <= batchCapacity) {
                memcpy((FlatVertex *)flatBatchVerts + flatBatchCount * 3, poly->verts, 0x48);
                flatBatchCount++;
            }
            if (flatBatchCount >= batchCapacity) {
                renderer->Render_SetStateFlags(stateFlags);
                renderer->DrawFlatTris(flatBatchVerts, batchCapacity * 3);
                renderer->ClearStateFlagsInline(stateFlags);
                flatBatchCount = 0;
                textureDirty = 1;
            }
            break;
        case RPOLY_BLEND:
            if (computeSortZ == 1)
                poly->sortZ = poly->verts[2] + poly->verts[8] + poly->verts[14];
            sortedPolys[sortedCount].Assign(poly);
            sortedList[sortedCount] = &sortedPolys[sortedCount];
            sortedCount++;
            break;
        case RPOLY_ADD:
            if (computeSortZ == 1)
                poly->sortZ = poly->verts[2] + poly->verts[8] + poly->verts[14];
            sortedPolys[sortedCount].Assign(poly);
            sortedList[sortedCount] = &sortedPolys[sortedCount];
            sortedCount++;
            break;
        default: {
            float *verts = poly->verts;
            u32 page = (poly->type - RPOLY_TEXTURED_BASE) & ~RPOLY_F_8000;
            if (page < immediateTexCount) {
                u32 *count;
                TlVertex *batch;
                u32 *flags;
                batch = (TlVertex *)texBatchVerts[page];
                count = &texBatchCounts[page];
                flags = &texStateFlags[page];
                if (*count <= batchCapacity) {
                    memcpy(batch + *count * 3, verts, 0x60);
                    (*count)++;
                }
                if (*count >= batchCapacity) {
                    if (page != lastTextureIndex || textureDirty == 1) {
                        renderer->BindTexture(textures[page], TEX_STAGE_0);
                        lastTextureIndex = page;
                        textureDirty = 0;
                    }
                    renderer->Render_SetStateFlags(*flags);
                    renderer->DrawTexTris(batch, batchCapacity * 3);
                    renderer->Render_ClearStateFlags(*flags);
                    *count = 0;
                }
            } else {
                if (computeSortZ == 1)
                    poly->sortZ = verts[2] + verts[10] + verts[18];
                sortedPolys[sortedCount].Assign(poly);
                sortedList[sortedCount] = &sortedPolys[sortedCount];
                sortedCount++;
            }
            break;
        }
    }
}

/* Turn a frame strip's four corners by 90 degrees about its first corner (the side strips are the top strip turned). */
inline void Ui_RotateQuad(s16 *q)
{
    s16 dy = q[5] - q[1];
    s16 dx = q[2] - q[0];
    q[2] = q[0];
    q[3] = q[1] + dx;
    q[4] = q[0] - dy;
    q[5] = q[1];
    q[6] = q[4];
    q[7] = q[3];
}

/* A corner (x, y) packed in one dword, as Ui_BuildFrameQuads stores it. */
#define UI_PT(x, y) ((u32)(((x) & 0xffff) | ((y) << 16)))
/* 24-bit BGR to one ARGB4444 texel (alpha written separately). */
#define BGR_TO_4444(c) \
    ((((c) >> 4) & 0xf) | (((c) >> 8) & 0xf0) | (((c) >> 12) & 0xf00) | (((c) >> 16) & 0xf000 & 0xffff))

/* point the sprite at the first bitmap of DAV id-list resId: its entry, size - 1, origin and texture page.
 * Returns whether it was found. */
s32 Sprite::LoadFromRes(u16 resId)
{
    void **ids;
    u16 *entry;
    u16 countv;
    DavBitmapRec *rec;

    ids = (void **)Res_GetValidatedIdList(resId, &countv);
    height = 0;
    widthMinus1 = 0;
    texture = 0;
    if (ids != 0 && countv != 0) {
        entry = (u16 *)*ids;
        texture = entry;
        rec = g_pDav->header->dir->bitmaps + *entry;
        widthMinus1 = rec->width - 1;
        height = rec->height - 1;
        u = (u8)rec->u;
        v = (u8)rec->v;
        texPage = (u16)TexAtlas_GetPage(rec);
    }
    return texture != 0;
}

/* draw the sprite over the HUD rect (x0, y0)-(x1, y1) (512x240 space) on a 2D layer, tinted, laid out by
 * flipMode (SpriteFlipMode): two triangles through g_spritePoly. */
void Sprite::Draw(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u32 flipMode)
{
    float cxs;
    float cy;
    float z;
    float ax2;
    float ayn;
    float sy02;
    u32 bgrp;
    float xB;
    float by;
    float sx0n;
    float sy1;
    float invW;
    TlVertex *vtxp;
    float uv[4][2];
    float sx1;
    u8 rotTmp;

    if (texture != 0) {
        z = g_screen.Draw2D_LayerToZ(layer);
        g_spritePoly.type = texPage + RPOLY_TEXTURED_BASE;
        g_spritePoly.sortZ = z;
        bgrp = Color_RgbToBgr(color);
        uv[0][0] = Tex_CornerUV(0, widthMinus1, u);
        uv[0][1] = Tex_CornerUV(0, height, v);
        uv[1][0] = Tex_CornerUV(widthMinus1, widthMinus1, u);
        uv[1][1] = Tex_CornerUV(0, height, v);
        uv[2][0] = Tex_CornerUV(widthMinus1, widthMinus1, u);
        uv[2][1] = Tex_CornerUV(height, height, v);
        uv[3][0] = Tex_CornerUV(0, widthMinus1, u);
        uv[3][1] = Tex_CornerUV(height, height, v);
        sx0n = g_screen.ScaleX(x0);
        sy02 = g_screen.ScaleY(y0);
        sx1 = g_screen.ScaleX(x1);
        sy1 = g_screen.ScaleY(y1);
        switch (flipMode) {
            case SPRFLIP_ROT90:
                cxs = (sx0n + sx1) / 2.0f;
                cy = (sy02 + sy1) / 2.0f;
                ax2 = cxs - cy + sy02;
                ayn = cxs + cy - sx1;
                xB = cxs - cy + sy1;
                by = cxs + cy - sx0n;
                rotTmp = 1;
                break;
            case SPRFLIP_UV_ROT180:
                ax2 = sx0n;
                ayn = sy02;
                xB = sx1;
                by = sy1;
                rotTmp = 2;
                break;
            case SPRFLIP_ROT270:
                cxs = (sx0n + sx1) / 2.0f;
                cy = (sy02 + sy1) / 2.0f;
                ax2 = cy + cxs - sy1;
                ayn = cy - cxs + sx0n;
                xB = cy + cxs - sy02;
                by = cy - cxs + sx1;
                rotTmp = 3;
                break;
            case SPRFLIP_MIRROR_X:
                ax2 = sx1;
                ayn = sy02;
                xB = sx0n;
                by = sy1;
                rotTmp = 0;
                break;
            case SPRFLIP_MIRROR_Y:
                ax2 = sx0n;
                ayn = sy1;
                xB = sx1;
                by = sy02;
                rotTmp = 0;
                break;
            case SPRFLIP_MIRROR_XY:
                ax2 = sx1;
                ayn = sy1;
                xB = sx0n;
                by = sy02;
                rotTmp = 0;
                break;
            default:
                ax2 = sx0n;
                ayn = sy02;
                xB = sx1;
                by = sy1;
                rotTmp = 0;
                break;
        }
        invW = 1.0f / (g_pViewFrustum->nearZ);
        vtxp = (TlVertex *)g_spritePoly.verts;
        vtxp[0].x = ax2;
        vtxp[0].y = ayn;
        vtxp[0].z = z;
        vtxp[0].rhw = invW;
        vtxp[0].diffuse = bgrp;
        vtxp[0].specular = 0xff000000;
        vtxp[1].x = xB;
        vtxp[1].y = ayn;
        vtxp[1].z = z;
        vtxp[1].rhw = invW;
        vtxp[1].diffuse = bgrp;
        vtxp[1].specular = 0xff000000;
        vtxp[2].x = xB;
        vtxp[2].y = by;
        vtxp[2].z = z;
        vtxp[2].rhw = invW;
        vtxp[2].diffuse = bgrp;
        vtxp[2].specular = 0xff000000;
        vtxp[0].u = uv[rotTmp][0];
        vtxp[0].v = uv[rotTmp][1];
        vtxp[1].u = uv[(rotTmp + 1) & 3][0];
        vtxp[1].v = uv[(rotTmp + 1) & 3][1];
        vtxp[2].u = uv[(rotTmp + 2) & 3][0];
        vtxp[2].v = uv[(rotTmp + 2) & 3][1];
        g_pPolyBin->AddPoly(&g_spritePoly);
        vtxp[1].x = ax2;
        vtxp[1].y = by;
        vtxp[1].u = uv[(rotTmp - 1) & 3][0];
        vtxp[1].v = uv[(rotTmp - 1) & 3][1];
        g_pPolyBin->SubmitPoly(&g_spritePoly);
    }
}

/* AnimSprite::Draw's argument list on a plain sprite: the frame is dropped. */
void Sprite::DrawFrame(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u32 frame, u32 flipMode)
{
    Draw(layer, x0, y0, x1, y1, color, flipMode);
}

/* a plain forwarder to Sprite::Draw. */
void Sprite::DrawThunk(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u32 flipMode)
{
    Draw(layer, x0, y0, x1, y1, color, flipMode);
}

/* find the DAV id-list group whose entries include bmpRecord; returns the entry array and its length (NULL, 0
 * when no group holds it). */
void **Res_FindBitmapGroup(void *bmpRecord, u16 *outCount)
{
    uptr *ents;
    u16 n;
    u16 id;
    uptr *recp;
    u32 j;
    u32 counts;
    u32 i;

    recp = IdList_GetBlob(&counts);
    if (recp != 0) {
        for (i = 0; i < counts; i++, recp = IdList_Next(recp)) {
            id = IdList_GetId((u16 *)recp);
            ents = IdList_FindWithCount(id, &n);
            for (j = 0; j < n; j++) {
                if (ents[j] == (uptr)bmpRecord) {
                    *outCount = n;
                    return (void **)ents;
                }
            }
        }
    }
    *outCount = 0;
    return 0;
}

/* load the frames of an animated sprite: the size of the bitmap resId names, then every bitmap of the id-list
 * group holding it (at most 10) as {page, u, v}. Returns whether it was found. */
s32 AnimSprite::InitFromRes(u16 resId)
{
    DavBitmapRec **groups;
    void **ids;
    void *entry;
    u16 countv;
    SpriteFrame *f;
    DavBitmapRec *rec;
    s32 iv;

    frameCount = 0;
    texture = 0;
    ids = (void **)Res_GetValidatedIdList(resId, &countv);
    if (ids != 0 && countv != 0) {
        entry = *ids;
        rec = g_pDav->header->dir->bitmaps + *(u16 *)entry;
        width = rec->width - 1;
        height = rec->height - 1;
        groups = (DavBitmapRec **)Res_FindBitmapGroup(rec, &frameCount);
        if (groups != 0 && frameCount > 0) {
            texture = entry;
            if (frameCount > 10)
                frameCount = 10;
            for (f = frames, iv = 0; iv < frameCount; iv++, f++) {
                rec = groups[iv];
                f->u = (u8)rec->u;
                f->v = (u8)rec->v;
                f->texPage = (u16)TexAtlas_GetPage(rec);
            }
        }
    }
    return texture != 0;
}

/* Sprite::Draw for one frame of the sheet (frame wraps modulo the frame count). */
void AnimSprite::Draw(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u8 frame, u32 flipMode)
{
    float cxs;
    float cy;
    float z;
    float ax2;
    float ayn;
    float sy02;
    u32 bgrp;
    float xB;
    float by;
    float sx0n;
    float sy1;
    SpriteFrame *frs;
    float invW;
    TlVertex *vtxp;
    float uv[4][2];
    float sx1;
    u8 rotTmp;

    if (frameCount > 0) {
        z = g_screen.Draw2D_LayerToZ(layer);
        if (frame >= frameCount)
            frame = frame % frameCount;
        frs = &frames[frame];
        g_spritePoly.type = frs->texPage + RPOLY_TEXTURED_BASE;
        g_spritePoly.sortZ = z;
        bgrp = Color_RgbToBgr(color);
        uv[0][0] = Tex_CornerUV(0, width, frs->u);
        uv[0][1] = Tex_CornerUV(0, height, frs->v);
        uv[1][0] = Tex_CornerUV(width, width, frs->u);
        uv[1][1] = Tex_CornerUV(0, height, frs->v);
        uv[2][0] = Tex_CornerUV(width, width, frs->u);
        uv[2][1] = Tex_CornerUV(height, height, frs->v);
        uv[3][0] = Tex_CornerUV(0, width, frs->u);
        uv[3][1] = Tex_CornerUV(height, height, frs->v);
        sx0n = g_screen.ScaleX(x0);
        sy02 = g_screen.ScaleY(y0);
        sx1 = g_screen.ScaleX(x1);
        sy1 = g_screen.ScaleY(y1);
        switch (flipMode) {
            case SPRFLIP_ROT90:
                cxs = (sx0n + sx1) / 2.0f;
                cy = (sy02 + sy1) / 2.0f;
                ax2 = cxs - cy + sy02;
                ayn = cxs + cy - sx1;
                xB = cxs - cy + sy1;
                by = cxs + cy - sx0n;
                rotTmp = 1;
                break;
            case SPRFLIP_UV_ROT180:
                ax2 = sx0n;
                ayn = sy02;
                xB = sx1;
                by = sy1;
                rotTmp = 2;
                break;
            case SPRFLIP_ROT270:
                cxs = (sx0n + sx1) / 2.0f;
                cy = (sy02 + sy1) / 2.0f;
                ax2 = cy + cxs - sy1;
                ayn = cy - cxs + sx0n;
                xB = cy + cxs - sy02;
                by = cy - cxs + sx1;
                rotTmp = 3;
                break;
            case SPRFLIP_MIRROR_X:
                ax2 = sx1;
                ayn = sy02;
                xB = sx0n;
                by = sy1;
                rotTmp = 0;
                break;
            case SPRFLIP_MIRROR_Y:
                ax2 = sx0n;
                ayn = sy1;
                xB = sx1;
                by = sy02;
                rotTmp = 0;
                break;
            case SPRFLIP_MIRROR_XY:
                ax2 = sx1;
                ayn = sy1;
                xB = sx0n;
                by = sy02;
                rotTmp = 0;
                break;
            default:
                ax2 = sx0n;
                ayn = sy02;
                xB = sx1;
                by = sy1;
                rotTmp = 0;
                break;
        }
        invW = 1.0f / (g_pViewFrustum->nearZ);
        vtxp = (TlVertex *)g_spritePoly.verts;
        vtxp[0].x = ax2;
        vtxp[0].y = ayn;
        vtxp[0].z = z;
        vtxp[0].rhw = invW;
        vtxp[0].diffuse = bgrp;
        vtxp[0].specular = 0xff000000;
        vtxp[1].x = xB;
        vtxp[1].y = ayn;
        vtxp[1].z = z;
        vtxp[1].rhw = invW;
        vtxp[1].diffuse = bgrp;
        vtxp[1].specular = 0xff000000;
        vtxp[2].x = xB;
        vtxp[2].y = by;
        vtxp[2].z = z;
        vtxp[2].rhw = invW;
        vtxp[2].diffuse = bgrp;
        vtxp[2].specular = 0xff000000;
        vtxp[0].u = uv[rotTmp][0];
        vtxp[0].v = uv[rotTmp][1];
        vtxp[1].u = uv[(rotTmp + 1) & 3][0];
        vtxp[1].v = uv[(rotTmp + 1) & 3][1];
        vtxp[2].u = uv[(rotTmp + 2) & 3][0];
        vtxp[2].v = uv[(rotTmp + 2) & 3][1];
        g_pPolyBin->AddPoly(&g_spritePoly);
        vtxp[1].x = ax2;
        vtxp[1].y = by;
        vtxp[1].u = uv[(rotTmp - 1) & 3][0];
        vtxp[1].v = uv[(rotTmp - 1) & 3][1];
        g_pPolyBin->SubmitPoly(&g_spritePoly);
    }
}

/* unreferenced forwarder to AnimSprite::Draw that skips its eighth argument. */
void AnimSprite::DrawThunk9(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u8 frame, u32 unused, u32 flipMode)
{
    Draw(layer, x0, y0, x1, y1, color, frame, flipMode);
}

/* unreferenced forwarder to AnimSprite::Draw. */
void AnimSprite::DrawThunk8(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u8 frame, u32 flipMode)
{
    Draw(layer, x0, y0, x1, y1, color, frame, flipMode);
}

/* the level's interface: the dialogue box and confirm menu, the crayon / triangle sprites, the HUD colours
 * and rects, the optional cannon-sight and telescope masks, and the UI strings. Called by Load_DAVnWAR. */
/* The 512x240 virtual HUD. */
inline s32 Hud_Width()
{
    return 0x200;
}
inline s32 Hud_Height()
{
    return 0xf0;
}

void Interface_Init()
{
    void **ids;
    DavBitmapRec *bmpTable;
    u16 n;

    bmpTable = g_pDav->header->dir->bitmaps;
    g_hasTelescopeMask = 0;
    Dialogue_Reset();
    Menu_BuildConfirmMenu();
    g_dialogueCurText = 0;
    g_dialogueMorePages = 1;
    g_animSpriteCrayon1.InitFromRes(DAV_IDI_IGLCRAI1);
    g_spriteCrayon2.LoadFromRes(DAV_IDI_IGLCRAI2);
    g_spriteTriangle.LoadFromRes(DAV_IDI_IGLTRIA_);
    g_fogPauseColor = 0xa05050;
    g_waterColor = 0x600060;
    g_uiTintColor = 0xa05050;
    g_subtitleRect[0] = 0x40;
    g_subtitleRect[1] = Hud_Height() / 2;
    g_subtitleRect[2] = Hud_Width() - 0x80;
    g_subtitleRect[3] = 0x40;
    g_screenRect[0] = 0;
    g_screenRect[1] = 0;
    g_screenRect[2] = 0x200;
    g_screenRect[3] = 0xf0;
    StringBank_RandomiseGlyphs();
    ids = (void **)Res_GetValidatedIdList(DAV_IDI_ICSMASQ_, &n);
    if (ids != 0) {
        g_resCannonMask = *ids;
        g_hasCannonMask = 1;
    }
    ids = (void **)Res_GetValidatedIdList(DAV_IDI_I01LVU4_, &n);
    if (ids != 0)
        g_resTelescopeMaskOuter = *ids;
    ids = (void **)Res_GetValidatedIdList(DAV_IDI_I01LVU5_, &n);
    if (ids != 0) {
        g_resTelescopeMaskInner = *ids;
        g_hasTelescopeMask = 1;
    }
    g_strValidate = Text_GetUiString(UISTR_VALIDATE);
    g_strUiString21 = Text_GetUiString(UISTR_CANCEL);
    g_strCancel = Text_GetUiString(UISTR_BACK);
    g_strUiString1C = Text_GetUiString(UISTR_CHANGE);
    g_strUiString1E = Text_GetUiString(UISTR_PRESS_BUTTON);
    g_strEmpty = Text_GetUiString(UISTR_EXIT);
}

/* fill a box's background: paint the 4x4 scratch texture (page count - 2) with the colour at half alpha and
 * stretch it over rect {x, y, w, h}. */
void Ui_DrawPanelFill(u32 color, s16 *rect)
{
    s32 k;
    float x0n;
    float y0;
    float z;
    Texture *texs;
    u32 bgr;
    s32 row;
    DDSURFACEDESC2 *desc;
    float y1p;
    uptr texels;
    float x1p;

    x0n = g_screen.ScaleX(rect[0]);
    y0 = g_screen.ScaleY(rect[1]);
    x1p = g_screen.ScaleX(rect[0] + rect[2]);
    y1p = g_screen.ScaleY(rect[1] + rect[3]);
    texs = g_pPolyBin->textures[g_pPolyBin->texturePageCount - 2];
    desc = new DDSURFACEDESC2;
    texs->Surface_LockForWrite(desc);
    texels = (uptr)desc->lpSurface;
    bgr = Color_RgbToBgr(color);
    *(u16 *)texels = BGR_TO_4444(bgr);
    *(u16 *)texels |= 0x8000;
    for (row = 0; row < 4; row++) {
        for (k = 0; k < 4; k++) {
            ((u16 *)(row * desc->lPitch + texels))[k] = BGR_TO_4444(bgr);
            ((u16 *)(row * desc->lPitch + texels))[k] |= 0x8000;
        }
    }
    texs->Surface_Unlock();
    delete desc;
    z = 0.058333333f;
    Draw2D_TexRect(z, x0n, y0, x1p, y1p, g_pPolyBin->texturePageCount - 2, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f, 0x808080,
                   1.0f, 1.0f, 0x808080, 1.0f, 1.0f, 0x808080);
}

/* build the four border strips of a box frame around rect {x, y, w, h} from the frame bitmap frameResId,
 * pushed out by inset: bottom, top, then the left and right strips (the top strip's layout turned by 90 degrees). */
void Ui_BuildFrameQuads(UiFrame *out, s16 *rect, u16 frameResId, u16 inset)
{
    DavBitmapRec *bmp;
    u16 *entry;
    DavBitmapRec *bmps;
    u16 count;
    void **list;
    u8 u;
    u8 vv;
    u8 w1;
    u8 hm;

    bmps = g_pDav->header->dir->bitmaps;
    list = (void **)Res_GetValidatedIdList(frameResId, &count);
    entry = (u16 *)*list;
    bmp = bmps + *entry;
    u = (u8)bmp->u;
    vv = (u8)bmp->v;
    w1 = bmp->width - 1;
    hm = bmp->height - 1;
    count = bmp->height;
    out->texPage = bmp->page;
    out->cellSize = (w1 << 8) + hm;

    *(u32 *)&out->quads[0][0] = UI_PT(rect[0] - inset, rect[1] + rect[3] - count + inset);
    *(u32 *)&out->quads[0][2] = UI_PT(rect[0] - inset + (rect[2] + inset * 2), rect[1] + rect[3] - count + inset);
    *(u32 *)&out->quads[0][4] = UI_PT(rect[0] - inset, rect[1] + rect[3] - count + inset + count);
    *(u32 *)&out->quads[0][6] =
        UI_PT(rect[0] - inset + (rect[2] + inset * 2), rect[1] + rect[3] - count + inset + count);
    out->uvs[0][0] = Tex_CornerUV(0, w1, u);
    out->uvs[0][1] = Tex_CornerUV(hm, hm, vv);
    out->uvs[0][2] = Tex_CornerUV(w1, w1, u);
    out->uvs[0][3] = Tex_CornerUV(hm, hm, vv);
    out->uvs[0][4] = Tex_CornerUV(0, w1, u);
    out->uvs[0][5] = Tex_CornerUV(0, hm, vv);
    out->uvs[0][6] = Tex_CornerUV(w1, w1, u);
    out->uvs[0][7] = Tex_CornerUV(0, hm, vv);

    *(u32 *)&out->quads[1][0] = UI_PT(rect[0] - inset, rect[1] - inset);
    *(u32 *)&out->quads[1][2] = UI_PT(rect[0] - inset + (rect[2] + inset * 2), rect[1] - inset);
    *(u32 *)&out->quads[1][4] = UI_PT(rect[0] - inset, rect[1] - inset + count);
    *(u32 *)&out->quads[1][6] = UI_PT(rect[0] - inset + (rect[2] + inset * 2), rect[1] - inset + count);
    out->uvs[1][0] = Tex_CornerUV(0, w1, u);
    out->uvs[1][1] = Tex_CornerUV(0, hm, vv);
    out->uvs[1][2] = Tex_CornerUV(w1, w1, u);
    out->uvs[1][3] = Tex_CornerUV(0, hm, vv);
    out->uvs[1][4] = Tex_CornerUV(0, w1, u);
    out->uvs[1][5] = Tex_CornerUV(hm, hm, vv);
    out->uvs[1][6] = Tex_CornerUV(w1, w1, u);
    out->uvs[1][7] = Tex_CornerUV(hm, hm, vv);

    *(u32 *)&out->quads[2][0] = UI_PT(rect[0] + count - inset, rect[1] - inset);
    *(u32 *)&out->quads[2][2] = UI_PT(rect[0] + count - inset + (rect[3] + inset * 2), rect[1] - inset);
    *(u32 *)&out->quads[2][4] = UI_PT(rect[0] + count - inset, rect[1] - inset + count);
    *(u32 *)&out->quads[2][6] = UI_PT(rect[0] + count - inset + (rect[3] + inset * 2), rect[1] - inset + count);
    Ui_RotateQuad(out->quads[2]);
    out->uvs[2][0] = Tex_CornerUV(w1, w1, u);
    out->uvs[2][1] = Tex_CornerUV(hm, hm, vv);
    out->uvs[2][4] = Tex_CornerUV(0, w1, u);
    out->uvs[2][5] = Tex_CornerUV(hm, hm, vv);
    out->uvs[2][2] = Tex_CornerUV(w1, w1, u);
    out->uvs[2][3] = Tex_CornerUV(0, hm, vv);
    out->uvs[2][6] = Tex_CornerUV(0, w1, u);
    out->uvs[2][7] = Tex_CornerUV(0, hm, vv);

    *(u32 *)&out->quads[3][0] = UI_PT(rect[0] + rect[2] + inset, rect[1] - inset);
    *(u32 *)&out->quads[3][2] = UI_PT(rect[0] + rect[2] + inset + (rect[3] + inset * 2), rect[1] - inset);
    *(u32 *)&out->quads[3][4] = UI_PT(rect[0] + rect[2] + inset, rect[1] - inset + count);
    *(u32 *)&out->quads[3][6] = UI_PT(rect[0] + rect[2] + inset + (rect[3] + inset * 2), rect[1] - inset + count);
    Ui_RotateQuad(out->quads[3]);
    out->uvs[3][0] = Tex_CornerUV(0, w1, u);
    out->uvs[3][1] = Tex_CornerUV(0, hm, vv);
    out->uvs[3][4] = Tex_CornerUV(w1, w1, u);
    out->uvs[3][5] = Tex_CornerUV(0, hm, vv);
    out->uvs[3][2] = Tex_CornerUV(0, w1, u);
    out->uvs[3][3] = Tex_CornerUV(hm, hm, vv);
    out->uvs[3][6] = Tex_CornerUV(w1, w1, u);
    out->uvs[3][7] = Tex_CornerUV(hm, hm, vv);
}

/* draw the four strips Ui_BuildFrameQuads built (the side strips slightly nearer), grey vertex colour. */
void Ui_DrawFrameQuads(UiFrame *frame, s32 unused)
{
    float yt;
    float left;
    float x1n;
    float y1;
    u16 strip;
    float z;
    s32 cellH;
    u32 greyCur;
    s32 cellW;

    z = 0.058333333f;
    greyCur = 0x808080;
    cellW = frame->cellSize >> 8;
    cellH = frame->cellSize & 0xff;
    for (strip = 0; strip < 4; strip++) {
        if (strip > 2)
            z = 0.058333333f;
        else
            z = 0.05f;
        left = g_screen.ScaleX(frame->quads[strip][0]);
        yt = g_screen.ScaleY(frame->quads[strip][1]);
        x1n = g_screen.ScaleX(frame->quads[strip][6]);
        y1 = g_screen.ScaleY(frame->quads[strip][7]);
        Draw2D_TexRect(z, left, yt, x1n, y1, frame->texPage, frame->uvs[strip][0], frame->uvs[strip][1], greyCur,
                       frame->uvs[strip][4], frame->uvs[strip][5], greyCur, frame->uvs[strip][2], frame->uvs[strip][3],
                       greyCur, frame->uvs[strip][6], frame->uvs[strip][7], greyCur);
    }
}

/* a framed, word-wrapped text box in font 1 (lineCount 0xffff: count the wrapped lines); when the text and a
 * three-row yes/no confirm menu fit the window, run the confirm menu and latch its choice. */
void Ui_DrawTextBox(TextBox *box, u16 lineCount)
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreRect, box->rect);
    u16 rows;
    s16 frame[4];

    Text_SetFont(FONT_GAME);
    Text_SetWindowRect(g_screenLayerBase + 6, box->rect, 1);
    if (lineCount == 0xffff)
        lineCount = Text_CountWrappedLines(box->text);
    rows = g_pCurFont->rows;
    box->bgColor = 0x80808;
    Text_WordWrap(box->text, TEXTALIGN_CENTER);
    box->fits = lineCount + 3 <= rows;
    if (box->fits) {
        Text_NewLine(2);
        Menu_SetCurrent(&g_confirmPage);
        Menu_Update(MENU_LAYOUT_IN_BOX, TEXTALIGN_CENTER);
        box->confirmChoice = 2 - g_confirmPage.cursor;
    }
    frame[0] = box->rect[0] - 4;
    frame[1] = box->rect[1] - 4;
    frame[2] = box->rect[2] + 4;
    frame[3] = box->rect[3] + 4;
    Ui_DrawPanelFill(box->bgColor, frame);
    Ui_BuildFrameQuads(&g_uiFrameBox, frame, DAV_IDI_IGLCADP_, 8);
    Ui_DrawFrameQuads(&g_uiFrameBox, 0);
    Hud_EndBox_stub();
}

/* Ui_DrawTextBox for a box with its own choice list: when the text and the list fit, run the list and latch
 * its cursor row. */
void Ui_DrawMenuBox(MenuBox *box)
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreRect, box->rect);
    u16 rows;
    s16 frame[4];
    u16 textLines;

    Text_SetFont(FONT_GAME);
    Text_SetWindowRect(g_screenLayerBase + 6, box->rect, 1);
    textLines = Text_CountWrappedLines(box->text);
    rows = g_pCurFont->rows;
    Text_WordWrap(box->text, TEXTALIGN_CONTINUE);
    box->fits = textLines + box->list->count <= rows;
    if (box->fits) {
        Text_NewLine(2);
        Menu_SetCurrent(box->list);
        Menu_Update(MENU_LAYOUT_IN_BOX, TEXTALIGN_CENTER);
        box->cursorRow = box->list->cursor - 1;
    }
    frame[0] = box->rect[0] - 4;
    frame[1] = box->rect[1] - 4;
    frame[2] = box->rect[2] + 4;
    frame[3] = box->rect[3] + 4;
    Ui_DrawPanelFill(box->bgColor, frame);
    Ui_BuildFrameQuads(&g_uiFrameBox, frame, DAV_IDI_IGLCADP_, 8);
    Ui_DrawFrameQuads(&g_uiFrameBox, 0);
    Hud_EndBox_stub();
}

#undef g_resTelescopeMaskOuter
#undef g_resTelescopeMaskInner
#undef g_resCannonMask

/* =================: the HUD overlays, first part ================= */

/* The HUD part uses the U16 screen-size inlines; the UI part uses S16.
 * These distinct names preserve both matched return types. */
/* The HUD part calls UiQuad's methods by their short spelling; they are the functions defined above as
 * UiQuad_SetFromBitmap / UiQuad_Mirror / UiQuad_Draw (see the header). Every Draw / Mirror / SetFromBitmap in this
 * part is a UiQuad call. */
#define SetFromBitmap UiQuad_SetFromBitmap
#define Mirror UiQuad_Mirror
#define Draw UiQuad_Draw
/* inline: the virtual screen size (Screen::virtWidth / virtHeight are u16). */
#define SDW_INLINE_FREE_SCREENWIDTHU16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENWIDTHU16
#define SDW_INLINE_FREE_SCREENHEIGHTU16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENHEIGHTU16

/* An ARGB8888 colour as ARGB4444. The last term's redundant 16-bit mask is the original's. */
#define ARGB4444(c) ((((c) >> 4) & 0xf) | (((c) >> 8) & 0xf0) | (((c) >> 12) & 0xf00) | (((c) >> 16) & 0xf000 & 0xffff))

/* the cinematic bars. While g_gameFlags & 0x1000 the timer runs up to 1500 ms of real time (state 1, then 2
 * when full, which also starts a voice line that was waiting for the bars); otherwise it runs back down to 0 (state 1,
 * then 0 and GF 0x10). The bars grow to 48 virtual lines and fade in from clear to black while opening; they are drawn
 * with the third-last texture page, refilled every frame. curBlend is written and never read. */
void Letterbox_Update()
{
    if (!g_letterboxState)
        return;
    float curZ;
    float theWidth;
    s32 curBlend;
    u32 theColor;
    float nHeight;
    float nBarHf;
    Texture *theTex;
    DDSURFACEDESC2 *desc;
    s16 pBarH;
    s16 alpha;
    uptr thePixels; /* the surface address, as an integer */
    pBarH = g_letterboxTimer * 0x83 / 4096;
    alpha = g_letterboxTimer * 699 / 256;
    if (g_letterboxTimer != 0) {
        theWidth = (float)(g_pD3DAppMain->clientRect.right - g_pD3DAppMain->clientRect.left);
        nHeight = (float)(g_pD3DAppMain->clientRect.bottom - g_pD3DAppMain->clientRect.top);
        curZ = 0.041666668f;
        theTex = g_pPolyBin->textures[g_pPolyBin->texturePageCount - 3];
        desc = new DDSURFACEDESC2;
        theTex->Surface_LockForWrite(desc);
        thePixels = (uptr)desc->lpSurface;
        if (g_letterboxTimer < 1500) {
            theColor = Rgb24_Lerp(0xff, 0, alpha) << 24;
            for (s32 y = 0; y < 4; y++)
                for (s32 curX = 0; curX < 4; curX++)
                    ((u16 *)(y * desc->lPitch + thePixels))[curX] = ARGB4444(theColor);
            curBlend = 2;
            nBarHf = g_screen.ScaleY(pBarH);
        } else {
            for (s32 y = 0; y < 4; y++)
                for (s32 curX = 0; curX < 4; curX++)
                    ((u16 *)(y * desc->lPitch + thePixels))[curX] = 0;
            curBlend = 0;
            nBarHf = g_screen.ScaleY(48);
        }
        theTex->Surface_Unlock();
        delete desc;
        Draw2D_TexRect(curZ, 0, 0, theWidth, nBarHf, g_pPolyBin->texturePageCount - 3, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f,
                       0x808080, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f, 0x808080);
        Draw2D_TexRect(curZ, 0, nHeight - nBarHf, theWidth, nHeight, g_pPolyBin->texturePageCount - 3, 1.0f, 1.0f,
                       0x808080, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f, 0x808080);
    }
    if (!(g_gameFlags & GF_LETTERBOX)) {
        g_dialogueCurText = 0;
        if (g_letterboxTimer > 0) {
            g_letterboxTimer -= g_dtRawMs;
            Dialogue_StopVoice();
            g_letterboxState = LETTERBOX_OPENING;
        } else {
            g_letterboxTimer = 0;
            g_letterboxState = LETTERBOX_OFF;
            g_gameFlags |= GF_TRANSITION_IDLE;
        }
    } else {
        Game_ClearFlags(GF_TRANSITION_IDLE);
        if (g_letterboxTimer < 1500) {
            g_letterboxTimer += g_dtRawMs;
            g_letterboxState = LETTERBOX_OPENING;
        } else {
            g_letterboxTimer = 1500;
            g_letterboxState = LETTERBOX_OPEN;
            if (g_voicePending) {
                Voice_PlayStream(g_voicePending, g_voiceOwner_2);
                if (g_voiceOwner_2)
                    g_voiceOwner_2->HandleMessage(0, MSG_VOICE_STARTED, 0);
                g_voicePlaying = g_voicePending;
                g_voicePending = 0;
            }
        }
    }
}

/* the fade curtain: level 0 (clear) .. 31 (opaque), black or white, over rect or, when rect is NULL, over the
 * whole viewport. The texture's alpha nibble is 15 - level/2 and the page blends with inverted alpha (0 = opaque). It
 * draws on the last texture page, which it refills every call. */
void Fade_DrawOverlay(bool white, u8 level, s16 *rect)
{
    s32 pX;
    float left;
    float top;
    float curZ;
    Texture *tex;
    s32 y;
    DDSURFACEDESC2 *curDesc;
    float nBottom;
    uptr pixels; /* the surface address, as an integer */
    float theRight;
    if (!rect) {
        left = 0;
        top = 0;
        theRight = (float)(g_pD3DAppMain->clientRect.right - g_pD3DAppMain->clientRect.left);
        nBottom = (float)(g_pD3DAppMain->clientRect.bottom - g_pD3DAppMain->clientRect.top);
    } else {
        left = g_screen.ScaleX(rect[0]);
        top = g_screen.ScaleY(rect[1]);
        theRight = g_screen.ScaleX(rect[0] + rect[2]);
        nBottom = g_screen.ScaleY(rect[1] + rect[3]);
    }
    tex = g_pPolyBin->textures[g_pPolyBin->texturePageCount - 1];
    curDesc = new DDSURFACEDESC2;
    tex->Surface_LockForWrite(curDesc);
    pixels = (uptr)curDesc->lpSurface;
    for (y = 0; y < 4; y++) {
        for (pX = 0; pX < 4; pX++) {
            ((u16 *)(y * curDesc->lPitch + pixels))[pX] = ((0xf - level / 2) << 12) & 0xf000;
            if (white)
                ((u16 *)(y * curDesc->lPitch + pixels))[pX] |= 0xffff;
        }
    }
    tex->Surface_Unlock();
    delete curDesc;
    curZ = 0.0083333338f;
    Draw2D_TexRect(curZ, left, top, theRight, nBottom, g_pPolyBin->texturePageCount - 1, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f,
                   0x808080, 1.0f, 1.0f, 0x808080, 1.0f, 1.0f, 0x808080);
}

/* the telescope overlay (class Telescope): a 4x2 mosaic of the outer and inner mask tiles, mirrored into the
 * four corners and centred, at 1.5x (or 1.0x with the depth pulled forward), and black bars over the rest. */
void Hud_DrawTelescopeMask(bool smallScale)
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreScreen);
    s32 nY0;
    s32 pX0;
    s32 curH;
    s32 curW;
    float nZ;
    u32 black;
    DavBitmapRec *nBitmaps;
    s32 theI;
    s16 pBm[4]; /* {u, v, width, height} of the outer tile's bitmap */
    nBitmaps = g_pDav->header->dir->bitmaps;
    nZ = g_screen.Draw2D_LayerToZ(g_screenLayerBase + 4);
    black = Color_RgbToBgr(0);
    if (!smallScale) {
        pBm[0] = nBitmaps[*g_resTelescopeMaskOuter].u;
        pBm[1] = nBitmaps[*g_resTelescopeMaskOuter].v;
        pBm[2] = nBitmaps[*g_resTelescopeMaskOuter].width;
        pBm[3] = nBitmaps[*g_resTelescopeMaskOuter].height;
        g_telescopeTileW = pBm[2] * 0xc00 / 2 >> 10;
        g_telescopeTileH = pBm[3] * 0xc00 / 2 >> 10;
        g_telescopeOriginX = (ScreenWidthU16() - g_telescopeTileW * 4) / 2;
        g_telescopeOriginY = (ScreenHeightU16() - g_telescopeTileH * 2) / 2;
        for (theI = 0; theI < 4; theI++) {
            g_telescopeMaskQuads[theI].SetFromBitmap(
                g_resTelescopeMaskOuter, g_telescopeOriginX + (g_telescopeTileW * 3 + 1) * (theI & 1),
                (s16)g_telescopeOriginY + (g_telescopeTileH + 1) * (theI >> 1), 0, 0, 0x600, 0x600);
            g_telescopeMaskQuads[theI].SetColor(0x808080);
            g_telescopeMaskQuads[theI].Mirror(theI & 1, theI >> 1);
        }
        for (theI = 0; theI < 4; theI++) {
            g_telescopeMaskQuads[theI + 4].SetFromBitmap(
                g_resTelescopeMaskInner, g_telescopeOriginX + g_telescopeTileW + (g_telescopeTileW + 1) * (theI & 1),
                (s16)g_telescopeOriginY + (g_telescopeTileH + 1) * (theI >> 1), 0, 0, 0x600, 0x600);
            g_telescopeMaskQuads[theI + 4].SetColor(0x808080);
            g_telescopeMaskQuads[theI + 4].Mirror(theI & 1, theI >> 1);
        }
    } else {
        pBm[0] = nBitmaps[*g_resTelescopeMaskOuter].u;
        pBm[1] = nBitmaps[*g_resTelescopeMaskOuter].v;
        pBm[2] = nBitmaps[*g_resTelescopeMaskOuter].width;
        pBm[3] = nBitmaps[*g_resTelescopeMaskOuter].height;
        g_telescopeTileW = pBm[2];
        g_telescopeTileH = pBm[3];
        g_telescopeOriginX = (ScreenWidthU16() - g_telescopeTileW * 4) / 2;
        g_telescopeOriginY = (ScreenHeightU16() - g_telescopeTileH * 2) / 2;
        for (theI = 0; theI < 4; theI++) {
            g_telescopeMaskQuads[theI].SetFromBitmap(
                g_resTelescopeMaskOuter, g_telescopeOriginX + (g_telescopeTileW * 3 + 1) * (theI & 1),
                (s16)g_telescopeOriginY + (g_telescopeTileH + 1) * (theI >> 1), 0, 0, 0x400, 0x400);
            g_telescopeMaskQuads[theI].SetColor(0x808080);
            g_telescopeMaskQuads[theI].Mirror(theI & 1, theI >> 1);
        }
        for (theI = 0; theI < 4; theI++) {
            g_telescopeMaskQuads[theI + 4].SetFromBitmap(
                g_resTelescopeMaskInner, g_telescopeOriginX + g_telescopeTileW + (g_telescopeTileW + 1) * (theI & 1),
                (s16)g_telescopeOriginY + (g_telescopeTileH + 1) * (theI >> 1), 0, 0, 0x400, 0x400);
            g_telescopeMaskQuads[theI + 4].SetColor(0x808080);
            g_telescopeMaskQuads[theI + 4].Mirror(theI & 1, theI >> 1);
        }
        nZ = nZ / 100.0f;
    }
    for (theI = 0; theI < 8; theI++)
        g_telescopeMaskQuads[theI].Draw(0xb);
    /* the four black bars around the mosaic: left, right, top, bottom */
    for (theI = 0; theI < 4; theI++) {
        pX0 = (theI != 0 ? g_telescopeOriginX : 0) + (theI == 1 ? g_telescopeTileW * 4 : 0);
        nY0 = theI == 3 ? (s16)g_telescopeOriginY + g_telescopeTileH * 2 : 0;
        curW = theI <= 1 ? g_telescopeOriginX : g_telescopeTileW * 4;
        curH = theI <= 1 ? ScreenHeightU16() : g_telescopeOriginY;
        Draw2D_FlatRect(nZ, HudElement::EdgeX(pX0), HudElement::EdgeY(nY0), HudElement::EdgeX(pX0 + curW),
                        HudElement::EdgeY(nY0 + curH), black, 0);
    }
}

/* the cannon-sight overlay (class CanonSimple): a 2x2 mosaic of the mask tile at 2x, mirrored, centred. */
void Hud_DrawCannonMask()
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreScreen);
    DavBitmapRec *theBitmaps;
    s32 curI;
    s16 bm[4]; /* {u, v, width, height} of the tile's bitmap */
    theBitmaps = g_pDav->header->dir->bitmaps;
    bm[0] = theBitmaps[*g_resCannonMask].u;
    bm[1] = theBitmaps[*g_resCannonMask].v;
    bm[2] = theBitmaps[*g_resCannonMask].width;
    bm[3] = theBitmaps[*g_resCannonMask].height;
    g_cannonTileW = (bm[2] << 11 >> 10) + 1;
    g_cannonTileH = (bm[3] << 11 >> 10) + 1;
    g_cannonOriginX = (ScreenWidthU16() - g_cannonTileW * 2) / 2;
    g_cannonOriginY = (ScreenHeightU16() - g_cannonTileH * 2) / 2;
    for (curI = 0; curI < 4; curI++) {
        g_cannonMaskQuads[curI].SetFromBitmap(g_resCannonMask, g_cannonOriginX + g_cannonTileW * (curI & 1),
                                              g_cannonOriginY + g_cannonTileH * (curI >> 1), 0, 0, 0x800, 0x800);
        g_cannonMaskQuads[curI].SetColor(0x808080);
        g_cannonMaskQuads[curI].Mirror(curI & 1, curI >> 1);
    }
    for (curI = 0; curI < 4; curI++)
        g_cannonMaskQuads[curI].Draw(0xb);
}

/* a flat rectangle in virtual HUD coordinates. */
void Ui_DrawFlatRect(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color)
{
    float left;
    float top;
    float z;
    u32 curBgr;
    float nBottom;
    float right;
    left = g_screen.ScaleX(x0);
    top = g_screen.ScaleY(y0);
    right = g_screen.ScaleX(x1);
    nBottom = g_screen.ScaleY(y1);
    curBgr = Color_RgbToBgr(color);
    z = g_screen.Draw2D_LayerToZ(layer);
    Draw2D_FlatRect(z, left, top, right, nBottom, curBgr, 0);
}

/* a four-colour gradient rectangle in virtual HUD coordinates. */
void Ui_DrawGouraudRect(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 c0, u32 c1, u32 c2, u32 c3)
{
    float left;
    float top;
    float curZ;
    u32 bgr1;
    u32 nBgr2;
    u32 fBgr3;
    float nBottom;
    u32 theBgr0;
    float theRight;
    left = g_screen.ScaleX(x0);
    top = g_screen.ScaleY(y0);
    theRight = g_screen.ScaleX(x1);
    nBottom = g_screen.ScaleY(y1);
    theBgr0 = Color_RgbToBgr(c0);
    bgr1 = Color_RgbToBgr(c1);
    nBgr2 = Color_RgbToBgr(c2);
    fBgr3 = Color_RgbToBgr(c3);
    curZ = g_screen.Draw2D_LayerToZ(layer);
    Draw2D_GouraudRect(curZ, left, top, theRight, nBottom, theBgr0, nBgr2, bgr1, fBgr3, 0);
}

/* a one-pixel outline just outside rect, as eight triangles; the pixel is at least 1.0 screen unit. */
void Ui_DrawRectOutline(s16 *rect, u32 color)
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreRect, rect);
    float left;
    float curZ;
    float top;
    u32 bgr;
    float pixelH;
    float myBottom;
    float curPixelW;
    float theRight;
    curPixelW = HudElement::SizeX(1);
    if (curPixelW < 1.0f)
        curPixelW = 1.0f;
    pixelH = HudElement::SizeY(1);
    if (pixelH < 1.0f)
        pixelH = 1.0f;
    left = g_screen.ScaleX(rect[0]);
    top = g_screen.ScaleY(rect[1]);
    theRight = g_screen.ScaleX(rect[0] + rect[2]);
    myBottom = g_screen.ScaleY(rect[1] + rect[3]);
    bgr = Color_RgbToBgr(color);
    curZ = g_screen.Draw2D_LayerToZ(g_screenLayerBase + 2);
    Draw2D_FlatTri(curZ, left - curPixelW, top - pixelH, theRight + curPixelW, top - pixelH, left, top, bgr, 0);
    Draw2D_FlatTri(curZ, theRight + curPixelW, top - pixelH, theRight, top, left, top, bgr, 0);
    Draw2D_FlatTri(curZ, left, myBottom, theRight, myBottom, left - curPixelW, myBottom + pixelH, bgr, 0);
    Draw2D_FlatTri(curZ, theRight, myBottom, theRight + curPixelW, myBottom + pixelH, left - curPixelW,
                   myBottom + pixelH, bgr, 0);
    Draw2D_FlatTri(curZ, left - curPixelW, myBottom + pixelH, left - curPixelW, top - pixelH, left, myBottom, bgr, 0);
    Draw2D_FlatTri(curZ, left - curPixelW, top - pixelH, left, top, left, myBottom, bgr, 0);
    Draw2D_FlatTri(curZ, theRight, myBottom, theRight, top, theRight + curPixelW, top - pixelH, bgr, 0);
    Draw2D_FlatTri(curZ, theRight, myBottom, theRight + curPixelW, top - pixelH, theRight + curPixelW,
                   myBottom + pixelH, bgr, 0);
}

#undef SetFromBitmap
#undef Mirror
#undef Draw
