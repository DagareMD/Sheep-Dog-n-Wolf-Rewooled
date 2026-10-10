
#include "sdw_types.h"
#include "../engine/screen.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 options);
#include "../engine/sound_mgr.h"
#include "../engine/maths.h"
#include "camera.h"
#include "../engine/interface.h"

#define SDW_MEMBERS_ScnObject            \
    static void *operator new(uptr size); \
    void SetUpdateMode(s32 mode);

#define SDW_MEMBERS_Mat44 Mat44();
#include "sdw_classes.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_ISSOUNDPLAYING_U16 1
#define SDW_INLINE_SCNOBJECT_STOPSOUND_U16 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_ISSOUNDPLAYING_U16
#undef SDW_INLINE_SCNOBJECT_STOPSOUND_U16
#define SDW_INLINE_SCNOBJECT_SETUPDATEMODE_S32 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_SETUPDATEMODE_S32
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32
extern Wolf *g_pWolf;
extern u32 *g_screenLayerBase;
u16 Sound_Play(u16 id, void *owner, u16 volume, u8 flags, s32 rate);
#define SDW_INLINE_FREE_SCREENHEIGHTS16 1
#include "../engine/screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENHEIGHTS16
#define ABS_VALUE(v) ((v) >= 0 ? (v) : -(v))

void Chronometer::PostLoadInit()
{
    Vec3s position;
    position.x = 0;
    position.y = 30000;
    position.z = 0;
    SetPosition(&position);
    bufToggle = 0;
    hudCam.rot.y = 0;
    hudCam.rot.z = 0;
    hudCam.rot.x = 0;
    hudCam.dist = 450;
    Vec3s_OffsetAlongAngles(&hudCam.pos, &hudCam.rot, hudCam.dist, &position);
    Camera_BuildViewMatrix(&hudCam, g_pZeroVec3s);
    slideOffset = 32;
    digitSprite.InitFromRes(DAV_IDI_ITICPTR_);
    PlayAnim(ATIMER01_ANIM_ON, 1, 0);
    SetUpdateMode(SCN_UPD_ALWAYS);
    onScreen = 0;
    panelColorTarget = 0xdd5050;
    panelColor = 0xdd5050;
    tickSoundHandle = 0;
    alarmSoundHandle = 0;
    SetMode(CHRONO_PARKED);
}
void Chronometer::Update()
{
    s16 xy[2];
    switch (mode) {
        case CHRONO_PARKED:
            if (g_pWolf->HandleMessage(this, MSG_WOLF_IS_DEAD, 0))
                SetMode(CHRONO_SLIDE_OUT);
            break;
        case CHRONO_SLIDE_IN:
            if (slideOffset <= 0 || g_pWolf->HandleMessage(this, MSG_WOLF_IS_DEAD, 0))
                SetMode(CHRONO_PARKED);
            else
                slideOffset -= 4;
            break;
        case CHRONO_SLIDE_OUT:
            if (slideOffset >= 32) {
                if (IsSoundPlaying(tickSoundHandle))
                    StopSound(tickSoundHandle);
                onScreen = 0;
                SetMode(CHRONO_PARKED);
            } else {
                slideOffset += 4;
                if (timeFixed == 0 && !IsSoundPlaying(alarmSoundHandle)) {
                    if (IsSoundPlaying(tickSoundHandle))
                        StopSound(tickSoundHandle);
                    alarmSoundHandle = Sound_Play(SND_CHRONO_ALARM, this, 255, SNDF_NO_RETRIGGER, 4096);
                }
            }
            break;
    }
    if (onScreen) {
        xy[0] = 37;
        xy[1] = ScreenHeightS16() + slideOffset - 16;
        {
            HudElement hud(HudElement::Start, HudElement::End);
            RenderEx(&hudCam, bufToggle ? unusedBufA : unusedBufB, 100, 0x180, xy);
            bufToggle = !bufToggle;
            digitSprite.Draw(g_screenLayerBase + 8, 65, ScreenHeightS16() - 8 - digitSprite.height + slideOffset,
                             digitSprite.width + 65, ScreenHeightS16() + slideOffset - 8, 0x808080,
                             (timeFixed / 40960) % 10, 0);
            digitSprite.Draw(g_screenLayerBase + 8, 83, ScreenHeightS16() - 8 - digitSprite.height + slideOffset,
                             digitSprite.width + 83, ScreenHeightS16() + slideOffset - 8, 0x808080, (timeFixed / 4096) % 10,
                             0);
            g_spriteCrayon2.Draw(g_screenLayerBase + 9, 10, ScreenHeightS16() + slideOffset - 32, 109,
                                 ScreenHeightS16() + slideOffset, panelColor, 0);
        }
        AdvanceAnim();
    }
}
sptr Chronometer::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    s32 unknownSlot;
    switch (msgId) {
        case MSG_CHRONO_SHOW_DEPARTURE:
        case MSG_CHRONO_SHOW_ARRIVAL:
            if (msgId == MSG_CHRONO_SHOW_DEPARTURE)
                panelColorTarget = 0x5050dd;
            else
                panelColorTarget = 0xdd5050;
            if (mode == CHRONO_PARKED) {
                if (timeFixed == 0 && onScreen == 1)
                    SetMode(CHRONO_SLIDE_OUT);
                else if (timeFixed != 0 && onScreen == 0) {
                    SetMode(CHRONO_SLIDE_IN);
                    timeFixed = (s32)(sptr)arg;
                } else if (panelColorTarget != panelColor && onScreen == 1)
                    SetMode(CHRONO_SLIDE_OUT);
                else if (panelColorTarget != panelColor && onScreen == 0) {
                    SetMode(CHRONO_SLIDE_IN);
                    timeFixed = (s32)(sptr)arg;
                } else if (onScreen) {
                    if (ABS_VALUE(timeFixed - (s32)(sptr)arg) < 4096)
                        timeFixed = (s32)(sptr)arg;
                    if ((timeFixed * 1000 >> 12) > 10000)
                        Sound_SetVolume(tickSoundHandle, 0);
                    else if (timeFixed) {
                        u16 volume = (u16)(63 - ((timeFixed << 12) / 1000 * 63) / 10000);
                        /* Cap the wrapped tick volume before SFX scaling, preserving the original tick timing. */
                        if (volume > 255)
                            volume = 255;
                        Sound_SetVolume(tickSoundHandle, volume);
                    } else
                        Sound_SetVolume(tickSoundHandle, 63);
                } else
                    timeFixed = (s32)(sptr)arg;
            } else if (mode == CHRONO_SLIDE_IN)
                timeFixed = (s32)(sptr)arg;
            break;
        case MSG_CHRONO_VISIBLE:
            if (!arg)
                SetMode(CHRONO_SLIDE_OUT);
            else {
                onScreen = 1;
                SetMode(CHRONO_SLIDE_IN);
            }
            break;
    }
    return 0;
}
void Chronometer::SetMode(u8 value)
{
    mode = value;
    if (mode == CHRONO_SLIDE_IN && !g_pWolf->HandleMessage(this, MSG_WOLF_IS_DEAD, 0)) {
        onScreen = 1;
        panelColor = panelColorTarget;
        if (IsSoundPlaying(tickSoundHandle))
            StopSound(tickSoundHandle);
        tickSoundHandle = Sound_Play(SND_CLOCK_TICK, this, 0, SNDF_NO_RETRIGGER, 4096);
    }
}
ScnObject *Chronometer_Create(u16 *record)
{
    Chronometer *object = new Chronometer;
    object = (Chronometer *)object->Init(record, 0);
    return object;
}
