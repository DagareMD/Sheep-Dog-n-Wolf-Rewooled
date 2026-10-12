/*
 * Everything here is drawn in the 512x240 virtual HUD space (the PlayStation's screen) and scaled by Screen::ScaleX/Y.
 * The fade and letterbox curtains are a 4x4 ARGB4444 texture (a PolyBatcher texture page reserved for it, locked and
 * refilled every frame) stretched over the screen.
 */
#include "sdw_enums.h"
#include "../sdk/ddraw.h"
#define SDW_MEMBERS_Texture void Surface_LockForWrite(DDSURFACEDESC2 *desc);


#include "sdw_enums.h"
#include "scenaric_props.h"
#include "sdw_classes.h"
#define SDW_INLINE_CINE_ISACTIVE 1
#include "cine_inlines.h"
#undef SDW_INLINE_CINE_ISACTIVE
#define SDW_INLINE_UIQUAD_SETCOLOR_U32 1
#include "ui_quad_inlines.h"
#undef SDW_INLINE_UIQUAD_SETCOLOR_U32
#define SDW_INLINE_UIICON_SETCOLOR_U32 1
#include "ui_icon_inlines.h"
#undef SDW_INLINE_UIICON_SETCOLOR_U32

/* The DAV directory (DavHeader.dir): packed, its pointers sit at +6/+0xa/+0x12, which the struct generator cannot lay
 * out, so it is declared here. */
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

/* ---- callees ---- */
typedef void (*MenuHandler)(u8 msg, MenuPage *self);
#include "fixed_math.h"
#include "interface.h"
#include "draw2d.h"
#include "scn_tools.h"
#include "scenaric_loop.h"
#include "text.h"
#include "input.h"
#include "game_state.h"
#include "screen.h"
#include "cine.h"
#include "scenaric.h"
#include "time.h"
u32 Rgb24_Lerp(u32 a, u32 b, s16 t);
void Draw2D_TexRect(float z, float x0, float y0, float x1, float y1, s32 texIndex, float uTL, float vTL, u32 cTL,
                    float uBL, float vBL, u32 cBL, float uTR, float vTR, u32 cTR, float uBR, float vBR,
                    u32 cBR);
void Dialogue_SetBoxActive(s32 active);
u8 Dialogue_Say(const char *text, s32 voiceId, ScnObject *speaker, u32 arg);
s32 Rand_Bounded(s32 bound);
u16 Text_CountWrappedLines(const char *s);
void Menu_BuildList(MenuPage *pages, Menu *menu, s8 count, const MenuHandler *handlers);
void Ui_DrawTextBox(TextBox *box, u16 lineCount);
void Ui_DrawMenuBox(MenuBox *box);
uptr *Res_GetValidatedIdList(u16 resId, u16 *outCount);
u16 Sound_Play(u16 soundId, void *owner, u16 volume, u8 flags, s32 rate);

/* ---- globals ---- */
extern u32 *g_screenLayerBase;
extern Wolf *g_pWolf;
extern u32 g_gameFlags;
extern s32 g_dt;
extern s16 g_dtRawMs;
extern s32 g_gameTime;
extern s32 g_dialogueCurText;
extern u16 *g_resTelescopeMaskOuter;
extern u16 *g_resTelescopeMaskInner;
extern u16 *g_resCannonMask;

/* inline: the constant mask is substituted but its `~` is left to run time. */
#define SDW_INLINE_FREE_GAME_CLEARFLAGS_U32 1
#include "game_state_inlines.h"
#undef SDW_INLINE_FREE_GAME_CLEARFLAGS_U32
/* inline: the virtual screen size (Screen::virtWidth / virtHeight are u16). */
#define SDW_INLINE_FREE_SCREENWIDTHU16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENWIDTHU16
#define SDW_INLINE_FREE_SCREENHEIGHTU16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENHEIGHTU16

/* An ARGB8888 colour as ARGB4444. The last term's redundant 16-bit mask is the original's. */
#define ARGB4444(c) ((((c) >> 4) & 0xf) | (((c) >> 8) & 0xf0) | (((c) >> 12) & 0xf00) | (((c) >> 16) & 0xf000 & 0xffff))

/* ---- globals defined here ---- */
u16 g_uiSoundHandle = 0;       /* the last UI blip's voice */
s32 g_promptActive = 0;
TextBox g_promptBox = {0};
u32 g_promptMaxLineWidth = 0;
u32 g_promptLineCount = 0;
s32 g_promptResult = 0;
ScnObject *g_promptSender = 0;

/* open the global prompt box (freezing the Wolf with message 0x0e), sized to its text and centred. */
void Prompt_Begin(char *text, ScnObject *sender)
{
    g_promptSender = sender;
    if (g_pWolf)
        g_pWolf->HandleMessage(sender, MSG_FREEZE, 0);
    g_promptBox.text = text;
    Text_SetFont(FONT_GAME);
    g_promptMaxLineWidth = Text_MaxLineLength(g_promptBox.text);
    g_promptLineCount = Text_CountWrappedLines(g_promptBox.text);
    g_promptActive = 1;
    g_promptResult = PROMPT_PENDING;
    g_promptBox.rect[2] = (g_promptMaxLineWidth + 1) * g_pCurFont->glyphWidth;
    g_promptBox.rect[3] = (g_promptLineCount + 3) * g_pCurFont->lineHeight + 4;
    g_promptBox.rect[0] = (ScreenWidthU16() - g_promptBox.rect[2]) / 2;
    g_promptBox.rect[1] = (ScreenHeightU16() - g_promptBox.rect[3]) / 2;
    g_promptBox.bgColor = 0;
}

/* close the global prompt and unfreeze the Wolf (message 0x0f). */
void Prompt_End()
{
    if (g_promptActive) {
        if (g_pWolf)
            g_pWolf->HandleMessage(g_promptSender, MSG_UNFREEZE, 0);
        g_promptActive = 0;
    }
}

/* draw the global prompt; CROSS with the bars fully out answers 1 or 2. -1 while pending. */
s32 Prompt_Update()
{
    if (!g_promptActive)
        return g_promptResult;
    Ui_DrawTextBox(&g_promptBox, (u16)g_promptLineCount);
    if (g_promptBox.fits && Pad_MenuPressed((u16)~PAD_CROSS) && g_letterboxState == LETTERBOX_OPEN) {
        if (g_promptBox.confirmChoice == PROMPT_CHOICE_1)
            g_promptResult = PROMPT_CHOICE_1;
        else
            g_promptResult = PROMPT_CHOICE_2;
    }
    return g_promptResult;
}

/* open an NPC question box: the question is class string firstStringId, answer i and its reply are strings
 * firstStringId + 1 + 2i and + 2 + 2i; the box is sized to the widest line and centred. Freezes the Wolf. */
void Dialog_Begin(DialogBox *dlg, u32 firstStringId, s8 answerCount, const MenuHandler *handlers, ScnObject *sender)
{
    s32 curI;
    s32 theBase;
    u16 maxLen;
    dlg->active = 1;
    if (g_pWolf)
        g_pWolf->HandleMessage(sender, MSG_FREEZE, 0);
    dlg->inputLatch = 1;
    dlg->sender = sender;
    dlg->questionText = sender->Text_GetClassString((u8)firstStringId);
    dlg->drawBox.text = dlg->questionText;
    theBase = firstStringId + 1;
    for (curI = 0; curI < answerCount; curI++) {
        dlg->answerText[curI] = sender->Text_GetClassString((u8)(theBase + curI * 2));
        dlg->replyText[curI] = sender->Text_GetClassString((u8)(theBase + curI * 2 + 1));
    }
    Menu_BuildList(dlg->listA, &dlg->listB, answerCount, handlers);
    Text_SetWindow(g_screenLayerBase + 6, 0, 0, 512, 240, 0);
    Text_SetFont(FONT_GAME);
    maxLen = Text_MaxLineLength(dlg->drawBox.text);
    for (curI = 0; curI < answerCount; curI++)
        maxLen =
            maxLen < Text_MaxLineLength(dlg->answerText[curI]) ? Text_MaxLineLength(dlg->answerText[curI]) : maxLen;
    dlg->drawBox.rect[2] = maxLen * g_pCurFont->glyphWidth > 512 ? 512 : maxLen * g_pCurFont->glyphWidth;
    dlg->drawBox.rect[3] = 0;
    dlg->drawBox.rect[1] = 0;
    dlg->drawBox.rect[0] = 0;
    Text_SetWindowRect(g_screenLayerBase + 6, dlg->drawBox.rect, 0);
    dlg->drawBox.rect[3] =
        (Text_CountWrappedLines(dlg->drawBox.text) + dlg->listB.count + 2) * g_pCurFont->lineHeight > 240
            ? 240
            : (Text_CountWrappedLines(dlg->drawBox.text) + dlg->listB.count + 2) * g_pCurFont->lineHeight;
    dlg->drawBox.rect[0] = (ScreenWidthU16() - dlg->drawBox.rect[2]) / 2;
    dlg->drawBox.rect[1] = (ScreenHeightU16() - dlg->drawBox.rect[3]) / 2;
    dlg->drawBox.bgColor = 0;
    dlg->drawBox.pages = dlg->listA;
    dlg->drawBox.list = &dlg->listB;
}

void Dialog_Close(DialogBox *dlg)
{
    dlg->active = 0;
}

/* one frame of an open question box: TRIANGLE closes it (1); CROSS picks the highlighted answer (2, once per
 * press); else 0. */
u8 Dialog_Update(DialogBox *dlg)
{
    Dialogue_SetBoxActive(1);
    Ui_DrawMenuBox(&dlg->drawBox);
    if (dlg->drawBox.fits) {
        if (Pad_MenuPressed((u16)~PAD_TRIANGLE)) {
            Dialogue_SetBoxActive(0);
            if (g_pWolf)
                g_pWolf->HandleMessage(dlg->sender, MSG_UNFREEZE, 0);
            Dialog_Close(dlg);
            return DIALOG_CLOSED;
        }
        if (!dlg->inputLatch && Pad_MenuPressed((u16)~PAD_CROSS)) {
            dlg->selected = dlg->drawBox.cursorRow;
            dlg->inputLatch = 1;
            return DIALOG_PICKED;
        }
    }
    dlg->inputLatch = 0;
    return DIALOG_NONE;
}

/* play the chosen answer's reply (with its voice from answerVoices, if given); when it has finished, close
 * the box and unfreeze the Wolf (0). 1 while it is still being said. */
u32 Dialog_UpdateAnswer(DialogBox *dlg, const u32 *answerVoices)
{
    s32 voice;
    voice = 0;
    if (answerVoices)
        voice = answerVoices[dlg->selected];
    if (!Dialogue_Say(dlg->replyText[dlg->selected], voice, 0, 1)) {
        Dialogue_SetBoxActive(0);
        if (g_pWolf)
            g_pWolf->HandleMessage(dlg->sender, MSG_UNFREEZE, 0);
        Dialog_Close(dlg);
        return 0;
    }
    dlg->inputLatch = 0;
    return 1;
}

/* the prompt layout rects {x, y, w, h}. Their widths are written from the screen width by static initialisers, so every
 * member after the first computed one is set at run time as well. */
s16 g_rectDialogBody[4] = {0x30, 0x10, (s16)(ScreenWidthU16() - 0x60), 0x8c};
s16 g_rectChoiceLeft[4] = {0x20, 0xa0, (s16)((ScreenWidthU16() - 0x40) / 2), 0x28};
s16 g_rectChoiceRight[4] = {(s16)((ScreenWidthU16() - 0x40) / 2 + 0x20), 0xa0,
                            (s16)((ScreenWidthU16() - 0x40) / 2), 0x28};
s16 g_rectChoiceWide[4] = {0x20, 0xa0, (s16)(ScreenWidthU16() - 0x60), 0x28};
s16 g_rectFooter[4] = {0x30, 0xcc, (s16)(ScreenWidthU16() - 0x60), 0x14};

#define Setup UiIcon_Setup
#define SetFadeLevel UiIcon_SetFadeLevel
#define Draw UiIcon_Draw
/* the memory-card screens' backdrop bitmap, full width at 2x, faded to level. The frame holds a dword the code never
 * touches between the id-list count and the icon; `spare` stands for it. */
void Ui_DrawMemCardBackdrop(u8 level)
{
    HudElement hud(HudElement::Stretch);
    UiIcon icon;
    s32 spare;
    u16 count;
    void **list;
    u16 *kBmp;
    list = (void **)Res_GetValidatedIdList(DAV_IDI_IMEMCMAP, &count);
    if (list) {
        kBmp = (u16 *)*list;
        icon.Setup(kBmp, 0, 0, 0, 0, 0x800, 0x400, 0);
        icon.mainQuad.h = 0xf0;
        icon.SetColor(0x808080);
        icon.SetFadeLevel(level);
        icon.Draw(3);
    }
}
#undef Setup
#undef SetFadeLevel
#undef Draw

/* the scroll arrow, drawn at (x, y) at double height; flipMode turns it into the down arrow. */
void Ui_DrawScrollArrow(s16 x, s16 y, u16 flipMode)
{
    Sprite arrow;
    if (arrow.LoadFromRes(DAV_IDI_IGLFLEC_))
        arrow.Draw(g_screenLayerBase + 2, x, y, x + arrow.widthMinus1, y + arrow.height * 2, 0x808080, flipMode);
}

/* text clipped to rect, centred vertically on its wrapped line count. */
void Ui_DrawTextInRect(s16 *rect, u8 align, u32 style, char *text)
{
    Text_SetWindowRect(g_screenLayerBase + 2, rect, 1);
    Text_CenterVertically(Text_CountWrappedLines(text));
    Text_PrintfStyled(align, style, text);
    Hud_EndBox_stub();
}

void Ui_PlayMoveSound();
void Ui_PlayCancelSound();
void Ui_PlayConfirmSound();

/* title and one highlighted line; CROSS answers 0x10, TRIANGLE 0x80, else 0. */
u8 Ui_PromptConfirm(char *title, char *prompt)
{
    Text_SetColor(0x4bccff);
    Ui_DrawTextInRect(g_rectDialogBody, TEXTALIGN_CENTER, 0, title);
    Ui_DrawTextInRect(g_rectChoiceWide, TEXTALIGN_CENTER, 1, prompt);
    if (Pad_MenuPressed((u16)~PAD_CROSS)) {
        Ui_PlayConfirmSound();
        return MCARD_CUR_LEFT;
    }
    if (Pad_MenuPressed((u16)~PAD_TRIANGLE)) {
        Ui_PlayCancelSound();
        return MCARD_IN_CANCEL;
    }
    return MCARD_CUR_NONE;
}

/* title and two options side by side; LEFT / RIGHT toggles *choice between 0x10 and 0x20, CROSS answers
 * *choice, TRIANGLE 0x80, else 0. */
s8 Ui_PromptYesNo(char *title, char *optLeft, char *optRight, u8 *choice)
{
    Text_SetColor(0x4bccff);
    Ui_DrawTextInRect(g_rectDialogBody, TEXTALIGN_CENTER, 0, title);
    Ui_DrawTextInRect(g_rectChoiceLeft, TEXTALIGN_CENTER, *choice == MCARD_CUR_LEFT, optLeft);
    Ui_DrawTextInRect(g_rectChoiceRight, TEXTALIGN_CENTER, *choice == MCARD_CUR_RIGHT, optRight);
    if (Pad_MenuRepeat((u16)~PAD_LEFT) || Pad_MenuRepeat((u16)~PAD_RIGHT)) {
        if (*choice == MCARD_CUR_LEFT)
            *choice = MCARD_CUR_RIGHT;
        else
            *choice = MCARD_CUR_LEFT;
        Ui_PlayMoveSound();
    }
    if (Pad_MenuPressed((u16)~PAD_CROSS)) {
        Ui_PlayConfirmSound();
        return *choice;
    }
    if (Pad_MenuPressed((u16)~PAD_TRIANGLE)) {
        Ui_PlayCancelSound();
        return MCARD_IN_CANCEL;
    }
    return MCARD_CUR_NONE;
}

/* the cursor-move blip. */
void Ui_PlayMoveSound()
{
    g_uiSoundHandle = Sound_Play(SND_SGLCHGLI, 0, 0xff, 0, 0x1000);
}

/* the cancel / back sound. */
void Ui_PlayCancelSound()
{
    g_uiSoundHandle = Sound_Play(SND_SMOJPMOV, 0, 0xff, 0, 0x1000);
}

/* the confirm sound. */
void Ui_PlayConfirmSound()
{
    g_uiSoundHandle = Sound_Play(SND_SBONMBTN, 0, 0xff, 0, 0x1000);
}
