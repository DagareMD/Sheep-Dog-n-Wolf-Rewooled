#include "sdw_enums.h"
#include "timer.h"
#include "sdw_classes.h"

s32 g_frameCount = 0;         /* never read by the game                                  */
s32 g_dtRaw = 0;              /* unscaled frame delta, trunc(seconds * 4096)            */
u32 g_gameTime = 0;           /* u32: the type most of the game's declarations use (21 of 29) */
s32 g_gameTimeMs = 0;
s32 g_dtRawMs = 0;
u32 g_rawTime = 0;            /* u32 (the cinematic player compares it unsigned)       */
s32 g_timeScale = 0;          /* 4.12; only ever written as 0x1000                      */
Timer *g_pTimer = 0;          /* the gameplay clock's Timer, made by Time_Init          */
s32 g_dtMs = 0;
double g_timePausedDelta = 0; /* negated unconsumed timer delta, Time_Pause -> Time_Resume */
s32 g_renderWorldFlag =
    0; /* only ever written (1 here; 0/1 and, beside GF_RENDER_WORLD), never read by address */
u8 g_framesThisSecond = 0;
s32 g_dt = 0;
/* the port's: a fraction of a 4096th that changes every frame (by about 0.618 of the range, so it covers 0..4095
 * evenly), added before a per-frame step is truncated, so that a step under one unit still adds up over the frames
 * (Math_ApproachLinear) */
s32 g_dtDither = 0;
s32 g_rawTimeMs = 0;       /* the level time                                          */
s32 g_frameCount2 = 0;     /* only read by two HUD blink functions                   */

/* ---- elsewhere ---- */
extern u32 g_gameFlags; /* bit 0x40 = paused                                      */
#include "game_state.h"

/* on every level load (its one caller is Load_DAVnWAR). Replaces and stops the gameplay Timer (so the first
 * Time_Update delta is 0) and zeroes the clock. */
void Time_Init()
{
    delete g_pTimer;
    g_pTimer = new Timer;
    g_pTimer->Stop();
    g_timeScale = 0x1000;
    g_renderWorldFlag = 1;
    g_framesThisSecond = 0;
    g_dt = 0;
    g_dtRaw = 0;
    g_gameTime = 0;
    g_rawTime = 0;
    g_dtMs = 0;
    g_dtRawMs = 0;
    g_gameTimeMs = 0;
    g_rawTimeMs = 0;
}

/* no callers. */
void Time_RestartTimer()
{
    g_pTimer->Start();
}

/* saves the not-yet-consumed delta (negated) and stops the timer. Not the whole pause-menu path. */
void Time_Pause()
{
    g_timePausedDelta = -g_pTimer->GetDelta(TIMER_TICKS);
    g_pTimer->Stop();
}

/* restarts and shifts the base back by the saved amount, so the suspended interval is not counted. */
void Time_Resume()
{
    g_pTimer->Start();
    g_pTimer->OffsetBase(g_timePausedDelta);
}

#define SDW_ABS(v) ((v) >= 0 ? (v) : -(v))

/* called once per frame from Main_Loop. */
void Time_Update(void)
{
    s32 timer_enabled = 1;
    s32 new_time;
    /* The fixed-step alternative remains in the PC instructions. */
    if (timer_enabled)
        g_dtRaw = (s32)(g_pTimer->GetDelta(TIMER_SECONDS) * 4096.0);
    else
        g_dtRaw = 0x88;

    if (g_gameFlags & GF_PAUSED)
        g_dt = 0;
    else
        g_dt = (g_dtRaw * g_timeScale) >> 12;
    if (timer_enabled) {
        if (g_dt > 0xAA)
            g_dt = 0xAA;
        else if (g_dt < 0)
            g_dt = 0xAA;
    }

    g_dtDither = (g_dtDither + 2531) & 4095;
    g_animDt = (g_dt * 1000) >> 2;
    g_dtRawMs = (g_dtRaw * 1000) >> 12; /* truncated a second time */
    g_dtMs = (g_dt * 1000) >> 12;

    new_time = g_gameTime + g_dt;
    if ((g_gameTime & 0xFFFFF000) != (new_time & 0xFFFFF000))
        g_framesThisSecond = 0;
    else
        g_framesThisSecond++;

    g_gameTime = new_time;
    g_gameTimeMs += g_dtMs;
    g_frameCount2 += 1;
    g_rawTime += g_dtRaw;
    g_rawTimeMs += g_dtRawMs;
    g_frameCount += 1;
}
