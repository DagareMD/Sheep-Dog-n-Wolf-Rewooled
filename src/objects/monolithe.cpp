/* Source reconstruction; all six functions compare strictly exact. */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
class ScnObject;
class Camera;
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 opts);
void Camera_StartScripted(ScnObject *owner, Camera *cam, u16 rx, u16 ry, u16 rz, Vec3s *eye, u16 focal, u32 mode,
                          s32 time);
#include "../engine/sound_mgr.h"
#include "../app/app_main.h"
#include "../engine/scn_tools.h"
#include "world_draw.h"
#include "../engine/fixed_math.h"
#include "camera.h"
#include "../engine/time.h"

#define SDW_MEMBERS_ScnObject            \
    static void *operator new(uptr size); \
    void SetRotation(Vec3s *r);          \
    void StartCamera(u16 rx, u16 ry, u16 rz, Vec3s *eye, u16 focal, u32 mode);

#define SDW_MEMBERS_Monolithe                         \
    s16 &SourceRiderX(ScnObject **objects, u16 index) \
    {                                                 \
        return objects[index]->pos.x;                 \
    }
#include "sdw_classes.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_GETCLASSID 1
#define SDW_INLINE_SCNOBJECT_INSTFLAGS_U16 1
#define SDW_INLINE_SCNOBJECT_SETROTATION_VEC3S 1
#define SDW_INLINE_SCNOBJECT_STOPSOUND_U16 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETCLASSID
#undef SDW_INLINE_SCNOBJECT_INSTFLAGS_U16
#undef SDW_INLINE_SCNOBJECT_SETROTATION_VEC3S
#undef SDW_INLINE_SCNOBJECT_STOPSOUND_U16
#define SDW_INLINE_SCNOBJECT_STARTCAMERA_U16_U16_U16_VEC3S_U16_U32 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_STARTCAMERA_U16_U16_U16_VEC3S_U16_U32
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32

/* Fields in the class padding, with the instructions that prove them: 4d90d3/+64 u8 state; 4d9bcb/+65 u8 saved state;
 * 4d9116/+66 u16 rider count; 4d906c/+6a u16 ghost count; 4d92f2/+70 s32 freeze reply; 4d9073/+74 s32 freeze gate;
 * 4d9084..4d90a0/+78 Vec3s home rotation; 4d9ada..4d9aef/+7e Vec3s turn; 4d904a/+84 u16* camera; 4d9055/+88
 * ScnObject*[10]; 4d9033/+b4 CollBox*; 4d99c7/+c0 rider object with stride12, offsets +b8/+ba/+bc; 4d90a7/+130 u16 sound.
 */

s32 Scenaric_FindByClass(u16 classId, ScnObject **out, s32 max);
s32 ObjGrid_QueryBoxOverlap(CollBox *query, ScnObject **out);
s32 Vec3s_DistXZ(Vec3s *a, Vec3s *b);
u16 Sound_Play(u16 soundId, void *owner, u16 volume, u8 flags, s32 rate);
extern Wolf *g_pWolf;

#define SDW_INLINE_FREE_BOXCONTAINS_COLLBOX_VEC3S 1
#include "../engine/coll_box_inlines.h"
#undef SDW_INLINE_FREE_BOXCONTAINS_COLLBOX_VEC3S

#define SDW_INLINE_FREE_SOUNDISPLAYING_U16 1
#include "../engine/sound_mgr_inlines.h"
#undef SDW_INLINE_FREE_SOUNDISPLAYING_U16

/* Monolithe_PostLoadInit */
void Monolithe::PostLoadInit()
{
    void *rec = record;
    sheepBox = (CollBox *)Scn_GetPropBox(rec, 4);
    uiCamera = Scn_GetPropCamera(rec, 0);
    ghostCount = Scenaric_FindByClass(CLASSID_PRAYINGGHOST, ghosts, 10);
    freezeEnabled = 1;
    homeRot.x = rot.x;
    homeRot.y = rot.y;
    homeRot.z = rot.z;
    spinSound = 0;
    SetState(MONOLITH_ST_IDLE);
}

/* Monolithe_Update. Original fixed ten-element query storage is preserved. */
void Monolithe::Update()
{
    Vec3s p;
    Vec3s turn;
    ScnObject *list[10];
    Vec4i v;
    u16 i;
    Mat34s matrix;
    Vec3s rotation;
    switch (monoState) {
        case MONOLITH_ST_IDLE:
            riderCount = ObjGrid_QueryBoxOverlap(sheepBox, list);
            for (i = 0; i < riderCount; i++) {
                if (list[i]->GetClassId() == CLASSID_SHEEP && !list[i]->InstFlags(INST_F_ATTACHED))
                    list[i]->HandleMessage(this, MSG_SHEEP_SLEEP, 0);
            }
            break;
        case MONOLITH_ST_SPIN: {
            if (g_dt <= 0)
                break;
            const s32 duration = 16 * TIME_REFERENCE_STEP;
            const bool firstStep = spinTicks == 0;
            s32 elapsed = spinTicks + g_dt;
            if (elapsed > duration)
                elapsed = duration;
            s32 angle = elapsed * 0x80 / TIME_REFERENCE_STEP;
            s32 step = angle - spinRot.y;
            turn.x = rot.x;
            turn.y = rot.y;
            turn.z = rot.z;
            turn.y = (turn.y + step) & 0xfff;
            spinRot.y = (s16)angle;
            rot = turn;
            if (firstStep) {
                StartCamera(uiCamera->rot[0], uiCamera->rot[1], uiCamera->rot[2], &uiCamera->eye, uiCamera->focal,
                            CAMSCR_BLEND_OUT);
                if (freezeEnabled)
                    freezeAccepted = freezeTarget->HandleMessage(this, MSG_FREEZE, 0);
            }
            spinTicks = (u16)elapsed;
            for (i = 0; i < riderCount; i++) {
                if (riders[i].object) {
                    if (firstStep)
                        riders[i].object->HandleMessage(this, MSG_SCRIPT_HOLD, 0);
                    if (spinTicks == duration)
                        riders[i].object->HandleMessage(this, MSG_SCRIPT_RELEASE, 0);
                    v.x = riders[i].offset.x;
                    v.y = riders[i].offset.y;
                    v.z = riders[i].offset.z;
                    Mat34s_FromEulerScaled(&spinRot, &matrix, 0);
                    Mat34s_TransformVec3i(&matrix, (Vec3i *)&v, &v);
                    p.x = pos.x + (s16)v.x;
                    p.y = pos.y + (s16)v.y;
                    p.z = pos.z + (s16)v.z;
                    rotation.x = riders[i].object->rot.x;
                    rotation.y = riders[i].object->rot.y;
                    rotation.z = riders[i].object->rot.z;
                    rotation.y = rotation.y + step;
                    riders[i].object->SetRotation(&rotation);
                    riders[i].object->SetPosition(&p);
                }
            }
            if (spinTicks >= duration) {
                if (::SoundIsPlaying(spinSound))
                    StopSound(spinSound);
                SetState(MONOLITH_ST_SLEEP_SHEEP);
                if (freezeAccepted) {
                    freezeTarget->HandleMessage(this, MSG_UNFREEZE, 0);
                    freezeAccepted = 0;
                }
                Camera_ReleaseScripted(this);
            }
            if (BoxContains(sheepBox, &g_pWolf->pos))
                SetState(MONOLITH_ST_WOLF_ON);
            break;
        }
        case MONOLITH_ST_WOLF_ON:
            if (!BoxContains(sheepBox, &g_pWolf->pos))
                monoState = previousState;
            break;
        case MONOLITH_ST_SLEEP_SHEEP:
            riderCount = ObjGrid_QueryBoxOverlap(sheepBox, list);
            for (i = 0; i < riderCount; i++) {
                if (list[i]->GetClassId() == CLASSID_SHEEP && !list[i]->InstFlags(INST_F_ATTACHED))
                    list[i]->HandleMessage(this, MSG_SHEEP_SLEEP, 0);
            }
            break;
    }
}

/* Monolithe_SetState */
void Monolithe::SetState(u8 newState)
{
    ScnObject *list[10];
    u16 i;
    switch (newState) {
        case MONOLITH_ST_IDLE:
            PlayAnim(AMONOL01_ANIM_STAND, 0, 0);
            riderCount = 0;
            break;
        case MONOLITH_ST_SPIN:
            riderCount = ObjGrid_QueryBoxOverlap(sheepBox, list);
            for (i = 0; i < riderCount; i++) {
                if (list[i] != this && Vec3s_DistXZ(&list[i]->pos, &pos) < 230) {
                    if (list[i]->GetClassId() != CLASSID_WOLF) {
                        riders[i].object = list[i];
                        riders[i].offset.x = SourceRiderX(list, i);
                        riders[i].offset.y = list[i]->pos.y;
                        riders[i].offset.z = list[i]->pos.z;
                        riders[i].offset.x -= pos.x;
                        riders[i].offset.y -= pos.y;
                        riders[i].offset.z -= pos.z;
                    } else
                        SetState(MONOLITH_ST_WOLF_ON);
                } else
                    riders[i].object = 0;
            }
            spinRot.x = 0;
            spinRot.y = 0;
            spinRot.z = 0;
            spinTicks = 0;
            freezeAccepted = 0;
            for (i = 0; i < ghostCount; i++) {
                if (ghosts[i] && ghosts[i]->GetClassId() == CLASSID_PRAYINGGHOST)
                    ghosts[i]->HandleMessage(this, MSG_GHOST_NOTICE, freezeTarget);
            }
            spinSound = Sound_Play(SND_MONOLITH_SPIN, this, 0x3ff, SNDF_LOOP | SNDF_POSITIONAL, 0x1000);
            break;
        case MONOLITH_ST_WOLF_ON:
            previousState = monoState;
            break;
    }
    monoState = newState;
}

/* Monolithe_HandleMessage */
sptr Monolithe::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    ScnObject **p;
    s32 i;
    switch (msgId) {
        case MSG_SWITCH_ON:
            if (monoState == MONOLITH_ST_IDLE) {
                freezeTarget = g_pWolf;
                if (!BoxContains(sheepBox, &g_pWolf->pos) && !g_pWolf->HandleMessage(this, MSG_WOLF_IS_DEAD, 0)) {
                    p = (ScnObject **)arg;
                    for (i = 0; i < 4; i++) {
                        if (p[i]) {
                            if (p[i]->GetClassId() == CLASSID_ROBOT) {
                                freezeTarget = p[i];
                                break;
                            }
                        } else
                            break;
                    }
                    SetState(MONOLITH_ST_SPIN);
                }
            }
            break;
        case MSG_SWITCH_OFF:
            if (monoState == MONOLITH_ST_SLEEP_SHEEP)
                SetState(MONOLITH_ST_IDLE);
            break;
        case MSG_FREEZE:
            freezeAccepted = 0;
            freezeEnabled = 0;
            return 1;
    }
    return 0;
}

/* Monolithe_Reset */
void Monolithe::Reset()
{
    SetState(MONOLITH_ST_IDLE);
    rot = homeRot;
    freezeEnabled = 1;
}

/* Monolithe_Create */
ScnObject *Monolithe_Create(void *record)
{
    Monolithe *obj = new Monolithe;
    obj = (Monolithe *)obj->Init(record, 0);
    return obj;
}
