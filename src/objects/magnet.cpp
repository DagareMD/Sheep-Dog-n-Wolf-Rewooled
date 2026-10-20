
#include "sdw_types.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
struct MoveModifyArg;
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 opts);

#define SDW_MEMBERS_ScnObject            \
    static void *operator new(uptr size); \
    void AttachTo(ScnObject *parent, u8 joint, Vec3s *offset, Vec3s *rot, u32 arg, uptr arg2);

#include "sdw_classes.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_INSTFLAGS_U16 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_INSTFLAGS_U16
#define SDW_INLINE_SCNOBJECT_GETCLASSID 1
#define SDW_INLINE_SCNOBJECT_ISINWORLD 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETCLASSID
#undef SDW_INLINE_SCNOBJECT_ISINWORLD
#define SDW_INLINE_SCNBODY_GETANIMID 1
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_GETANIMID
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32
#define SDW_INLINE_SHADOW_SETVISIBLE_S32 1
#include "../engine/shadow_inlines.h"
#undef SDW_INLINE_SHADOW_SETVISIBLE_S32

/* The MSG_MODIFY_MOVE payload is MoveModifyArg: BlackHole reads the same layout and skips its pull on the same bit. */

s32 Vec3s_Dist(Vec3s *a, Vec3s *b);
#include "../engine/scn_tools.h"
#include "../engine/fixed_math.h"
#include "../engine/sound_mgr.h"
s32 ObjGrid_QueryBoxPoints(const CollBox *query, ScnObject **out);
u16 Sound_Play(u16 soundId, void *owner, u16 volume, u8 flags, s32 rate);
extern s32 g_dt;
extern s32 g_dtDither; /* time.cpp: Rewooled, rounds a frame-time step (see engine/approach.cpp) */

#define SDW_INLINE_SCNOBJECT_GETPARENT 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETPARENT

/* Magnet_PullMover */
void Magnet::PullMover(MoveModifyArg *arg, ScnObject *mover)
{
    Vec3s p; /* acceleration */
    Vec3s q; /* predicted position */
    Vec3s r; /* attraction direction */
    s32 s;   /* attraction step */
    s32 t;   /* target distance */
    s32 u;   /* attraction strength */
    if (arg->velocity.y > 300) {
        arg->velocity.y = 300;
        arg->delta.y = (s16)((g_dt * 300 + g_dtDither) >> 12);
    }
    q.x = mover->pos.x + arg->delta.x;
    q.y = mover->pos.y + arg->delta.y;
    q.z = mover->pos.z + arg->delta.z;
    t = Vec3s_Dist(&q, &attractPos);
    r.x = attractPos.x - q.x;
    r.y = attractPos.y - q.y;
    r.z = attractPos.z - q.z;
    Vec3s_Normalize(&r, &r);
    if (t > 400)
        u = 0;
    else {
        if (t < 60) {
            if (onRod)
                s = 340;
            else
                s = 0;
        } else
            s = 340 - (t - 60);
        u = s * s * 600 / 115600;
    }
    s = (u * g_dt + g_dtDither) >> 12;
    if (s >= t) {
        arg->delta.x = attractPos.x - mover->pos.x;
        arg->delta.y = attractPos.y - mover->pos.y;
        arg->delta.z = attractPos.z - mover->pos.z;
    } else {
        p.x = u * r.x / 4096;
        p.y = u * r.y / 4096;
        p.z = u * r.z / 4096;
        arg->velocity.x += p.x;
        arg->velocity.y += p.y;
        arg->velocity.z += p.z;
        Vec3s_ScaleByDt(&arg->velocity, &arg->delta);
    }
}

/* Magnet_ReleaseObject. The untouched drop-flag bits are intentionally uninitialized. */
void Magnet::ReleaseObject(ScnObject *obj)
{
    s32 result;
    DropMsgArg arg;
    result = obj->HandleMessage(this, MSG_RIDER_REMOVE, 0);
    if (this == obj->GetParent()) {
        arg.placed = 0;
        arg.flag1 = 1;
        arg.pos = obj->pos;
        obj->HandleMessage(this, MSG_DROP, &arg);
    }
}

/* Magnet_UpdateCaughtObject */
void Magnet::UpdateCaughtObject(ScnObject *obj, s32 dist2)
{
    DropMsgArg arg;
    if (onRod && dist2 <= 400) {
        if (!obj->InstFlags(INST_F_ATTACHED))
            obj->HandleMessage(this, MSG_PICKUP, 0);
    } else if (this == obj->GetParent()) {
        arg.placed = 0;
        arg.flag1 = 1;
        arg.pos = obj->pos;
        obj->HandleMessage(this, MSG_DROP, &arg);
    }
    if (dist2 <= 4225)
        obj->HandleMessage(this, MSG_MAGNET_CONTACT, 0);
}

/* Magnet_UpdateCaughtList */
void Magnet::UpdateCaughtList(ScnObject **objects, s32 *dist2s, s32 count)
{
    s32 i;
    s32 j;
    if (count == 0)
        ReleaseAll();
    else {
        if (!humSound)
            humSound = Sound_Play(SND_MAGNET_HUM, this, 0xff, SNDF_LOOP | SNDF_POSITIONAL | SNDF_NO_RETRIGGER, 0x1000);
        for (i = 0; i < 4; i++) {
            if (caught[i]) {
                j = 0;
                while (j < count && caught[i] != objects[j])
                    j++;
                if (count == j) {
                    ReleaseObject(caught[i]);
                    caught[i] = 0;
                } else {
                    UpdateCaughtObject(objects[j], dist2s[j]);
                    objects[j] = 0;
                }
            }
        }
        i = 0;
        for (j = 0; j < count; j++) {
            if (objects[j]) {
                if (objects[j]->HandleMessage(this, MSG_RIDER_ADD, 0)) {
                    UpdateCaughtObject(objects[j], dist2s[j]);
                    while (caught[i])
                        i++;
                    caught[i] = objects[j];
                }
            }
        }
    }
}

/* Magnet_ReleaseAll */
void Magnet::ReleaseAll()
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (caught[i]) {
            ReleaseObject(caught[i]);
            caught[i] = 0;
        }
    }
    if (humSound) {
        u16 handle = humSound;
        Sound_Stop(handle, this);
        humSound = 0;
    }
}

/* Magnet_Update */
void Magnet::Update()
{
    ScnObject *p[4]; /* accepted objects */
    s32 q;           /* query count */
    s32 r;           /* magnetic-force reply */
    s32 s;           /* accepted count */
    s32 t[4];        /* squared distances */
    s32 u;           /* current squared distance */
    struct {
        ScnObject *object;
        s32 force;
    } v;
    ScnObject *w[64]; /* query candidates */
    ScnObject *x;
    CollBox i;
    s32 j;
    s = 0;
    if (InstFlags(INST_F_ATTACHED)) {
        onRod = GetParent()->GetClassId() == CLASSID_MAGNETROD;
        attractPos = pos;
        if (onRod)
            attractPos.y += 75;
        i.min.x = attractPos.x - 400;
        i.min.y = attractPos.y - 400;
        i.min.z = attractPos.z - 400;
        i.max.x = attractPos.x + 400;
        i.max.y = attractPos.y + 400;
        i.max.z = attractPos.z + 400;
        q = ObjGrid_QueryBoxPoints(&i, w);
        for (j = 0; j < q; j++) {
            x = w[j];
            u = Vec3s_DistSq(&x->pos, &attractPos);
            if (u <= 160000) {
                r = x->HandleMessage(this, MSG_MAGNET_QUERY, 0);
                if (r && s < 4) {
                    v.object = this;
                    v.force = r;
                    x->HandleMessage(this, MSG_MAGNET_PULL, &v);
                    if (s == 0 && !onRod) {
                        v.object = x;
                        v.force = r;
                        GetParent()->HandleMessage(this, MSG_MAGNET_PULL, &v);
                    }
                    p[s] = x;
                    t[s] = u;
                    s++;
                }
            }
        }
        if (s && !onRod) {
            if (GetAnimId() != AAIMAN01_ANIM_MAGNET)
                PlayAnim(AAIMAN01_ANIM_MAGNET, 1, 1);
        } else {
            if (GetAnimId() != AAIMAN01_ANIM_LINK)
                PlayAnim(AAIMAN01_ANIM_LINK, 1, 1);
        }
    }
    UpdateCaughtList(p, t, s);
    AdvanceAnim();
}

/* Magnet_HandleMessage */
sptr Magnet::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    Vec3s *dropPos;
    MoveModifyArg *move;
    switch (msgId) {
        case MSG_QUERY_ACTION:
            if (sender->GetClassId() == CLASSID_WOLF)
                return CTX_PICKUP;
            break;
        case MSG_PICKUP:
            AttachTo(sender, (u8)(uptr)arg, 0, 0, 0, 0);
            if (sender->GetClassId() != CLASSID_MAGNETROD)
                shadow.SetVisible(0);
            PlayAnim(AAIMAN01_ANIM_LINK, 0, 0);
            return 1;
        case MSG_DROP:
            dropPos = (Vec3s *)arg;
            Detach();
            SetPosition(dropPos);
            shadow.SetVisible(1);
            PlayAnim(AAIMAN01_ANIM_OBJET, 0, 0);
            return 1;
        case MSG_INVENTORY_STORED:
            ReleaseAll();
            return 1;
        case MSG_MODIFY_MOVE:
            move = (MoveModifyArg *)arg;
            if (!move->noPull)
                PullMover(move, sender);
            return 1;
        case MSG_CONTAINER_STATE:
            switch ((u32)(uptr)arg) {
                case CONTAINER_RELEASED:
                    homePos = pos;
                    break;
            }
            return 1;
        case MSG_CHECKPOINT_ROLLBACK:
            Reset();
            return 1;
    }
    return 0;
}

/* Magnet_Reset */
void Magnet::Reset()
{
    if (IsInWorld())
        SetPosition(&homePos);
    humSound = 0;
    shadow.SetVisible(1);
    PlayAnim(AAIMAN01_ANIM_OBJET, 0, 0);
    attractPos = pos;
}

/* Magnet_Init */
void Magnet::PostLoadInit()
{
    s32 i;
    humSound = 0;
    if (IsInWorld())
        SnapToGround(1);
    homePos = pos;
    shadow.radius = 15;
    shadow.SetVisible(1);
    PlayAnim(AAIMAN01_ANIM_OBJET, 0, 0);
    for (i = 0; i < 4; i++)
        caught[i] = 0;
    onRod = 0;
    attractPos = pos;
}

/* Magnet_Create */
ScnObject *Magnet_Create(void *record)
{
    Magnet *obj = new Magnet;
    obj = (Magnet *)obj->Init(record, 0);
    return obj;
}
