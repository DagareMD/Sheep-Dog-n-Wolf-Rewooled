#include "sdw_types.h"
#include "screen.h"
#include "sdw_enums.h"

#include "sdw_classes.h"

#include "text.h"
#include "input.h"

extern s32 g_dtMs;
extern u32 *g_screenLayerBase;

/* active-low pad words: one button down each */
u16 g_cheatSeqMystery[6] = {(u16)~PAD_SQUARE,   (u16)~PAD_SQUARE, (u16)~PAD_TRIANGLE,
                            (u16)~PAD_TRIANGLE, (u16)~PAD_CIRCLE, (u16)~PAD_CIRCLE};
u16 g_cheatSeqLevelSkip[7] = {(u16)~PAD_SQUARE,   (u16)~PAD_SQUARE, (u16)~PAD_TRIANGLE, (u16)~PAD_SQUARE,
                              (u16)~PAD_TRIANGLE, (u16)~PAD_CIRCLE, (u16)~PAD_CIRCLE};

s32 g_cheatSeqTimer = 0;
s32 g_cheatMysteryProgress = 0;
s32 g_cheatMysteryFlag = 0;
s32 g_cheatLevelSkipProgress = 0;
s32 g_cheatLevelSkip = 0;
s32 g_countdownMs = 0;

/* ----: the pad cheat sequences (OPTIMISED) ---- */
#pragma optimize("g", on)
/* advances *progress when this frame's new button word is the next one of seq; 1 once all len matched.
 * The inter-press timeout g_cheatSeqTimer is shared by both sequences and accumulated once per CALL. */
s32 Cheat_CheckSequence(s32 *progress, const u16 *seq, s32 len)
{
    if (g_cheatSeqTimer > 1500)
        *progress = 0;
    else
        g_cheatSeqTimer += g_dtMs;
    if (g_pad.IsConnected() && g_pad.cur.buttons != PAD_ALL_RELEASED && g_pad.prev.buttons != g_pad.cur.buttons) {
        if (*progress < len && seq[*progress] == g_pad.cur.buttons) {
            g_cheatSeqTimer = 0;
            if (++*progress >= len) {
                *progress = 0;
                return 1;
            }
        } else
            *progress = 0;
    }
    return 0;
}

/* called every frame by Main_Loop. */
void Cheat_Poll()
{
    if (Cheat_CheckSequence(&g_cheatMysteryProgress, g_cheatSeqMystery, 6))
        g_cheatMysteryFlag = 1;
    g_cheatLevelSkip = Cheat_CheckSequence(&g_cheatLevelSkipProgress, g_cheatSeqLevelSkip, 7);
}

s32 Cheat_IsLevelSkipRequested()
{
    return g_cheatLevelSkip;
}
#pragma optimize("", on)

/* ----: the countdown of a cut timed mode (no callers reach it) ---- */
s32 Countdown_Format(char *out)
{
    if (out) {
        if (g_countdownMs > 0)
            Text_Sprintf(out, "%d", g_countdownMs / 1000 % 60);
        else
            Text_Sprintf(out, "GAMEOVER!");
    }
    return g_countdownMs;
}

/* the tint it computes (towards red below 30 s) is never used. */
void Countdown_DrawHud()
{
    HudElement hud(HudElement::Start, HudElement::Start);
    char msg[16];
    s16 box[4];
    s32 color;
    box[0] = 0x50;
    box[1] = 0x28;
    box[3] = g_pCurFont->lineHeight;
    box[2] = g_pCurFont->glyphWidth << 4;
    color = Countdown_Format(msg);
    if (color < 0)
        color = 0;
    else if (g_countdownMs > 30000)
        color = -1;
    else
        color = ((u32)(color * 255) / 30000 << 8) + 0xff00ff;
    Text_SetWindow(g_screenLayerBase + 8, box[0], box[1], box[0] + box[2], box[1] + box[3], 1);
    Text_SetFont(FONT_GAME);
    Text_PrintFmt(msg);
    Hud_EndBox_stub();
}
