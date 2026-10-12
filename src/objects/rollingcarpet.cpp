/* RollingCarpet (class 115),: the rolling belt that a rider drives, with its RCarpetMobile targets. */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
#include "../sdk/ddraw.h"
#include "../sdk/crt.h"
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 opts);
#include "../engine/sound_mgr.h"
#include "../engine/scn_tools.h"
#include "../engine/collide.h"
#include "camera.h"
#include "../engine/id_list.h"
#include "../app/app_main.h"
#include "../engine/game_state.h"
#include "../engine/draw2d.h"

#define SDW_MEMBERS_ScnObject                      \
    static void *operator new(uptr size);           \
    void SetRotation(Vec3s *rotation);             \
    CollBox *GetFirstModelBox();                   \
    void GetModelBoxes(CollBox **out, u32 *count); \
    void StartCamera(u16 x, u16 y, u16 z, Vec3s *position, u16 focal, u32 mode, s32 duration);

#define SDW_MEMBERS_CollBox                                                                      \
    s32 ContainsPointXZ(Vec3s *point) /* inline */                                 \
    {                                                                                            \
        return point->x >= min.x && point->x <= max.x && point->z >= min.z && point->z <= max.z; \
    }
#define SDW_MEMBERS_Texture                                               \
    Texture(D3DApp *app, u32 width, u32 height, u32 format, s32 *result); \
    void Surface_LockForRead(DDSURFACEDESC2 *desc);                       \
    long Surface_LockForWrite(DDSURFACEDESC2 *desc);
#include "sdw_classes.h"
#include "../engine/tex_scroll.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_GETCLASSID 1
#define SDW_INLINE_SCNOBJECT_GETFACING 1
#define SDW_INLINE_SCNOBJECT_INSTFLAGS_U16 1
#define SDW_INLINE_SCNOBJECT_SETROTATION_VEC3S 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETCLASSID
#undef SDW_INLINE_SCNOBJECT_GETFACING
#undef SDW_INLINE_SCNOBJECT_INSTFLAGS_U16
#undef SDW_INLINE_SCNOBJECT_SETROTATION_VEC3S
#define SDW_INLINE_SCNOBJECT_ENABLEBOXCOLLIDE_S32 1
#define SDW_INLINE_SCNOBJECT_STOPSOUND_U16 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_ENABLEBOXCOLLIDE_S32
#undef SDW_INLINE_SCNOBJECT_STOPSOUND_U16
#define SDW_INLINE_SCNOBJECT_ISSOUNDPLAYING_U16 1
#define SDW_INLINE_SCNOBJECT_SETSOUNDRATE_U16_S32 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_ISSOUNDPLAYING_U16
#undef SDW_INLINE_SCNOBJECT_SETSOUNDRATE_U16_S32
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32

/* The packed DAV directory, as in src/engine/load_dav.cpp. */
#include "sdw_fileptr.h"
#pragma pack(push, 1)
struct DavDirectory {
    u16 indexCount, bitmapCount, unk04;
    SDW_DAVPTR(u16) indices;
    SDW_DAVPTR(DavBitmapRec) bitmaps;
    u32 fileSize;
    SDW_DAVPTR(u32) idLists;
};
#pragma pack(pop)

s32 Box_GroundQueryFlatTop(GroundQuery *query, CollBox *box, Vec3s *position, s32 margin);
s32 ObjGrid_QueryBoxOverlap(CollBox *box, ScnObject **out);
void Camera_StartScripted(ScnObject *owner, Camera *cam, u16 x, u16 y, u16 z, Vec3s *position, u16 focal, u32 mode,
                          s32 duration);
u16 Sound_Play(u16 id, void *owner, u16 volume, u8 flags, s32 rate);
extern s32 g_dtMs;
struct RollingCarpetBeltState {
    Texture *backup;
    TexScroll scroll;
    s32 ready;
};
RollingCarpetBeltState g_rollingCarpetBelt;
#define g_rollingCarpetBeltBackup g_rollingCarpetBelt.backup
#define g_rollingCarpetBeltScroll g_rollingCarpetBelt.scroll
#define g_rollingCarpetBeltReady g_rollingCarpetBelt.ready

inline void Scn_GetPropU32(void *props, u32 offset, u32 *out)
{
    *out = *(u32 *)((u8 *)props + offset + 0x14);
}
#define SDW_INLINE_FREE_SCN_GETPROPS32_VOID_U32 1
#include "../engine/scn_tools_inlines.h"
#undef SDW_INLINE_FREE_SCN_GETPROPS32_VOID_U32
#define SDW_INLINE_SCNOBJECT_GETMODELBOXES_COLLBOX_U32 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETMODELBOXES_COLLBOX_U32
inline CollBox *ScnObject::GetFirstModelBox()
{
    ModelBoxList *list = inst_model->boxes;
    if (list)
        return list->boxes;
    return 0;
}
#define SDW_INLINE_SCNOBJECT_STARTCAMERA_U16_U16_U16_VEC3S_U16_U32_S32 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_STARTCAMERA_U16_U16_U16_VEC3S_U16_U32_S32

s32 RollingCarpet::CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY,
                                 CollContact *contacts, s32 *nContacts, u32 mode)
{
    return Collide_BoxVsObjBox(this, mover, disp, collBox, &pos, outFrac, outY, contacts, nContacts);
}

void RollingCarpet::PostLoadInit()
{
    struct {
        u32 mode;
        u16 pad, heading;
        CollBox *boxes;
        u32 count, index;
        u16 *props;
    } w;
    w.props = record;
    camera = Scn_GetPropCamera(w.props, 0);
    targets[0] = Scn_GetPropObject(w.props, 0xc);
    targets[1] = Scn_GetPropObject(w.props, 0x10);
    targets[2] = Scn_GetPropObject(w.props, 0x14);
    targets[3] = Scn_GetPropObject(w.props, 0x18);
    targets[4] = Scn_GetPropObject(w.props, 0x1c);
    Scn_GetPropU32(w.props, 4, &w.mode);
    if (targets[0])
        targets[0]->HandleMessage(this, MSG_CARPET_SET_MODE, (void *)(sptr)w.mode);
    if (targets[1])
        targets[1]->HandleMessage(this, MSG_CARPET_SET_MODE, (void *)(sptr)w.mode);
    if (targets[2])
        targets[2]->HandleMessage(this, MSG_CARPET_SET_MODE, (void *)(sptr)w.mode);
    if (targets[3])
        targets[3]->HandleMessage(this, MSG_CARPET_SET_MODE, (void *)(sptr)w.mode);
    if (targets[4])
        targets[4]->HandleMessage(this, MSG_CARPET_SET_MODE, (void *)(sptr)w.mode);
    ratio = Scn_GetPropS32(w.props, 8);
    ratio <<= 12;
    ratio /= 10;
    GetModelBoxes(&w.boxes, &w.count);
    topBox.Box_Translate(GetFirstModelBox(), &pos);
    topBox.max.y = topBox.min.y;
    topBox.min.y -= 50;
    topBox.max.z -= 150;
    topBox.min.z += 150;
    topBox.max.x -= 50;
    topBox.min.x += 50;
    center.x = (topBox.min.x + topBox.max.x) >> 1;
    center.z = (topBox.min.z + topBox.max.z) >> 1;
    rollRot.x = rot.x;
    rollRot.y = rot.y;
    rollRot.z = rot.z;
    w.heading = rollRot.y & 0xfff;
    if (w.heading < 0x200)
        axis = RC_AXIS_Z;
    else if (w.heading < 0x600)
        axis = RC_AXIS_X;
    else if (w.heading < 0xa00)
        axis = RC_AXIS_Z;
    else if (w.heading < 0xe00)
        axis = RC_AXIS_X;
    else
        axis = RC_AXIS_Z;
    collBox = 0;
    for (w.index = 0; w.index < w.count; w.index++) {
        if (axis == RC_AXIS_Z) {
            if (!(w.boxes[w.index].flags & COLLBOX_DOOR_SET))
                collBox = &w.boxes[w.index];
        } else if (w.boxes[w.index].flags & COLLBOX_DOOR_SET) {
            collBox = &w.boxes[w.index];
        }
    }
    EnableBoxCollide(0);
    if (!g_rollingCarpetBeltReady) {
        InitBeltScroll(10);
        g_rollingCarpetBeltReady = 1;
    }
    rcFlags.camera = 0;
    rcFlags.registered = 0;
    SetState(RC_ST_IDLE);
}

void RollingCarpet::Reset()
{
    if (rcFlags.registered) {
        SetState(RC_ST_RELEASE);
        timerMs = 0;
    } else
        SetState(RC_ST_IDLE);
    motorSound = 0;
}

ScnObject *RollingCarpet::FindRider()
{
    struct {
        u16 pad, index;
        ScnObject *selected;
        ScnObject *found[65];
        s32 count;
    } w;
    w.selected = 0;
    w.count = ObjGrid_QueryBoxOverlap(&topBox, w.found);
    if (w.count != 0 && w.count < 3) {
        for (w.index = 0; w.index < w.count; w.index++) {
            if (!w.found[w.index]->InstFlags(INST_F_ATTACHED) && w.found[w.index] != this)
                w.selected = w.found[w.index];
        }
    }
    return w.selected;
}

void RollingCarpet::FaceRider()
{
    struct {
        s32 unused, forward;
        u16 pad;
        s16 heading;
    } w;
    w.unused = 0;
    w.heading = rider->GetFacing();
    switch (axis) {
        case RC_AXIS_Z:
            if (w.heading < 0x400) {
                rollRot.y = 0x800;
                w.forward = 0;
            } else if (w.heading < 0xc00) {
                rollRot.y = 0;
                w.forward = 1;
            } else {
                rollRot.y = 0x800;
                w.forward = 0;
            }
            break;
        case RC_AXIS_X:
            if (w.heading < 0x800) {
                rollRot.y = 0xc00;
                w.forward = 0;
            } else {
                rollRot.y = 0x400;
                w.forward = 1;
            }
            break;
    }
    if (rot.y != rollRot.y)
        SetRotation(&rollRot);
}

void RollingCarpet::Update()
{
    s32 speed;
    switch (state) {
        case RC_ST_IDLE:
            rider = FindRider();
            if (rider)
                SetState(RC_ST_REGISTER);
            riderDelta = 0;
            riderSpeed = 0;
            rcFlags.camera = 0;
            break;
        case RC_ST_REGISTER:
            SetState(RC_ST_RIDDEN);
            break;
        case RC_ST_RELEASE:
            timerMs -= g_dtMs;
            if (timerMs <= 0)
                SetState(RC_ST_IDLE);
            break;
        case RC_ST_RIDDEN:
            rider->HandleMessage(this, MSG_AXIS_LOCK, (void *)(uptr)axis);
            if (rider->HandleMessage(this, MSG_QUERY_CONTROLLED, 0)) {
                if (!rcFlags.camera && camera) {
                    rcFlags.camera = 1;
                    StartCamera(camera->rot[0], camera->rot[1], camera->rot[2], &camera->eye, camera->focal,
                                CAMSCR_BLEND_IN | CAMSCR_BLEND_OUT, 0x1000);
                }
            } else if (rcFlags.camera) {
                rcFlags.camera = 0;
                Camera_ReleaseScripted(this);
            }
            if (rider->GetClassId() == CLASSID_WOLF) {
                if ((riderSpeed >= 0 ? riderSpeed : -riderSpeed) >= 250)
                    SetState(RC_ST_ROLLING);
            } else if ((riderSpeed >= 0 ? riderSpeed : -riderSpeed) >= 150)
                SetState(RC_ST_ROLLING);
            if (!topBox.ContainsPointXZ(&rider->pos) || rider->InstFlags(INST_F_ATTACHED))
                SetState(RC_ST_RELEASE);
            break;
        case RC_ST_ROLLING:
            if (rider->GetClassId() == CLASSID_WOLF) {
                if ((riderSpeed >= 0 ? riderSpeed : -riderSpeed) < 250)
                    SetState(RC_ST_RIDDEN);
            } else if ((riderSpeed >= 0 ? riderSpeed : -riderSpeed) < 150)
                SetState(RC_ST_RIDDEN);
            if ((riderSpeed >= 0 ? riderSpeed : -riderSpeed) >= 700)
                SetState(RC_ST_ROLLING_FAST);
            rider->HandleMessage(this, MSG_AXIS_LOCK, (void *)(uptr)axis);
            if (rider->InstFlags(INST_F_ATTACHED))
                SetState(RC_ST_RELEASE);
            if (riderDelta >= 0)
                speed = riderSpeed >= 0 ? riderSpeed : -riderSpeed;
            else
                speed = -(riderSpeed >= 0 ? riderSpeed : -riderSpeed);
            if (targets[0])
                targets[0]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[1])
                targets[1]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[2])
                targets[2]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[3])
                targets[3]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[4])
                targets[4]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (rider->GetClassId() == CLASSID_WOLF)
                ScrollBelt(riderDelta / 3 >= 0 ? riderDelta / 3 : -(riderDelta / 3));
            else
                ScrollBelt(riderDelta >= 0 ? riderDelta : -riderDelta);
            break;
        case RC_ST_ROLLING_FAST:
            if ((riderSpeed >= 0 ? riderSpeed : -riderSpeed) < 700)
                SetState(RC_ST_ROLLING);
            timerMs -= g_dtMs;
            if (timerMs <= 0)
                SetState(RC_ST_RELEASE);
            rider->HandleMessage(this, MSG_AXIS_LOCK, (void *)(uptr)axis);
            if (riderDelta >= 0)
                speed = riderSpeed >= 0 ? riderSpeed : -riderSpeed;
            else
                speed = -(riderSpeed >= 0 ? riderSpeed : -riderSpeed);
            if (targets[0])
                targets[0]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[1])
                targets[1]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[2])
                targets[2]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[3])
                targets[3]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (targets[4])
                targets[4]->HandleMessage(this, MSG_CARPET_DRIVE, &speed);
            if (rider->GetClassId() == CLASSID_WOLF)
                ScrollBelt(riderDelta / 3 >= 0 ? riderDelta / 3 : -(riderDelta / 3));
            else
                ScrollBelt(riderDelta >= 0 ? riderDelta : -riderDelta);
            break;
    }
    AdvanceAnim();
}

sptr RollingCarpet::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    struct {
        MoveModifyArg *move;
        Vec3s delta;
        u16 pad;
    } w;
    if (sender) {
        switch (msgId) {
            case MSG_MODIFY_MOVE:
                w.move = (MoveModifyArg *)arg;
                switch (axis) {
                    case RC_AXIS_X:
                        riderDelta = w.move->delta.x;
                        if (riderDelta)
                            riderSpeed = w.move->velocity.x;
                        else
                            riderSpeed = 0;
                        break;
                    case RC_AXIS_Z:
                        riderDelta = w.move->delta.z;
                        if (riderDelta)
                            riderSpeed = w.move->velocity.z;
                        else
                            riderSpeed = 0;
                        break;
                }
                FaceRider();
                w.delta.x = center.x;
                w.delta.y = center.y;
                w.delta.z = center.z;
                w.delta.x -= rider->pos.x;
                w.delta.y -= rider->pos.y;
                w.delta.z -= rider->pos.z;
                w.move->delta.x = (w.delta.x >= 0 ? w.delta.x : -w.delta.x) <= 4 ? w.delta.x : (s16)(w.delta.x >> 2);
                w.move->delta.z = (w.delta.z >= 0 ? w.delta.z : -w.delta.z) <= 4 ? w.delta.z : (s16)(w.delta.z >> 2);
                switch (state) {
                    case RC_ST_RIDDEN:
                    case RC_ST_ROLLING:
                    case RC_ST_ROLLING_FAST:
                        if (w.move->delta.y < -10)
                            SetState(RC_ST_RELEASE);
                        break;
                }
                break;
            case MSG_GROUND_QUERY:
                if (collBox)
                    return Box_GroundQueryFlatTop((GroundQuery *)arg, collBox, &pos, 10);
                break;
        }
    }
    return 0;
}

void RollingCarpet::SetState(u8 newState)
{
    switch (newState) {
        case RC_ST_REGISTER:
            rider->HandleMessage(this, MSG_RIDER_ADD, 0);
            rcFlags.registered = 1;
            break;
        case RC_ST_RELEASE:
            rider->HandleMessage(this, MSG_RIDER_REMOVE, 0);
            rcFlags.registered = 0;
            if (rcFlags.camera) {
                rcFlags.camera = 0;
                Camera_ReleaseScripted(this);
            }
            timerMs = 0x400;
            if (rider && rider->InstFlags(INST_F_ATTACHED))
                riderSpeed = 0;
            break;
        case RC_ST_IDLE:
            PlayAnim(ATAPIS01_ANIM_STAND, 0, 0);
            StopSound(motorSound);
            motorSound = 0;
            riderSpeed = 0;
            break;
        case RC_ST_RIDDEN:
            PlayAnim(ATAPIS01_ANIM_STAND, 0, 0);
            StopSound(motorSound);
            motorSound = 0;
            riderSpeed = 0;
            break;
        case RC_ST_ROLLING:
            if (rider->GetClassId() == CLASSID_WOLF)
                PlayAnim(ATAPIS01_ANIM_RUN, 1, 0);
            if (rider->GetClassId() == CLASSID_SHEEP)
                PlayAnim(ATAPIS01_ANIM_WALK, 1, 0);
            if (!IsSoundPlaying(motorSound))
                motorSound = Sound_Play(SND_CARPET_MOTOR, this, 0xff, SNDF_LOOP | SNDF_POSITIONAL, 0x1000);
            SetSoundRate(motorSound, 0x1000);
            break;
        case RC_ST_ROLLING_FAST:
            if (rider->GetClassId() == CLASSID_WOLF)
                PlayAnim(ATAPIS01_ANIM_RUNF, 1, 0);
            timerMs = 0x800;
            SetSoundRate(motorSound, 0x2000);
            break;
    }
    state = newState;
}

ScnObject *RollingCarpet_Create(void *record)
{
    RollingCarpet *obj = new RollingCarpet;
    obj = (RollingCarpet *)obj->Init(record, 0);
    obj->flags |= SCN_OF_CUSTOM_COLLIDE;
    obj->collBox = 0;
    g_rollingCarpetBeltReady = 0;
    return obj;
}

void RollingCarpet::InitBeltScroll(s8 step)
{
    uptr *entries;
    u16 count;
    entries = IdList_FindWithCount(DAV_IDI_ITPDESS_, &count);
    delete g_rollingCarpetBeltBackup;
    g_rollingCarpetBeltBackup =
        TexScroll_Setup(&g_rollingCarpetBeltScroll, &g_pDav->header->dir->bitmaps[*(u16 *)entries[0]], step);
}

/* The callers pass a short; only its low signed byte is stored. */
void RollingCarpet::ScrollBelt(s16 rows)
{
    g_rollingCarpetBeltScroll.step = (s8)rows;
    TexScroll_Step(&g_rollingCarpetBeltScroll, g_rollingCarpetBeltBackup);
}
