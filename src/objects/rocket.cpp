#include "sdw_types.h"
#include "../engine/screen.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 opts);
#include "../engine/sound_mgr.h"
#include "../engine/interface.h"
#include "../engine/maths.h"
/* where a carried rocket sits on its carrier; const, main CONST ahead of the state table and the vtable */
const Vec3s g_rocketCarryOffset = {0, 10, 20};
inline s32 Scn_GetPropS32(u16 *props, u32 offset)
{
    s32 value = *(s32 *)((u8 *)props + offset + 0x14);
    return value;
}

#define SDW_MEMBERS_ScnObject                                                                      \
    static void *operator new(uptr size);                                                           \
    void AttachTo(ScnObject *parent, u8 joint, Vec3s *offset, Vec3s *rotation, u32 arg, uptr arg2); \
    void AttachTo(ScnObject *parent, u8 joint, const Vec3s *offset, Vec3s *rotation, u32 arg,      \
                  uptr arg2);                         \
    void AttachRocket(ScnObject *parent, u8 joint)                                                 \
    {                                                                                              \
        AttachTo(parent, joint, &g_rocketCarryOffset, 0, 0, 0);                                    \
    }                                                                                              \
    void SetRotation(Vec3s *rotation);                                                             \
    s32 GetPropertyS32(u32 offset)                                                                 \
    {                                                                                              \
        u16 *props = record;                                                                       \
        {                                                                                          \
            u32 field = offset;                                                                    \
            s32 number = *(s32 *)((u8 *)props + field + 0x14);    \
            return number;                                                                         \
        }                                                                                          \
    }


#include "sdw_classes.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_ATTACHTO_SCNOBJECT_U8_CONST_VEC3S_VEC3S_U32_U32 1
#define SDW_INLINE_SCNOBJECT_DROP_VEC3S 1
#define SDW_INLINE_SCNOBJECT_SETROTATION_VEC3S 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_ATTACHTO_SCNOBJECT_U8_CONST_VEC3S_VEC3S_U32_U32
#undef SDW_INLINE_SCNOBJECT_DROP_VEC3S
#undef SDW_INLINE_SCNOBJECT_SETROTATION_VEC3S
#define SDW_INLINE_SCNOBJECT_ISINWORLD 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_ISINWORLD
#define SDW_INLINE_SCNOBJECT_GETCLASSID 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETCLASSID
#define SDW_INLINE_SCNOBJECT_FACING 2
#define SDW_INLINE_SCNOBJECT_INSTFLAGS_U16 1
#define SDW_INLINE_SCNOBJECT_SETVISIBLE_S32 1
#define SDW_INLINE_SCNOBJECT_STOPSOUND_U16 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_FACING
#undef SDW_INLINE_SCNOBJECT_INSTFLAGS_U16
#undef SDW_INLINE_SCNOBJECT_SETVISIBLE_S32
#undef SDW_INLINE_SCNOBJECT_STOPSOUND_U16
#define SDW_INLINE_SCNOBJECT_SETSOUNDRATE_U16_S32 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_SETSOUNDRATE_U16_S32
#define SDW_INLINE_SCNBODY_ANIMFLAGS_U16 1
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_ANIMFLAGS_U16
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32
#define SDW_INLINE_SHADOW_SETENABLED_S32 1
#include "../engine/shadow_inlines.h"
#undef SDW_INLINE_SHADOW_SETENABLED_S32
#define SDW_INLINE_SPRITE_DRAWTHUNKAT_U32_S32_S32_U32_U32 1
#include "../engine/sprite_inlines.h"
#undef SDW_INLINE_SPRITE_DRAWTHUNKAT_U32_S32_S32_U32_U32
u16 Sound_Play(u16 id, void *owner, u16 volume, u8 flags, s32 rate);
/* anim id and flags (1 loop, 2 blend, 4 shadow) per state */
const u16 g_rocketStateAnims[][2] = {{AFUSEE01_ANIM_STAND, ROCKET_AF_SHADOW},
                                     {AFUSEE01_ANIM_STAND, 0},
                                     {AFUSEE01_ANIM_CRASH1, 0},
                                     {AFUSEE01_ANIM_STAND, 0},
                                     {AFUSEE01_ANIM_STAND, ROCKET_AF_SHADOW},
                                     {AFUSEE01_ANIM_ROCKET0, 0},
                                     {AFUSEE01_ANIM_ROCKET2, ROCKET_AF_BLEND},
                                     {AFUSEE01_ANIM_ROCKET3, ROCKET_AF_LOOP}};
extern u32 *g_screenLayerBase;
extern u32 g_gameTime;
extern s32 g_dt;
Sprite g_rocketFuelIconSprite;
Sprite g_rocketGaugeSprite;
Sprite g_rocketDamageIconSprite;
Rocket *g_pRocket = 0;

#define SDW_INLINE_FREE_SCREENWIDTHS32 1
#include "../engine/screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENWIDTHS32

void Rocket::SetState(u8 newState)
{
    struct {
        const u16 *entry;
        s32 loop, blend;
    } w;
    w.entry = g_rocketStateAnims[newState];
    state = newState;
    w.loop = w.entry[1] & ROCKET_AF_LOOP;
    w.blend = w.entry[1] & ROCKET_AF_BLEND;
    PlayAnim(w.entry[0], w.loop, w.blend);
    if (w.entry[1] & ROCKET_AF_SHADOW)
        shadow.SetEnabled(1);
    else
        shadow.SetEnabled(0);
    if ((u16)engineSoundHandle) {
        StopSound(engineSoundHandle);
        engineSoundHandle = 0;
    }
    enginePitch = 0x1000;
}

void Rocket::DrawGauges()
{
    HudElement hud(HudElement::End, HudElement::Start);
    struct {
        s32 amount, right, height, top, left;
    } w;
    w.top = 50;
    w.height = g_rocketGaugeSprite.height - 11;
    w.left = ScreenWidthS32() - 50;
    w.right = w.left + 25;
    g_rocketGaugeSprite.DrawThunkAt(g_screenLayerBase + 8, w.left - 1 - g_rocketGaugeSprite.widthMinus1 / 2, w.top - 6,
                                    0x808080, 0);
    g_rocketGaugeSprite.DrawThunkAt(g_screenLayerBase + 8, w.right - 1 - g_rocketGaugeSprite.widthMinus1 / 2, w.top - 6,
                                    0x808080, 8);
    g_rocketFuelIconSprite.DrawThunkAt(g_screenLayerBase + 8, w.left - g_rocketFuelIconSprite.widthMinus1 / 2,
                                       w.top - 6 - g_rocketFuelIconSprite.height - 2, 0x808080, 0);
    g_rocketDamageIconSprite.DrawThunkAt(g_screenLayerBase + 8, w.right - g_rocketDamageIconSprite.widthMinus1 / 2,
                                         w.top - 6 - g_rocketDamageIconSprite.height - 2, 0x808080, 0);
    w.top += w.height;
    w.amount = fuel * w.height / fuelMax;
    Ui_DrawFlatRect(g_screenLayerBase + 9, w.left - 4, w.top - w.amount, w.left + 4, w.top, fuelColor);
    w.amount = (hitsMax - hits) * w.height / hitsMax;
    Ui_DrawFlatRect(g_screenLayerBase + 9, w.right - 4, w.top - w.amount, w.right + 4, w.top, hitsColor);
}

void Rocket::Update()
{
    switch (state) {
        case ROCKET_CRASH:
            if (AnimFlags(ANIM_F_FINISHED)) {
                SetVisible(0);
                SetPosition(&homePos);
                SetRotation(g_pZeroVec3s);
                stateTime = g_gameTime;
                fuel = fuelMax;
                hits = hitsMax;
                SetState(ROCKET_RESPAWN_WAIT);
            }
            break;
        case ROCKET_RESPAWN_WAIT:
            if (g_gameTime - stateTime >= 0x3000) {
                SetVisible(1);
                stateTime = g_gameTime;
                SetState(ROCKET_RESPAWN_WOBBLE);
            }
            break;
        case ROCKET_RESPAWN_WOBBLE:
            if (IsRespawnWobbleDone(g_gameTime - stateTime))
                SetState(ROCKET_IDLE);
            break;
        case ROCKET_FLYING:
            hits = (g_dt >> 2) + hits;
            if (hits > hitsMax)
                hits = hitsMax;
            break;
    }
    AdvanceAnim();
}

sptr Rocket::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    struct {
        DropMsgArg *drop;
        u8 pad0[3], joint;
        ScnObject *parent;
        s32 step;
        Vec3s rotation;
        u16 pad1;
    } w;
    switch (msgId) {
        case MSG_QUERY_ACTION:
            if (sender->GetClassId() == CLASSID_WOLF && state == ROCKET_IDLE)
                return CTX_PICKUP;
            break;
        case MSG_HELD_STATE_BEGIN:
            if (!arg)
                SetState(ROCKET_ACTIVATE_A);
            else
                SetState(ROCKET_ACTIVATE_B);
            Sound_Play(SND_SFUTKFRK, this, 0xff, SNDF_POSITIONAL | SNDF_NO_RETRIGGER, 0x1000);
            active = 1;
            return 1;
        case MSG_HELD_STATE_END:
            if (InstFlags(INST_F_ATTACHED))
                SetState(ROCKET_HELD);
            else
                SetState(ROCKET_IDLE);
            active = 0;
            return 1;
        case MSG_CARRY_ANIM:
            if (active) {
                switch ((u32)(uptr)arg) {
                    case WOLF_CUE_JUMP:
                        SetState(ROCKET_ACTIVATE_B);
                        break;
                    case WOLF_CUE_WALK:
                        SetState(ROCKET_FLYING);
                        engineSoundHandle = Sound_Play(SND_SFUNTEUF, this, 0xff,
                                                       SNDF_LOOP | SNDF_POSITIONAL | SNDF_NO_RETRIGGER, 0x1000);
                        break;
                }
            }
            return 1;
        case MSG_PICKUP:
            w.parent = sender;
            w.joint = (u8)(uptr)arg;
            AttachTo(w.parent, w.joint, &g_rocketCarryOffset, 0, 0, 0);
            SetState(ROCKET_HELD);
            return 1;
        case MSG_DROP: {
            w.drop = (DropMsgArg *)arg;
            Detach();
            SetPosition(&w.drop->pos);
            if (w.drop->placed) {
                w.rotation.z = 0;
                w.rotation.y = sender->Facing();
                w.rotation.x = 0x400;
                rot = w.rotation;
                SetState(ROCKET_CRASH);
            } else {
                SetState(ROCKET_IDLE);
            }
        }
            return 1;
        case MSG_CONTAINER_STATE:
            switch ((u32)(uptr)arg) {
                case CONTAINER_RELEASED:
                    homePos = sender->pos;
                    break;
            }
            return 1;
        case MSG_INVENTORY_STORED:
        case MSG_CHECKPOINT_ROLLBACK:
            Reset();
            return 1;
        case MSG_QUERY_HELD_ACTION:
            return HELD_ROCKET;
        case MSG_ROCKET_BURN_FUEL:
            if ((u16)engineSoundHandle) {
                w.step = (g_dt << 12) / 0x2000;
                if ((s32)(sptr)arg > g_dt)
                    enginePitch += w.step;
                else
                    enginePitch -= w.step;
                if (enginePitch > 0x2000)
                    enginePitch = 0x2000;
                else if (enginePitch < 0x1000)
                    enginePitch = 0x1000;
                SetSoundRate(engineSoundHandle, (u16)enginePitch);
            }
            fuel -= (s32)(sptr)arg;
            if (fuel <= 0) {
                fuel = 0;
                return 0;
            }
            return 1;
        case MSG_ROCKET_DAMAGE:
            hits -= (s32)(sptr)arg;
            if (hits <= 0) {
                hits = 0;
                return 0;
            }
            return 1;
    }
    return 0;
}

void Rocket::Render(Camera *view)
{
    if (state == ROCKET_RESPAWN_WOBBLE)
        RenderRespawnWobble(view, g_gameTime - stateTime);
    else
        ScnMobile::Render(view);
    if (state == ROCKET_FLYING)
        DrawGauges();
}

void Rocket::Reset()
{
    if (IsInWorld()) {
        SetVisible(1);
        SetPosition(&homePos);
        SetRotation(g_pZeroVec3s);
        SetState(ROCKET_IDLE);
    }
    fuel = fuelMax;
    hits = hitsMax;
    active = 0;
}

void Rocket::PostLoadInit()
{
    homePos = pos;
    stateTime = 0;
    fuelMax = GetPropertyS32(0) << 12;
    hitsMax = ((GetPropertyS32(8) * 2) << 12) / 100;
    fuelColor = GetPropertyS32(4);
    hitsColor = GetPropertyS32(12);
    fuel = fuelMax;
    hits = hitsMax;
    g_rocketGaugeSprite.LoadFromRes(DAV_IDI_IFUJAUG_);
    g_rocketFuelIconSprite.LoadFromRes(DAV_IDI_IFUJIC1_);
    g_rocketDamageIconSprite.LoadFromRes(DAV_IDI_IFUJIC2_);
    engineSoundHandle = 0;
    SetState(ROCKET_IDLE);
    PlayAnim(g_rocketStateAnims[0][0], 0, 0);
    shadow.radius = 20;
    active = 0;
}

ScnObject *Rocket_Create(void *record)
{
    Rocket *obj = new Rocket;
    obj = (Rocket *)obj->Init(record, 0);
    g_pRocket = obj;
    return obj;
}
