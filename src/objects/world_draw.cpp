#include "sdw_types.h"
#include "sdw_enums.h"
#include "../sdk/d3d7.h"
#include "../sdk/ddraw.h"
#include "../sdk/win32.h"
class Mat44;

struct SamScreenGeometry {
    u16 width, height, x, y, aspect;
};

#define SDW_MEMBERS_D3DApp                  \
    void SetTransform(u32 state, Mat44 *m); \
    void ClearStateFlagsInline(u32);        \
    void Render_SetStateFlags(u32);         \
    void DrawPrimitiveInline(u32 type, u32 fvf, void *vertices, u32 count);
#define SDW_MEMBERS_Frustrum     \
    u8 IsFogEnabled()            \
    {                            \
        u8 enabled = fogEnabled; \
        return enabled;          \
    }
#define SDW_MEMBERS_Mat44 Mat44();
#define SDW_MEMBERS_Screen SamScreenGeometry *GetGeometry(SamScreenGeometry *);
#include "sdw_classes.h"
#include "../engine/draw2d.h"
#include "instance.h"
#include "../engine/fixed_math.h"
#include "../engine/tex_table.h"
#include "../engine/screen.h"
#include "animation.h"
#include "../engine/scenaric.h"
#define SDW_INLINE_D3DAPP_SETTRANSFORM_U32_MAT44 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_SETTRANSFORM_U32_MAT44
#define SDW_INLINE_D3DAPP_DRAWPRIMITIVEINLINE_U32_U32_VOID_U32 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_DRAWPRIMITIVEINLINE_U32_U32_VOID_U32
struct SkyVertex {
    float x, y, z, rhw;
    u32 diffuse, specular;
    SkyVertex() {}
};
u32 g_skyVtxDrawn;          /* vertices collected into the buffer */
SkyVertex g_skyVerts[9000];
inline void SetInstanceFlags(InstanceBase *instance, u16 mask)
{
    instance->inst_flags |= mask;
}
inline void ClearInstanceFlags(InstanceBase *instance, u16 mask)
{
    instance->inst_flags &= (u16)~mask;
}
inline void ScaleDiagonal(Mat34s *out, const Vec3s *scale, s32 shift)
{
    out->rot[0] = (out->rot[0] * scale->x) >> shift;
    out->rot[4] = (out->rot[4] * scale->y) >> shift;
    out->rot[8] = (out->rot[8] * scale->z) >> shift;
}
void Mat34s_FromEulerScaled(const Vec3s *rot, Mat34s *out, const Vec3s *scale)
{
    Mat34s_FromEulerXZY(rot, out);
    if (scale) {
        ScaleDiagonal(out, scale, 0);
    }
}
s32 Stub_Return0_55cadc()
{
    return 0;
}
void Instance_UpdateVisibility(Instance *instance, const Vec3i *center, u16 radius, s32 unused)
{
    float a, b;
    if (!g_pViewFrustum->cullDisabled) {
        a = center->z * g_pViewFrustum->tanHalfFov;
        b = a * g_pViewFrustum->aspect;
        ClearInstanceFlags(instance, INST_F_DRAWN);
        if (center->x - radius < a && center->x + radius > -a && -center->y - radius < b && -center->y + radius > -b &&
            center->z + radius > g_pViewFrustum->nearZ && center->z - radius < (double)g_pViewFrustum->viewDistance)
            SetInstanceFlags(instance, INST_F_DRAWN);
    } else
        SetInstanceFlags(instance, INST_F_DRAWN);
}
#define SDW_INLINE_D3DAPP_CLEARSTATEFLAGSINLINE_U32 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_CLEARSTATEFLAGSINLINE_U32

void WorldObj_DrawSky(WorldObj *object, Camera *camera)
{
    Mat44 c, d;
    Camera a;
    Mesh *e;
    d.SetIdentity();
    a.viewMatCopy = camera->viewMatCopy;
    a.viewMatCopy.m[3][0] = a.viewMatCopy.m[3][1] = a.viewMatCopy.m[3][2] = 0.0f;
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_VIEW, &a.viewMatCopy);
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_WORLD, &d);
    if (!Cull_IsAabbVisible(&object->aabb, &a) || !object->inst_kind)
        ClearInstanceFlags(object, INST_F_DRAWN);
    else {
        e = (Mesh *)Texture_FindByResource(object->inst_model);
        SetInstanceFlags(object, INST_F_DRAWN);
        e->TransformAll();
        g_skyVtxDrawn = e->CollectVisibleVerts(g_skyVerts, 0);
        if (g_pViewFrustum->IsFogEnabled() == 1)
            g_pD3DAppMain->ClearStateFlagsInline(RSF_FOG);
        g_pD3DAppMain->Render_SetStateFlags(g_pPolyBin->GetTypeStateFlags(RPOLY_OPAQUE));
        g_pD3DAppMain->DrawPrimitiveInline(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                           g_skyVerts, g_skyVtxDrawn);
        if (g_pViewFrustum->IsFogEnabled() == 1)
            g_pD3DAppMain->Render_SetStateFlags(RSF_FOG);
    }
}

extern u32 *g_screenLayerBase;
#define g_projFocalScale (g_screen.projDist) /* = g_screen + 0x88: a field, not an object of its own */
void Instance_UpdateVisibility(Instance *, const Vec3i *, u16, s32);
#define SDW_INLINE_FREE_INSTANCEHASFLAGS_INSTANCEBASE_U16 1
#define SDW_INLINE_FREE_INSTANCESETFLAGS_INSTANCEBASE_U16 1
#include "instance_inlines.h"
#undef SDW_INLINE_FREE_INSTANCEHASFLAGS_INSTANCEBASE_U16
#undef SDW_INLINE_FREE_INSTANCESETFLAGS_INSTANCEBASE_U16
#define SDW_INLINE_FREE_INSTANCECLEARFLAGS_INSTANCEBASE_U16 1
#include "instance_inlines.h"
#undef SDW_INLINE_FREE_INSTANCECLEARFLAGS_INSTANCEBASE_U16
void WorldObj_DrawHidden(WorldObj *object, Camera *camera)
{
    InstanceClearFlags(object, INST_F_DRAWN);
}
#define DRAW_STATIC_BODY                                                       \
    Mat44 a;                                                                   \
    Mesh *b;                                                                   \
    a.SetIdentity();                                                           \
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_VIEW, &camera->viewMatCopy); \
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_WORLD, &a);                  \
    if (!Cull_IsAabbVisible(&object->aabb, camera) || !object->inst_kind)      \
        InstanceClearFlags(object, INST_F_DRAWN);                              \
    else {                                                                     \
\
        b = (Mesh *)Texture_FindByResource(object->inst_model);                \
        InstanceSetFlags(object, INST_F_DRAWN);                                \
        b->TransformAll();                                                     \
        b->DrawImmediate(g_pPolyBin, g_pViewFrustum);                          \
    }
void WorldObj_DrawStatic(WorldObj *object, Camera *camera)
{
    DRAW_STATIC_BODY
}
void WorldObj_DrawStatic0B(WorldObj *object, Camera *camera)
{
    DRAW_STATIC_BODY
}
inline void SwapInstancePointers(void *&a, void *&b)
{
    void *saved = a;
    a = b;
    b = saved;
}
void Instance_SwapModelAndSecondary(Instance *instance)
{
    SwapInstancePointers((void *&)instance->inst_model, instance->secondaryRes);
}
void Instance_SetRigidTransforms(Instance *instance, const Camera *camera, const Vec3s *scale, const Mat44 *matrix)
{
    Mat44 world_8;
    Mat44 translation_10;
    Vec4i position_14, transformed_7, bounds_6;
    if (matrix)
        world_8 = *matrix;
    else {
        Mat44 rotation;
        if (scale)
            world_8.SetScale(Math_Fixed10ToFloat_s16(scale->x), Math_Fixed10ToFloat_s16(scale->y),
                             Math_Fixed10ToFloat_s16(scale->z));
        else
            world_8.SetIdentity();
        rotation.SetRotXYZ(Math_Angle4096ToRadians_2(instance->rot.x & 0xfff),
                           Math_Angle4096ToRadians_2(instance->rot.y & 0xfff),
                           Math_Angle4096ToRadians_2(instance->rot.z & 0xfff));
        world_8.MulInPlace(rotation);
    }
    translation_10.SetTranslation((float)instance->pos.x, (float)instance->pos.y, (float)instance->pos.z);
    world_8.MulInPlace(translation_10);
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_WORLD, &world_8);
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_VIEW, (Mat44 *)&camera->viewMatCopy);
    bounds_6.x = instance->boundCenter.x;
    bounds_6.y = instance->boundCenter.y;
    bounds_6.z = instance->boundCenter.z;
    position_14.x = instance->pos.x;
    position_14.y = instance->pos.y;
    position_14.z = instance->pos.z;
    transformed_7.x = (s16)(position_14.x * camera->viewMat.m[0][0] + position_14.y * camera->viewMat.m[1][0] +
                            position_14.z * camera->viewMat.m[2][0] + camera->viewMat.m[3][0]);
    transformed_7.y = (s16)(position_14.x * camera->viewMat.m[0][1] + position_14.y * camera->viewMat.m[1][1] +
                            position_14.z * camera->viewMat.m[2][1] + camera->viewMat.m[3][1]);
    transformed_7.z = (s16)(position_14.x * camera->viewMat.m[0][2] + position_14.y * camera->viewMat.m[1][2] +
                            position_14.z * camera->viewMat.m[2][2] + camera->viewMat.m[3][2]);
    bounds_6.x += transformed_7.x;
    bounds_6.y += transformed_7.y;
    bounds_6.z += transformed_7.z;
    Instance_UpdateVisibility(instance, (const Vec3i *)&bounds_6, instance->boundRadius, 0);
}
void Stub_Empty_55d5cb() {}
void Instance_DrawSecondary_Stub(Instance *instance, Camera *camera, const Vec3s *scale, const Mat34s *matrix) {}
void Instance_DrawAnimated(Instance *instance, Animator *animation, Camera *camera, const Vec3s *scale)
{
    Mesh *a;
    Instance_DrawAnimParts(instance, animation, camera, scale);
    if (!InstanceHasFlags(instance, INST_F_DRAWN))
        return;
    a = (Mesh *)Texture_FindByResource(instance->inst_model);
    if (InstanceHasFlags(instance, INST_F_TINT))
        a->Mesh_DrawOutlinedTinted(Color_RgbToBgr(instance->tintColor), Math_Fixed12ToFloat_s16(instance->tintAmount),
                                   g_pPolyBin, g_pViewFrustum, -1.0f);
    else
        a->Mesh_DrawOutlined(g_pPolyBin, g_pViewFrustum, -1.0f);
}
struct HudVertex {
    float x, y, z, rhw;
};
struct HudWork {
    SamScreenGeometry geoB, geoA;
    s32 focal;
    Mesh *mesh;
    u32 bytes, index;
    float depth, minimumZ, dy, dx;
    HudVertex *vertices;
};
void Instance_DrawAnimatedOnScreen(Instance *instance, Animator *animation, Camera *camera, sptr unused3, s32 unused4,
                                   s32 distance, const s16 *screenXY)
{
    HudWork w;
    w.focal = g_projFocalScale;
    g_screen.SetProjection(distance);
    w.dx = HudElement::FromCentreX(screenXY[0] - (g_screen.GetGeometry(&w.geoA)->width >> 1));
    w.dy = HudElement::FromCentreY(screenXY[1] - (g_screen.GetGeometry(&w.geoB)->height >> 1));
    Instance_DrawAnimParts(instance, animation, camera, 0);
    w.mesh = (Mesh *)Texture_FindByResource(instance->inst_model);
    w.mesh->vbTransformed->Lock(DDLOCK_WAIT, (void **)&w.vertices, &w.bytes);
    w.depth = g_screen.Draw2D_LayerToZ(g_screenLayerBase + 6);
    w.minimumZ = 1.0f;
    for (w.index = 0; w.index < w.mesh->vertexCount; w.index++)
        if (w.vertices[w.index].z < w.minimumZ)
            w.minimumZ = w.vertices[w.index].z;
    for (w.index = 0; w.index < w.mesh->vertexCount; w.index++) {
        w.vertices[w.index].x += w.dx;
        w.vertices[w.index].y += w.dy;
        w.vertices[w.index].z += w.depth - w.minimumZ;
        w.vertices[w.index].rhw = 1.0f / (g_pViewFrustum->nearZ);
    }
    w.mesh->vbTransformed->Unlock();
    w.mesh->DrawImmediate(g_pPolyBin, g_pViewFrustum);
    g_screen.SetProjection(w.focal);
}
void Instance_DrawRigid(Instance *instance, Camera *camera, const Vec3s *scale, const Mat34s *matrix)
{
    Mesh *a;
    u32 b;
    Mat44 c;
    if (matrix) {
        c.SetIdentity();
        c.m[0][0] = Math_Fixed12ToFloat_s16(matrix->rot[0]);
        c.m[0][1] = Math_Fixed12ToFloat_s16(matrix->rot[3]);
        c.m[0][2] = Math_Fixed12ToFloat_s16(matrix->rot[6]);
        c.m[1][0] = Math_Fixed12ToFloat_s16(matrix->rot[1]);
        c.m[1][1] = Math_Fixed12ToFloat_s16(matrix->rot[4]);
        c.m[1][2] = Math_Fixed12ToFloat_s16(matrix->rot[7]);
        c.m[2][0] = Math_Fixed12ToFloat_s16(matrix->rot[2]);
        c.m[2][1] = Math_Fixed12ToFloat_s16(matrix->rot[5]);
        c.m[2][2] = Math_Fixed12ToFloat_s16(matrix->rot[8]);
        c.m[3][0] = (float)matrix->trans[0];
        c.m[3][1] = (float)matrix->trans[1];
        c.m[3][2] = (float)matrix->trans[2];
        Instance_SetRigidTransforms(instance, camera, scale, &c);
    } else
        Instance_SetRigidTransforms(instance, camera, scale, 0);
    if (!InstanceHasFlags(instance, INST_F_DRAWN))
        return;
    a = (Mesh *)Texture_FindByResource(instance->inst_model);
    a->TransformAll();
    if (InstanceHasFlags(instance, INST_F_TINT))
        a->Mesh_DrawOutlinedTinted(Color_RgbToBgr(instance->tintColor), Math_Fixed12ToFloat_s16(instance->tintAmount),
                                   g_pPolyBin, g_pViewFrustum, -1.0f);
    else
        a->Mesh_DrawOutlined(g_pPolyBin, g_pViewFrustum, -1.0f);
}
void WorldObj_DrawAnimated(WorldObj *object, Camera *camera)
{
    AnimatedWorldObj *a =
        (AnimatedWorldObj *)object;
    Mesh *b;
    Anim_Advance(&a->anim);
    if (a->anim.pendingSoundId)
        Anim_FireSoundEvent(&a->anim, 0);
    Instance_DrawAnimParts(&a->inst, &a->anim, camera, 0);
    if (!InstanceHasFlags(object, INST_F_DRAWN))
        return;
    b = (Mesh *)Texture_FindByResource(object->inst_model);
    b->Mesh_DrawOutlined(g_pPolyBin, g_pViewFrustum, -1.0f);
}

/* the draw function per WorldObj.inst_kind (Game_RenderStaticWorld); slot 5 is empty */
void (*g_worldObjDrawFns[6])(WorldObj *obj, Camera *cam) = {
    WorldObj_DrawHidden, WorldObj_DrawStatic, WorldObj_DrawStatic0B, WorldObj_DrawSky, WorldObj_DrawAnimated, 0};
