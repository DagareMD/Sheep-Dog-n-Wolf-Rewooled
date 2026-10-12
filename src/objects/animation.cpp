#include "sdw_types.h"
class Mat44;
#include "sdw_enums.h"
#include "../sdk/d3d7.h"
#include "../sdk/crt.h"

/* the Mat44 members the functions below use */
#define SDW_MEMBERS_Mat44 Mat44();
#define SDW_MEMBERS_D3DApp void SetTransform(u32 state, Mat44 *m);
#include "sdw_classes.h"
#define SDW_INLINE_D3DAPP_SETTRANSFORM_U32_MAT44 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_SETTRANSFORM_U32_MAT44
#include "sdw_enums.h"
#include "../sdk/d3d7.h"

Mat44 g_animPartMatrices[42];
char g_strUnknown[] = "<unknown>";
char g_fmtAnimNotFound[] = "Cannot find AnimID %d, Model %s, Anim %s\n";

/* ---- declarations used by the animation helpers ---- */
#include "../app/app_main.h"
#include "../engine/game_state.h"
#include "../engine/fixed_math.h"
#include "../engine/mat44.h"
#include "../engine/screen.h"
#include "../engine/draw2d.h"
#include "world_draw.h"
#include "../engine/tex_table.h"
#include "instance.h"
#include "../engine/load_warmeshes.h"
#include "scenaric_props.h"
u16 Str_Length(const char *);
u32 Anim_Start(Instance *instance, Animator *animation, u16 id, u32 mode);
u32 Anim_GetDurationMs(Instance *instance, u16 id, u8 includeFirstTrack);
void Anim_GetDebugNames(Instance *instance, u16 id, const char **modelName, const char **animationName);
void Debug_Printf(const char *format, ...);
u16 Anim_DecodeFirstKey(Animator *, void *);
u16 Anim_DecodeKey(Animator *, const void *, u16);
void Anim_InterpolatePose(Animator *);
/* Variable-size direct animation table at Model::animTable. */
struct DirectAnimTable {
    u32 count;
    SDW_WARPTR(AnimHeader) entries[1]; /* file offsets (sdw_fileptr.h) */
};

inline u16 NextFrame(u16 current, u16 count)
{
    return (current + 1) % count;
}
inline void SwapPoses(void *&a, void *&b)
{
    void *saved = a;
    a = b;
    b = saved;
}
inline u32 HasFlags(Animator *a, u16 mask)
{
    return a->flags & mask;
}
inline void SetFlags(Animator *a, u16 mask)
{
    a->flags |= mask;
}
inline void ClearFlags(Animator *a, u16 mask)
{
    a->flags &= (u16)~mask;
}

/* ---- declarations used by the instance matrix and part drawing ---- */
/* The explicit out-pointer spelling agrees with the declaration of Mat44_Mul's definition. */
struct RawMatrix {
    float m[4][4];
};
struct MatrixReturnStorage {
    float m[4][4];
    MatrixReturnStorage() {}
};

#define g_mirrorRenderFlag (g_screen.mirrorRenderFlag)

#define g_mirrorRenderOn (g_screen.mirrorRenderOn)

#define g_mirrorPlaneVert2x (g_screen.mirrorPlaneVert2x)

void Instance_CalcWorldMatrix(Instance *, Camera *, const Vec3s *, Mat44 *);

/* ---- declarations used by the key readers ---- */

u32 Anim_StartDirect(Instance *instance, Animator *animation, u16 id, u32 mode)
{
    AnimHeader *a = ((DirectAnimTable *)(void *)instance->inst_model->animTable)->entries[id];
    if (a == 0)
        return 0;
    animation->cur = a;
    animation->animId = id;
    animation->timeAcc = 0;
    switch (mode) {
        case 0:
            animation->frame = NextFrame(0, animation->cur->keyCount);
            Anim_DecodeFirstKey(animation, animation->bufA);
            Anim_DecodeFirstKey(animation, animation->bufC);
            animation->frameDuration = Anim_DecodeKey(animation, animation->bufA, animation->frame);
            break;
        case 1:
            animation->frame = 0;
            SwapPoses(animation->bufA, animation->bufC);
            animation->frameDuration = Anim_DecodeFirstKey(animation, animation->bufB);
            break;
    }
    return 1;
}

AnimKey *Anim_GetCurrentKey(Animator *animation)
{
    u16 a = 0;
    AnimKey *b = (AnimKey *)animation->cur->keys;
    while (a < animation->frame) {
        ++a;
        b = (AnimKey *)((u16 *)b + b->payloadWords + 4);
    }
    return b;
}

AnimKey *Anim_GetNextKey(Animator *animation, u16 frame)
{
    u16 a = 0;
    u16 b = (frame + 1) % animation->cur->keyCount;
    AnimKey *c = (AnimKey *)animation->cur->keys;
    while (a != b) {
        ++a;
        c = (AnimKey *)((u16 *)c + c->payloadWords + 4);
    }
    return c;
}

void Anim_GetDebugNames(Instance *instance, u16 id, const char **modelName, const char **animationName)
{
    u16 a;
    u16 *b;
    u16 c, d, e;
    const char *f = g_strUnknown;
    if (!g_animNameTable) {
        *animationName = f;
        *modelName = f;
        return;
    }
    b = (u16 *)g_animNameTable;
    a = *b++;
    while (a--) {
        e = *b++;
        c = *b++;
        *modelName = (char *)b;
        b += (Str_Length(*modelName) + 2) / 2;
        while (c--) {
            d = *b++;
            *animationName = (char *)b;
            b += (Str_Length(*animationName) + 2) / 2;
            if (e == *instance->record && d == id)
                return;
        }
    }
    *animationName = f;
    *modelName = f;
}

/* ----: Anim_Start ---- */
inline AnimHeader *StartLookup(Instance *instance, u16 id, u32 mode)
{
    struct {
        const char *modelName;
        const char *animationName;
        u32 *mappedTable;
        AnimHeader *entry;
    } w;
    /* Skip directCount and that many direct entries. */
    /* the same words; the game resolves the entry's file offset (sdw_fileptr.h) */
    w.mappedTable = (u32 *)(void *)instance->inst_model->animTable;
    w.mappedTable += w.mappedTable[0] + 1;
    if (id >= w.mappedTable[0])
        return 0;
    w.entry = (AnimHeader *)War_Resolve(w.mappedTable[id + 1]);
    if (w.entry == 0) {
        if (mode & ANIM_SET_OPTIONAL)
            return 0;
        Anim_GetDebugNames(instance, id, &w.modelName, &w.animationName);
        Debug_Printf(g_fmtAnimNotFound, id, w.modelName, w.animationName);
    }
    return w.entry;
}
inline void StartSetFlags(Animator *animation, u16 mask)
{
    animation->flags |= mask;
}
inline void StartClearFlags(Animator *animation, u16 mask)
{
    animation->flags &= (u16)~mask;
}

u32 Anim_Start(Instance *instance, Animator *animation, u16 id, u32 mode)
{
    AnimHeader *data = StartLookup(instance, id, mode);
    if (data == 0) {
        StartClearFlags(animation, ANIM_F_LOOP);
        StartSetFlags(animation, ANIM_F_FINISHED);
        StartSetFlags(animation, ANIM_F_PLAYING);
        animation->animId = id;
        return 0;
    }
    if (mode & ANIM_SET_LOOP)
        StartSetFlags(animation, ANIM_F_LOOP);
    else
        StartClearFlags(animation, ANIM_F_LOOP);
    StartClearFlags(animation, ANIM_F_FINISHED);
    StartSetFlags(animation, ANIM_F_PLAYING);
    animation->cur = data;
    animation->animId = id;
    animation->timeAcc = 0;
    if (mode & ANIM_SET_BLEND) {
        animation->frame = 0;
        SwapPoses(animation->bufA, animation->bufC);
        animation->frameDuration = Anim_DecodeFirstKey(animation, animation->bufB);
    } else {
        animation->frame = NextFrame(0, animation->cur->keyCount);
        Anim_DecodeFirstKey(animation, animation->bufA);
        Anim_DecodeFirstKey(animation, animation->bufC);
        animation->frameDuration = Anim_DecodeKey(animation, animation->bufA, animation->frame);
    }
    return 1;
}

/* ----: Anim_GetDurationMs ---- */
static __inline AnimHeader *Anim_LookupDuration(Instance *instance, u16 animationId)
{
    struct {
        const char *modelName;
        const char *animationName;
        u32 *mapped;
        AnimHeader *found;
    } w;

    /* 32-bit count-before-base address arithmetic, as in Anim_Start.
     * Skip the direct entries to reach the mapped count. */
    /* the same words; the game resolves the entry's file offset (sdw_fileptr.h) */
    w.mapped = (u32 *)(void *)instance->inst_model->animTable;
    w.mapped += w.mapped[0] + 1;
    if ((u32)animationId >= *w.mapped)
        return 0;
    w.found = (AnimHeader *)War_Resolve(w.mapped[animationId + 1]);
    if (!w.found) {
        /* The original keeps this constant-false branch from its lookup path. */
        if (0)
            return 0;
        Anim_GetDebugNames(instance, animationId, &w.modelName, &w.animationName);
        Debug_Printf(g_fmtAnimNotFound, animationId, w.modelName, w.animationName);
    }
    return w.found;
}

u32 Anim_GetDurationMs(Instance *instance, u16 animationId, u8 includeFirstTrack)
{
    /* Existing caller name retained; this byte includes the first KEY. */
    struct {
        AnimKey *key;
        u16 pad;
        u16 index;
        u32 total;
        AnimHeader *animation;
    } w;
    w.animation = Anim_LookupDuration(instance, animationId);
    if (!w.animation)
        return 0;

    w.key = (AnimKey *)w.animation->keys;
    if (includeFirstTrack)
        w.total = w.key->durationMs;
    else
        w.total = 0;
    for (w.index = 1; w.index < w.animation->keyCount; w.index++) {
        w.key = (AnimKey *)((u16 *)w.key + w.key->payloadWords + 4);
        w.total += w.key->durationMs;
    }
    return w.total;
}

void Anim_RestorePose(Animator *animation)
{
    u16 a = animation->frame - 1;
    u16 b;
    if (a == 0xffff)
        a = 0;
    Anim_DecodeFirstKey(animation, animation->bufA);
    SwapPoses(animation->bufA, animation->bufB);
    for (b = 1; b <= a; ++b)
        Anim_DecodeKey(animation, animation->bufB, b);
    Anim_DecodeFirstKey(animation, animation->bufA);
    SwapPoses(animation->bufA, animation->bufB);
    for (b = 1; b <= animation->frame; ++b)
        Anim_DecodeKey(animation, animation->bufB, b);
    Anim_InterpolatePose(animation);
}

void Anim_Advance(Animator *animation)
{
    if (HasFlags(animation, ANIM_F_PLAYING))
        Anim_InterpolatePose(animation);
    else
        return;
    if (!HasFlags(animation, ANIM_F_LOOP) && HasFlags(animation, ANIM_F_FINISHED))
        return;
    animation->timeAcc += ((u32)g_animDt * animation->speed) >> 12;
    ClearFlags(animation, ANIM_F_FINISHED);
    while (animation->timeAcc >= (u32)(animation->frameDuration << 10)) {
        animation->timeAcc -= animation->frameDuration << 10;
        animation->frame = NextFrame(animation->frame, animation->cur->keyCount);
        if (animation->frame == 0) {
            SetFlags(animation, ANIM_F_FINISHED);
            if (!HasFlags(animation, ANIM_F_LOOP)) {
                SwapPoses(animation->bufC, animation->bufB);
                ClearFlags(animation, ANIM_F_PLAYING);
                return;
            }
            if (!HasFlags(animation, ANIM_F_ALT_WRAP)) {
                Anim_DecodeFirstKey(animation, animation->bufA);
                animation->frame = NextFrame(0, animation->cur->keyCount);
                animation->frameDuration = Anim_DecodeKey(animation, animation->bufA, animation->frame);
            } else {
                SwapPoses(animation->bufA, animation->bufB);
                animation->frameDuration = Anim_DecodeFirstKey(animation, animation->bufB);
            }
        } else {
            SwapPoses(animation->bufA, animation->bufB);
            animation->frameDuration = Anim_DecodeKey(animation, animation->bufA, animation->frame);
        }
    }
}

void Anim_PostInitStub(Instance *instance, Animator *animation) {}

void Anim_GetRootOffset(Instance *instance, Animator *animation, Vec3s *out)
{
    ModelJoint *a;
    Vec3f b;
    AnimJointPose *c;
    u32 d;
    Mat44 e;
    Vec3f f;
    c = (AnimJointPose *)animation->bufC;
    a = instance->inst_model->joints;
    e.SetRotXZY(Math_Angle4096ToRadians_2(instance->rot.x & 0xfff), Math_Angle4096ToRadians_2(instance->rot.y & 0xfff),
                Math_Angle4096ToRadians_2(instance->rot.z & 0xfff));
    b.x = (float)a->offset.x + c->pos[0];
    b.y = (float)a->offset.y + c->pos[1];
    b.z = (float)a->offset.z + c->pos[2];
    f.x = e.m[0][0] * b.x + e.m[1][0] * b.y + e.m[2][0] * b.z + e.m[3][0];
    f.y = e.m[0][1] * b.x + e.m[1][1] * b.y + e.m[2][1] * b.z + e.m[3][1];
    f.z = e.m[0][2] * b.x + e.m[1][2] * b.y + e.m[2][2] * b.z + e.m[3][2];
    out->x = (s16)(f.x / 8.0f);
    out->y = (s16)(f.y / 8.0f);
    out->z = (s16)(f.z / 8.0f);
}

/* ----: Instance_CalcWorldMatrix ---- */
#define SDW_INLINE_FREE_INSTANCEHASFLAGS_INSTANCEBASE_U16 1
#define SDW_INLINE_FREE_INSTANCESETFLAGS_INSTANCEBASE_U16 1
#include "instance_inlines.h"
#undef SDW_INLINE_FREE_INSTANCEHASFLAGS_INSTANCEBASE_U16
#undef SDW_INLINE_FREE_INSTANCESETFLAGS_INSTANCEBASE_U16
#define SDW_INLINE_FREE_INSTANCECLEARFLAGS_INSTANCEBASE_U16 1
#include "instance_inlines.h"
#undef SDW_INLINE_FREE_INSTANCECLEARFLAGS_INSTANCEBASE_U16
void Instance_CalcWorldMatrix(Instance *instance, Camera *camera, const Vec3s *scale, Mat44 *out)
{
    if (InstanceHasFlags(instance, INST_F_ATTACHED)) {
        AttachLink *link_18 = instance->attachLink;
        if (link_18->flags.matrixValid) {
            *out = link_18->matrix;
            if (link_18->flags.hasLocal) {
                Vec3f point_9, result_18;
                point_9.x = link_18->localOffset.x * 8.0f;
                point_9.y = link_18->localOffset.y * 8.0f;
                point_9.z = link_18->localOffset.z * 8.0f;
                result_18.x =
                    point_9.x * out->m[0][0] + point_9.y * out->m[1][0] + point_9.z * out->m[2][0] + out->m[3][0];
                result_18.y =
                    point_9.x * out->m[0][1] + point_9.y * out->m[1][1] + point_9.z * out->m[2][1] + out->m[3][1];
                result_18.z =
                    point_9.x * out->m[0][2] + point_9.y * out->m[1][2] + point_9.z * out->m[2][2] + out->m[3][2];
                out->m[3][0] = result_18.x;
                out->m[3][1] = result_18.y;
                out->m[3][2] = result_18.z;
            }
            if (link_18->flags.hasWorld) {
                out->m[3][0] += link_18->worldOffset.x * 8.0f;
                out->m[3][1] += link_18->worldOffset.y * 8.0f;
                out->m[3][2] += link_18->worldOffset.z * 8.0f;
            }
            InstanceSetFlags(instance, INST_F_DRAWN);
        } else
            InstanceClearFlags(instance, INST_F_DRAWN);
    } else {
        Mat44 scaleMatrix_7;
        Mat44 rotationMatrix_8;
        Mat44 translationMatrix_0;
        Vec4i position_14, transformed_5, bounds_1;
        u32 unused_2;
        RawMatrix product_29, worldProduct_18;
        if (scale)
            scaleMatrix_7.SetScale(Math_Fixed10ToFloat_s16(scale->x), Math_Fixed10ToFloat_s16(scale->y),
                                   Math_Fixed10ToFloat_s16(scale->z));
        else
            scaleMatrix_7.SetIdentity();
        rotationMatrix_8.SetRotXZY(Math_Angle4096ToRadians_2(instance->rot.x & 0xfff),
                                   Math_Angle4096ToRadians_2(instance->rot.y & 0xfff),
                                   Math_Angle4096ToRadians_2(instance->rot.z & 0xfff));
        if (g_mirrorRenderFlag == -1 || g_mirrorRenderOn == 1) {
            rotationMatrix_8.m[0][1] = -rotationMatrix_8.m[0][1];
            rotationMatrix_8.m[1][1] = -rotationMatrix_8.m[1][1];
            rotationMatrix_8.m[2][1] = -rotationMatrix_8.m[2][1];
            instance->pos.y = g_mirrorPlaneVert2x - instance->pos.y;
        }
        translationMatrix_0.SetTranslation(instance->pos.x * 8.0f, instance->pos.y * 8.0f, instance->pos.z * 8.0f);
        *out = *Mat44_Mul((Mat44 *)&worldProduct_18, Mat44_Mul((Mat44 *)&product_29, &scaleMatrix_7, &rotationMatrix_8),
                          &translationMatrix_0);
        bounds_1.x = instance->boundCenter.x;
        bounds_1.y = instance->boundCenter.y;
        bounds_1.z = instance->boundCenter.z;
        position_14.x = instance->pos.x;
        position_14.y = instance->pos.y;
        position_14.z = instance->pos.z;
        transformed_5.x = (s16)(position_14.x * camera->viewMat.m[0][0] + position_14.y * camera->viewMat.m[1][0] +
                                position_14.z * camera->viewMat.m[2][0] + camera->viewMat.m[3][0]);
        transformed_5.y = (s16)(position_14.x * camera->viewMat.m[0][1] + position_14.y * camera->viewMat.m[1][1] +
                                position_14.z * camera->viewMat.m[2][1] + camera->viewMat.m[3][1]);
        transformed_5.z = (s16)(position_14.x * camera->viewMat.m[0][2] + position_14.y * camera->viewMat.m[1][2] +
                                position_14.z * camera->viewMat.m[2][2] + camera->viewMat.m[3][2]);
        bounds_1.x += transformed_5.x;
        bounds_1.y += transformed_5.y;
        bounds_1.z += transformed_5.z;
        Instance_UpdateVisibility(instance, (const Vec3i *)&bounds_1, instance->boundRadius, 1);
        if (g_mirrorRenderFlag == -1 || g_mirrorRenderOn == 1)
            instance->pos.y = g_mirrorPlaneVert2x - instance->pos.y;
    }
}

/* ----: Instance_DrawAnimParts ---- */
void Instance_DrawAnimParts(Instance *instance, Animator *animation, Camera *camera, const Vec3s *scale)
{
    Mat44 world;
    Instance_CalcWorldMatrix(instance, camera, scale, &world);
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_VIEW, &camera->viewMat);
    if (InstanceHasFlags(instance, INST_F_DRAWN)) {
        u32 unused_9;
        Mesh *mesh_9;
        u16 vertexCount_0;
        AnimJointPose *pose_9;
        Mat44 *matrix_6;
        u16 index_0;
        AnimJointPose *parentPose_3;
        u16 firstVertex_17;
        ModelJoint *joint_13;
        AttachLink *link_2;
        AttachLink **links_8;
        u8 childCount_2;
        Mat44 samWaist, samLowerChest;
        bool samGuardTwist = false;
        mesh_9 = (Mesh *)Texture_FindByResource(instance->inst_model);
        if (instance->inst_mode == INST_MODE_SCN_BODY && animation->nbJoints == 29 &&
            instance->inst_model->nbJoints == 29 && mesh_9->vertexCount == 168 &&
            static_cast<ScnObject *>(instance)->classId == CLASSID_SAM) {
            const Sam *sam = static_cast<Sam *>(instance);
            const ModelJoint *joints = instance->inst_model->joints;
            samGuardTwist = (sam->state == SAM_STT_LOOKAFTER || sam->state == SAM_STT_LOOKAFTER_TIMED) &&
                            joints[9].parent == 1 && joints[9].vertexCount == 8 &&
                            joints[10].parent == 9 && joints[10].vertexCount == 14;
        }
        pose_9 = (AnimJointPose *)animation->bufC;
        joint_13 = instance->inst_model->joints;
        matrix_6 = g_animPartMatrices;
        matrix_6->SetTranslation(joint_13->offset.x + pose_9->pos[0], joint_13->offset.y + pose_9->pos[1],
                                 joint_13->offset.z + pose_9->pos[2]);
        matrix_6->MulInPlace(world);
        pose_9++;
        joint_13++;
        matrix_6++;
        for (index_0 = 1; index_0 < animation->nbJoints; index_0++, pose_9++, joint_13++, matrix_6++) {
            Mat44 translation;
            MatrixReturnStorage result;
            MatrixReturnStorage intermediate;
            parentPose_3 = (AnimJointPose *)animation->bufC + joint_13->parent;
            matrix_6->SetRotYXZ(pose_9->rot[0], pose_9->rot[1], pose_9->rot[2]);
            translation.SetTranslation(joint_13->offset.x * parentPose_3->scale[0] + pose_9->pos[0],
                                       joint_13->offset.y * parentPose_3->scale[1] + pose_9->pos[1],
                                       joint_13->offset.z * parentPose_3->scale[2] + pose_9->pos[2]);
            *matrix_6 = *Mat44_Mul((Mat44 *)&result,
                                   Mat44_Mul((Mat44 *)&intermediate, matrix_6, &translation),
                                   &g_animPartMatrices[joint_13->parent]);
            if (samGuardTwist && index_0 == 9) {
                // ASAMSH01's waist row gets one third of the guarding yaw. Keep the full joint
                // matrix above for its descendants and attachments; these are vertex-only variants.
                float yaw = pose_9->rot[1];
                if (yaw > 3.1415927f)
                    yaw -= 6.2831855f;
                if (yaw < -3.1415927f)
                    yaw += 6.2831855f;
                samWaist.SetRotYXZ(pose_9->rot[0], yaw / 3.0f, pose_9->rot[2]);
                samWaist.MulInPlace(translation);
                samWaist.MulInPlace(g_animPartMatrices[joint_13->parent]);
                samLowerChest.SetRotYXZ(pose_9->rot[0], yaw * (2.0f / 3.0f), pose_9->rot[2]);
                samLowerChest.MulInPlace(translation);
                samLowerChest.MulInPlace(g_animPartMatrices[joint_13->parent]);
            } else if (samGuardTwist && index_0 == 10) {
                // Preserve the chest's animated local pose, with only the inherited waist yaw reduced.
                Mat44 chestLocal;
                chestLocal.SetRotYXZ(pose_9->rot[0], pose_9->rot[1], pose_9->rot[2]);
                chestLocal.MulInPlace(translation);
                Mat44_Mul(&samLowerChest, &chestLocal, &samLowerChest);
            }
        }
        joint_13 = instance->inst_model->joints + 1;
        pose_9 = (AnimJointPose *)animation->bufC + 1;
        for (index_0 = 1; index_0 < animation->nbJoints; index_0++, joint_13++, pose_9++) {
            Mat44 scaling;
            MatrixReturnStorage result;
            if (pose_9->channels & (ANIMKEY_SCALE_X | ANIMKEY_SCALE_Y | ANIMKEY_SCALE_Z)) {
                if (scale)
                    scaling.SetScale(Math_U16ToUnitFloat(scale->x) * pose_9->scale[0],
                                     Math_U16ToUnitFloat(scale->y) * pose_9->scale[1],
                                     Math_U16ToUnitFloat(scale->z) * pose_9->scale[2]);
                else
                    scaling.SetScale(pose_9->scale[0], pose_9->scale[1], pose_9->scale[2]);
                g_animPartMatrices[index_0] =
                    *Mat44_Mul((Mat44 *)&result, &scaling, &g_animPartMatrices[index_0]);
                if (samGuardTwist && index_0 == 9)
                    Mat44_Mul(&samWaist, &scaling, &samWaist);
                else if (samGuardTwist && index_0 == 10)
                    Mat44_Mul(&samLowerChest, &scaling, &samLowerChest);
            }
        }
        if (instance->attachedChildCount) {
            links_8 = AttachLink_FindFirstChildSlot(instance);
            childCount_2 = instance->attachedChildCount;
            while (childCount_2 > 0) {
                link_2 = *links_8++;
                childCount_2--;
                if (!link_2->flags.hasRot) {
                    if (!link_2->flags.rootRotation)
                        (link_2)->matrix = g_animPartMatrices[link_2->partIndex];
                    else {
                        (link_2)->matrix = g_animPartMatrices[0];
                        (link_2)->matrix.m[3][0] = g_animPartMatrices[link_2->partIndex].m[3][0];
                        (link_2)->matrix.m[3][1] = g_animPartMatrices[link_2->partIndex].m[3][1];
                        (link_2)->matrix.m[3][2] = g_animPartMatrices[link_2->partIndex].m[3][2];
                    }
                } else {
                    Mat44 rotation;
                    if (!link_2->flags.rootRotation) {
                        rotation.SetRotXYZ(Math_Angle4096ToRadians_2(link_2->rot.x),
                                           Math_Angle4096ToRadians_2(link_2->rot.y),
                                           Math_Angle4096ToRadians_2(link_2->rot.z));
                        (link_2)->matrix = g_animPartMatrices[link_2->partIndex];
                        (link_2)->matrix.MulInPlace(rotation);
                    } else {
                        rotation.SetRotXYZ(Math_Angle4096ToRadians_2(link_2->rot.x),
                                           Math_Angle4096ToRadians_2(link_2->rot.y),
                                           Math_Angle4096ToRadians_2(link_2->rot.z));
                        (link_2)->matrix = g_animPartMatrices[0];
                        (link_2)->matrix.m[3][0] = (link_2)->matrix.m[3][1] = (link_2)->matrix.m[3][2] = 0.0f;
                        (link_2)->matrix.MulInPlace(rotation);
                        (link_2)->matrix.m[3][0] = g_animPartMatrices[link_2->partIndex].m[3][0];
                        (link_2)->matrix.m[3][1] = g_animPartMatrices[link_2->partIndex].m[3][1];
                        (link_2)->matrix.m[3][2] = g_animPartMatrices[link_2->partIndex].m[3][2];
                    }
                }
                link_2->flags.matrixValid = 1;
            }
        }
        firstVertex_17 = 0;
        joint_13 = instance->inst_model->joints + 1;
        pose_9 = (AnimJointPose *)animation->bufC + 1;
        for (index_0 = 1; index_0 < animation->nbJoints; index_0++, joint_13++, pose_9++) {
            vertexCount_0 = joint_13->vertexCount;
            g_animPartMatrices[index_0].MulInPlace(g_matUnk6d5428);
            if (samGuardTwist && index_0 == 9) {
                samWaist.MulInPlace(g_matUnk6d5428);
                g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_WORLD, &samWaist);
            } else
                g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_WORLD, &g_animPartMatrices[index_0]);
            mesh_9->TransformRange(firstVertex_17, vertexCount_0);
            if (samGuardTwist && index_0 == 10) {
                // The lower chest ring is vertices 60, 61, 64 and 68 in the original mesh:
                // offsets 2, 3, 6 and 10 within joint 10. Everything above keeps the full turn.
                samLowerChest.MulInPlace(g_matUnk6d5428);
                g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_WORLD, &samLowerChest);
                mesh_9->TransformRange(firstVertex_17 + 2, 2);
                mesh_9->TransformRange(firstVertex_17 + 6, 1);
                mesh_9->TransformRange(firstVertex_17 + 10, 1);
            }
            firstVertex_17 += vertexCount_0;
        }
    }
}

void Anim_InterpolatePose(Animator *animation)
{
    float a = (float)animation->timeAcc / (float)(animation->frameDuration << 10);
    u16 b;
    AnimJointPose *c, *d, *e;
    float f, g, h, i, j, k;
    for (b = 0; b < animation->nbJoints; ++b) {
        e = (AnimJointPose *)animation->bufA + b;
        d = (AnimJointPose *)animation->bufB + b;
        c = (AnimJointPose *)animation->bufC + b;
        if ((f = d->rot[0] - e->rot[0]) > 3.1415927f)
            f -= 6.2831855f;
        if (f < -3.1415927f)
            f += 6.2831855f;
        if ((g = f * a + e->rot[0]) < 0.0f)
            g += 6.2831855f;
        else
            while (g >= 6.2831855f)
                g -= 6.2831855f;
        c->rot[0] = g;
        if ((h = d->rot[1] - e->rot[1]) > 3.1415927f)
            h -= 6.2831855f;
        if (h < -3.1415927f)
            h += 6.2831855f;
        if ((i = h * a + e->rot[1]) < 0.0f)
            i += 6.2831855f;
        else
            while (i >= 6.2831855f)
                i -= 6.2831855f;
        c->rot[1] = i;
        if ((j = d->rot[2] - e->rot[2]) > 3.1415927f)
            j -= 6.2831855f;
        if (j < -3.1415927f)
            j += 6.2831855f;
        if ((k = j * a + e->rot[2]) < 0.0f)
            k += 6.2831855f;
        else
            while (k >= 6.2831855f)
                k -= 6.2831855f;
        c->rot[2] = k;
        c->pos[0] = (d->pos[0] - e->pos[0]) * a + e->pos[0];
        c->pos[1] = (d->pos[1] - e->pos[1]) * a + e->pos[1];
        c->pos[2] = (d->pos[2] - e->pos[2]) * a + e->pos[2];
        c->scale[0] = (d->scale[0] - e->scale[0]) * a + e->scale[0];
        c->scale[1] = (d->scale[1] - e->scale[1]) * a + e->scale[1];
        c->scale[2] = (d->scale[2] - e->scale[2]) * a + e->scale[2];
        c->channels = d->channels;
    }
}

/* ----: the packed key readers ---- */
/* The original decoders share this expansion. Each listed channel is read
 * separately; cursor alignment and signed/unsigned conversion are preserved.
 * cast kept (BYTES, WORDS, CURSOR): an entry's values are s8 bytes or s16 words by its ANIMKEY_8BIT bit, and the
 * byte cursor is realigned to the next u16 by its address. */
#define DECODE_ENTRY(CURSOR, POSE, JOINT, CTRL, BYTES, WORDS)                                                \
    CTRL = *CURSOR++;                                                                                        \
    JOINT = CTRL & 0x3f;                                                                                     \
    POSE[JOINT].rot[0] = POSE[JOINT].rot[1] = POSE[JOINT].rot[2] = POSE[JOINT].pos[0] = POSE[JOINT].pos[1] = \
        POSE[JOINT].pos[2] = 0.0f;                                                                           \
    POSE[JOINT].scale[0] = POSE[JOINT].scale[1] = POSE[JOINT].scale[2] = 1.0f;                               \
    POSE[JOINT].channels = CTRL;                                                                             \
    if (CTRL & ANIMKEY_8BIT) {                                                                               \
\
        BYTES = (s8 *)CURSOR;                                                                                \
        if (CTRL & ANIMKEY_ROT_X)                                                                            \
            POSE[JOINT].rot[0] = Math_Angle128ToRadians(*BYTES++);                                           \
        if (CTRL & ANIMKEY_ROT_Y)                                                                            \
            POSE[JOINT].rot[1] = Math_Angle128ToRadians(*BYTES++);                                           \
        if (CTRL & ANIMKEY_ROT_Z)                                                                            \
            POSE[JOINT].rot[2] = Math_Angle128ToRadians(*BYTES++);                                           \
        if (CTRL & ANIMKEY_POS_X)                                                                            \
            POSE[JOINT].pos[0] = (float)*BYTES++;                                                            \
        if (CTRL & ANIMKEY_POS_Y)                                                                            \
            POSE[JOINT].pos[1] = (float)*BYTES++;                                                            \
        if (CTRL & ANIMKEY_POS_Z)                                                                            \
            POSE[JOINT].pos[2] = (float)*BYTES++;                                                            \
        if (CTRL & ANIMKEY_SCALE_X)                                                                          \
            POSE[JOINT].scale[0] = Math_U8ToUnitFloat((u8) * BYTES++);                                       \
        if (CTRL & ANIMKEY_SCALE_Y)                                                                          \
            POSE[JOINT].scale[1] = Math_U8ToUnitFloat((u8) * BYTES++);                                       \
        if (CTRL & ANIMKEY_SCALE_Z)                                                                          \
            POSE[JOINT].scale[2] = Math_U8ToUnitFloat((u8) * BYTES++);                                       \
        if ((uptr)BYTES % 2) {                                                                                \
            BYTES++;                                                                                         \
\
            CURSOR = (u16 *)BYTES;                                                                           \
        } else                                                                                               \
            CURSOR = (u16 *)BYTES;                                                                           \
    } else {                                                                                                 \
        WORDS = (s16 *)CURSOR;                                                                               \
        if (CTRL & ANIMKEY_ROT_X)                                                                            \
            POSE[JOINT].rot[0] = Math_Angle4096ToRadians(*WORDS++);                                          \
        if (CTRL & ANIMKEY_ROT_Y)                                                                            \
            POSE[JOINT].rot[1] = Math_Angle4096ToRadians(*WORDS++);                                          \
        if (CTRL & ANIMKEY_ROT_Z)                                                                            \
            POSE[JOINT].rot[2] = Math_Angle4096ToRadians(*WORDS++);                                          \
        if (CTRL & ANIMKEY_POS_X)                                                                            \
            POSE[JOINT].pos[0] = (float)*WORDS++;                                                            \
        if (CTRL & ANIMKEY_POS_Y)                                                                            \
            POSE[JOINT].pos[1] = (float)*WORDS++;                                                            \
        if (CTRL & ANIMKEY_POS_Z)                                                                            \
            POSE[JOINT].pos[2] = (float)*WORDS++;                                                            \
        if (CTRL & ANIMKEY_SCALE_X)                                                                          \
            POSE[JOINT].scale[0] = Math_U16ToUnitFloat((u16) * WORDS++);                                     \
        if (CTRL & ANIMKEY_SCALE_Y)                                                                          \
            POSE[JOINT].scale[1] = Math_U16ToUnitFloat((u16) * WORDS++);                                     \
        if (CTRL & ANIMKEY_SCALE_Z)                                                                          \
            POSE[JOINT].scale[2] = Math_U16ToUnitFloat((u16) * WORDS++);                                     \
\
        CURSOR = (u16 *)WORDS;                                                                               \
    }

u16 Anim_DecodeFirstKey(Animator *animation, void *destination)
{
    u16 *a;
    u16 b, c, d, e, f;
    u16 g;
    s8 *h;
    s16 *i;
#define POSE ((AnimJointPose *)destination)
    a = (u16 *)animation->cur->keys;
    d = *a++;
    c = *a++;
    a++;
    animation->pendingSoundId = *a++;
    for (b = 0; b < c; ++b) {
        DECODE_ENTRY(a, POSE, e, f, h, i)
    }
    return d;
#undef POSE
}

u16 Anim_DecodeKey(Animator *animation, const void *source, u16 keyIndex)
{
    u16 *a;
    u16 b, c, d, e;
    AnimJointPose *f;
    u16 g, h, i;
    u16 j;
    s8 *k;
    s16 *l;
    f = (AnimJointPose *)animation->bufB;
    memcpy(f, source, animation->nbJoints * sizeof(AnimJointPose));
    a = (u16 *)animation->cur->keys;
    for (c = 0; c < keyIndex; ++c) {
        g = a[2];
        a += 4;
        a += g;
    }
    e = *a++;
    d = *a++;
    a++;
    if (*a)
        animation->pendingSoundId = *a++;
    else
        a++;
    for (b = 0; b < d; ++b) {
        DECODE_ENTRY(a, f, h, i, k, l)
    }
    return e;
}

void Anim_ClearPose(Animator *animation, AnimJointPose *pose)
{
    u16 a;
    for (a = 0; a < animation->nbJoints; ++a) {
        pose->rot[0] = pose->rot[1] = pose->rot[2] = 0.0f;
        pose->pos[0] = pose->pos[1] = pose->pos[2] = 0.0f;
        pose->scale[0] = pose->scale[1] = pose->scale[2] = 1024.0f;
        pose->channels = 0;
    }
}
