#ifndef SDW_ENGINE_TIME_H
#define SDW_ENGINE_TIME_H

/* The functions and globals time.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

class Timer;

extern s32 g_dtRaw;
extern s32 g_dt;
extern u8 g_framesThisSecond;
extern Timer *g_pTimer;
extern u32 g_rawTime;
extern s32 g_rawTimeMs;
extern s32 g_renderWorldFlag;
void Time_Init();
void Time_Pause();
void Time_Resume();
void Time_Update();

// The fixed game step retained in Time_Update: about 30 updates per second.
enum { TIME_REFERENCE_STEP = 0x88 };

// Convert an amount per reference step to this update, retaining fractional units.
// Each independently changing value owns its remainder, reset when its motion starts.
inline s32 Time_ScaleStep(s32 step, s32 &remainder)
{
    s64 total = (s64)step * g_dt + remainder;
    remainder = (s32)(total % TIME_REFERENCE_STEP);
    return (s32)(total / TIME_REFERENCE_STEP);
}

#endif
