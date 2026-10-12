#include "sdw_types.h"
#include "../sdk/d3d7.h"
class Mat44;

#include "../sdk/crt.h"

#define SDW_MEMBERS_Screen                    \
    Screen(); /* Screen_Construct */ \
    SamScreenGeometry *GetGeometry(SamScreenGeometry *out);

#define SDW_MEMBERS_D3DApp void SetTransform(u32 state, Mat44 *m); /* inline */
struct SamScreenGeometry {
    unsigned short width, height, x, y, aspect;
}; /* as src/objects/sam.cpp */
#include "sdw_classes.h"
#define SDW_INLINE_SCREEN_LAYERS4_U16 1
#define SDW_INLINE_SCREEN_LAYERS60_U16 1
#include "screen_inlines.h"
#undef SDW_INLINE_SCREEN_LAYERS4_U16
#undef SDW_INLINE_SCREEN_LAYERS60_U16
#define SDW_INLINE_D3DAPP_SETTRANSFORM_U32_MAT44 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_SETTRANSFORM_U32_MAT44

#include "fixed_math.h"
#include "draw2d.h"
#include "screen.h"

/* ---- globals ---- */
extern u32 *g_screenLayerBase;

/* ---- the virtual screen (512 x 240, the PS1 resolution) ---- */
Screen g_screen;

/* Render-viewport pixels per virtual unit: the original stretch and the largest contained 4:3 area.
 * Window presentation scales this completed frame separately, preserving its aspect ratio. */
static struct HudScale {
    float w, h, vw, vh;
    float kx, ky, sx, sy;
} s_hudScale;
static HudElement *s_hudElement;

static void Hud_UpdateScale()
{
    s_hudScale.w = g_pViewFrustum->viewportWidth;
    s_hudScale.h = g_pViewFrustum->viewportHeight;
    s_hudScale.vw = g_screen.virtWidth;
    s_hudScale.vh = g_screen.virtHeight;
    s_hudScale.kx = s_hudScale.w / s_hudScale.vw;
    s_hudScale.ky = s_hudScale.h / s_hudScale.vh;
    float areaW = s_hudScale.h * 4.0f / 3.0f;
    float areaH = s_hudScale.w * 3.0f / 4.0f;
    if (areaW > s_hudScale.w)
        areaW = s_hudScale.w;
    if (areaH > s_hudScale.h)
        areaH = s_hudScale.h;
    s_hudScale.sx = areaW / s_hudScale.vw;
    s_hudScale.sy = areaH / s_hudScale.vh;
}

HudElement::HudElement(Anchor x, Anchor y, CentreFn readCentre, const void *centreData)
    : stock(false), resolved(false), ax(x), ay(y), centre(readCentre), data(centreData),
      cx(0), cy(0), mx(0), bx(0), my(0), by(0)
{
    if (!s_hudElement)
        s_hudElement = this;
}

HudElement::HudElement(Anchor x, Anchor y, float centreX, float centreY)
    : HudElement(x, y)
{
    cx = centreX;
    cy = centreY;
}

HudElement::HudElement(Mapping mapping) : HudElement(Start, Start)
{
    stock = mapping == Stretch;
}

HudElement::~HudElement()
{
    if (s_hudElement == this)
        s_hudElement = 0;
}

bool HudElement::Active()
{
    return s_hudElement != 0;
}

static float Hud_AnchorOffset(HudElement::Anchor anchor, float centre, float k, float s, float pixels, float units)
{
    if (anchor == HudElement::Start)
        return 0;
    if (anchor == HudElement::End)
        return pixels - units * s;
    return centre * (k - s);
}

void HudElement::Resolve()
{
    if (stock) {
        mx = s_hudScale.kx;
        my = s_hudScale.ky;
        bx = by = 0;
    } else {
        if (centre)
            centre(data, cx, cy);
        mx = s_hudScale.sx;
        my = s_hudScale.sy;
        bx = Hud_AnchorOffset(ax, cx, s_hudScale.kx, mx, s_hudScale.w, s_hudScale.vw);
        by = Hud_AnchorOffset(ay, cy, s_hudScale.ky, my, s_hudScale.h, s_hudScale.vh);
    }
    resolved = true;
}

float HudElement::MapX(s32 x)
{
    if (!s_hudElement)
        return x * s_hudScale.kx;
    if (!s_hudElement->resolved)
        s_hudElement->Resolve();
    return x * s_hudElement->mx + s_hudElement->bx;
}

float HudElement::MapY(s32 y)
{
    if (!s_hudElement)
        return y * s_hudScale.ky;
    if (!s_hudElement->resolved)
        s_hudElement->Resolve();
    return y * s_hudElement->my + s_hudElement->by;
}

float HudElement::EdgeX(s32 x)
{
    return x <= 0 ? 0 : x >= s_hudScale.vw ? s_hudScale.w : MapX(x);
}

float HudElement::EdgeY(s32 y)
{
    return y <= 0 ? 0 : y >= s_hudScale.vh ? s_hudScale.h : MapY(y);
}

float HudElement::FromCentreX(s32 x)
{
    return MapX(x + ((s32)s_hudScale.vw >> 1)) - s_hudScale.w * 0.5f;
}

float HudElement::FromCentreY(s32 y)
{
    return MapY(y + ((s32)s_hudScale.vh >> 1)) - s_hudScale.h * 0.5f;
}

float HudElement::SizeX(s32 x)
{
    return x * (s_hudElement && s_hudElement->stock ? s_hudScale.kx : s_hudScale.sx);
}

float HudElement::SizeY(s32 y)
{
    return y * (s_hudElement && s_hudElement->stock ? s_hudScale.ky : s_hudScale.sy);
}

void HudElement::CentreScreen(const void *, float &x, float &y)
{
    x = s_hudScale.vw * 0.5f;
    y = s_hudScale.vh * 0.5f;
}

void HudElement::CentreRect(const void *data, float &x, float &y)
{
    const s16 *rect = (const s16 *)data;
    x = rect[0] + rect[2] * 0.5f;
    y = rect[1] + rect[3] * 0.5f;
}

Screen::Screen()
{
    scratch4 = 0;
    scratch60 = 0;
}

Screen::~Screen()
{
    if (scratch4)
        free(scratch4);
    if (scratch60)
        free(scratch60);
}

/* once the device is up: viewport from the frustum's size, virtual screen 512 x 240, projection distance
 * 384, and the two layer-handle tables. 0 when the device is not ready. */
s32 Screen::Init(s32 unused)
{
    if (!g_pD3DAppMain->deviceReady)
        return 0;
    viewportY = 0;
    viewportX = 0;
    viewportWidth = (s16)g_pViewFrustum->viewportWidth;
    viewportHeight = (s16)g_pViewFrustum->viewportHeight;
    virtWidth = 0x200;
    virtHeight = 0xf0;
    SetProjection(0x180);
    scratch4 = malloc(4);
    scratch60 = malloc(0x60);
    Hud_UpdateScale();
    return 1;
}

/* a virtual-screen x in viewport pixels. */
float Screen::ScaleX(s32 x)
{
    return HudElement::MapX(x);
}

/* a virtual-screen y in viewport pixels. */
float Screen::ScaleY(s32 y)
{
    return HudElement::MapY(y);
}

/* the index of a layer handle (g_screenLayerBase + index). */
u16 Screen::LayerIndex(u32 *layer)
{
    return (u16)(((uptr)layer - (uptr)g_screenLayerBase) >> 2);
}

/* the depth of a 2D layer: index / 120. */
float Screen::Draw2D_LayerToZ(u32 *layer)
{
    return LayerIndex(layer) / 120.0f;
}

/* the virtual screen's size, origin 0,0 and 1/aspect in 4.12. */
SamScreenGeometry *Screen::GetGeometry(SamScreenGeometry *out)
{
    SamScreenGeometry geo;
    geo.width = 0x200;
    geo.height = 0xf0;
    geo.x = geo.y = 0;
    geo.aspect = Math_FloatToFixed12_s16(1.0f / g_pViewFrustum->aspect);
    *out = geo;
    return out;
}

/* clears target and z-buffer to color. */
void Screen::Clear(u32 color)
{
    D3DApp *app = g_pD3DAppMain;
    SDW_RD(app->pD3DDevice)->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

/* the PS1-style projection distance: horizontal fov from tan = 256 / dist, aspect = width / height, and
 * the projection matrix (y flipped) pushed to the device.
 * Hor+: the game fixes the horizontal fov, so a screen wider than 4:3 would show less height. There the tangent is
 * widened by (width / height) / (4 / 3), which keeps the 4:3 view's height and adds width at the sides; at 4:3 or
 * narrower it is the game's. The visibility tests (Instance_UpdateVisibility, Cull_IsAabbVisible) read the same
 * tanHalfFov, so objects are kept exactly as far out as the screen shows. projDist stays as the camera set it, so a
 * view saved and set back is not widened twice. Every change of view comes through here. */
void Screen::SetProjection(s32 dist)
{
    float tanHalf;
    float aspect;
    projDist = dist;
    tanHalf = 512.0f / (projDist * 2.0f);
    aspect = g_pViewFrustum->viewportWidth / g_pViewFrustum->viewportHeight;
    if (aspect > 4.0f / 3.0f)
        tanHalf *= aspect / (4.0f / 3.0f);
    g_pViewFrustum->SetFovFromTan(tanHalf);
    g_pViewFrustum->aspect = aspect;
    g_pViewFrustum->BuildProjectionMatrix(&g_projMatrix);
    g_projMatrix.m[1][1] = -g_projMatrix.m[1][1];
    g_pD3DAppMain->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &g_projMatrix);
}
