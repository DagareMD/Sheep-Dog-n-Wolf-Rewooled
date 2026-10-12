/*
 * DaffyTrainingLevel_Update drives it with three messages: 0x3781 (arg = the time, in g_dt ticks) shows it and sets the
 * time, 0x3782 (arg 1 / 0) starts / stops the countdown and its ticking loop, and 0x3780 asks for the time left (hiding
 * the watch once it is 0). While it runs, the watch model is drawn on the HUD through its own camera, the seconds left
 * are drawn as two digits next to a crayon frame, and the tick gets louder over the last 50 seconds. It has no designer
 * properties. Camera's two matrices are Mat44, whose out-of-line constructor the embedded camera calls (Watch_Create /
 * ).
 *
 * Camera's two matrices are Mat44 members, whose out-of-line constructor the embedded camera calls (Watch_Create
 *).
 * */

#define SDW_MEMBERS_ScnObject            \
    static void *operator new(uptr size); \
    void SetUpdateMode(u8 mode);

#define SDW_MEMBERS_Mat44 Mat44(); /* Mat44_Ctor, empty and out of line */
#include "sdw_types.h"
#include "../engine/screen.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 opts); /* (used by the PlayAnim inline) */
#include "sdw_classes.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_SETUPDATEMODE_U8 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_SETUPDATEMODE_U8
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32

#include "camera.h"
#include "../engine/sound_mgr.h"
#include "../engine/interface.h"
u16 Sound_Play(u16 soundId, void *owner, u16 volume, u8 flags, s32 rate);
extern s32 g_dt;
extern u32 *g_screenLayerBase; /* 2D draw layers: layer n is g_screenLayerBase + n */
extern u32 g_uiTintColor;

/* the class factory for CLASSID 103 "Watch": new Watch (the base vtables, the camera's two matrices, then
 * Watch's vtable), then ScnBody::Init(record, 0) through the vtable. */
ScnObject *Watch_Create(void *record)
{
    ScnBody *obj = new Watch;
    obj = obj->Init(record, 0);
    return obj;
}

/* 0 hides the watch (never updated, anim 0, countdown off), 1 shows it (always updated, anim 1 looping). */
void Watch::SetState(u8 newState)
{
    switch (newState) {
        case WATCH_ST_OFF:
            SetUpdateMode(SCN_UPD_NEVER);
            PlayAnim(ATIMER01_ANIM_DEFAULT, 0, 0);
            running = 0;
            break;
        case WATCH_ST_ON:
            SetUpdateMode(SCN_UPD_ALWAYS);
            PlayAnim(ATIMER01_ANIM_ON, 1, 0);
            break;
    }
    state = newState;
}

/* vtable +0x00: at the origin; the HUD camera 450 back looking along (0,0,0), its view matrix built once;
 * the digit sprite; hidden. */
void Watch::PostLoadInit()
{
    Vec3s zero;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    SetPosition(&zero);
    hudBufToggle = 0;
    hudCam.rot.y = 0;
    hudCam.rot.z = 0;
    hudCam.rot.x = 0;
    hudCam.dist = 450;
    Vec3s_OffsetAlongAngles(&hudCam.pos, &hudCam.rot, hudCam.dist, &zero);
    Camera_BuildViewMatrix(&hudCam, &zero);
    digits.InitFromRes(DAV_IDI_ITICPTR_);
    tickSound = 0;
    SetState(WATCH_ST_OFF);
}

/* vtable +0x14: forget the tick channel (without stopping it) and hide. */
void Watch::Reset()
{
    tickSound = 0;
    SetState(WATCH_ST_OFF);
}

/* vtable +0x04: while running, count down (clamped at 0), draw the watch on the HUD at (0x23, 0x20), the
 * two digits of the seconds left and the crayon frame, and set the tick volume; then the animation step. */
void Watch::Update()
{
    s16 vol;
    s16 xy[2];
    if (running) {
        timeLeft -= g_dt;
        if (timeLeft < 0)
            timeLeft = 0;
        xy[0] = 0x23;
        xy[1] = 0x20;
        {
            HudElement hud(HudElement::Start, HudElement::Start);
            RenderEx(&hudCam, hudBufToggle ? hudPrimBuf[0] : hudPrimBuf[1], 100, 0x180, xy);
            hudBufToggle = !hudBufToggle;
            digits.Draw(g_screenLayerBase + 8, 0x3f, 0x18, digits.width + 0x3f, digits.height + 0x18, 0x808080,
                        timeLeft / 0xa000 % 10, 0);
            digits.Draw(g_screenLayerBase + 8, 0x51, 0x18, digits.width + 0x51, digits.height + 0x18, 0x808080,
                        timeLeft / 0x1000 % 10, 0);
            g_spriteCrayon2.Draw(g_screenLayerBase + 9, 8, 0x10, 0x6b, 0x30, g_uiTintColor, 0);
        }
        if ((timeLeft * 1000 >> 12) < 50000)
            vol = 127 - (timeLeft * 1000 >> 12) * 127 / 50000;
        else
            vol = 0;
        Sound_SetVolume(tickSound, vol);
    }
    AdvanceAnim();
}

/* vtable +0x10: 0x3780 returns the time left (and hides the watch when it is 0); 0x3781 sets the time
 * (arg, g_dt ticks) and shows it; 0x3782 starts (arg 1: the ticking loop, sound 0x3f) or stops (arg 0) the countdown.
 * Returns 1 for the last two, 0 for any other message. */
sptr Watch::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    u16 handle;
    switch (msgId) {
        case MSG_WATCH_TIME_LEFT:
            if (timeLeft <= 0)
                SetState(WATCH_ST_OFF);
            return timeLeft;
        case MSG_WATCH_START:
            timeLeft = (s32)(sptr)arg;
            SetState(WATCH_ST_ON);
            return 1;
        case MSG_WATCH_TICK:
            running = (u32)(uptr)arg;
            if (running)
                tickSound = Sound_Play(SND_CLOCK_TICK, this, 0, SNDF_LOOP | SNDF_NO_RETRIGGER, 0x1000);
            else {
                handle = tickSound;
                Sound_Stop(handle, this);
            }
            return 1;
    }
    return 0;
}
