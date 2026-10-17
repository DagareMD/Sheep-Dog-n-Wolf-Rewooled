/*
 * Rewooled: the language wheel's disc cut into a slice per language (langpack.h), in the wheel model itself.
 *
 * The model (ALANGUE1) is a twelve-sided disc on joint 1: a coloured rim and, on its front face, four pictures, each a
 * fan of triangles from the disc's centre, language k's at k quarter turns (English, Portuguese, Spanish, Italian, the
 * original wheel's order). When the Wheel level is loaded, the model's mesh is rebuilt: its four picture fans give way
 * to N slices, one per language (langpack.h), language k's centred at k Nths of a turn, so that the wheel code's
 * index * (2 pi / N) brings it to the front as index * pi/2 did; the rim and everything else stay as they are. A
 * slice's outer edge runs from the rim at its one side, through every corner of the rim between, to the rim at its
 * other side, so the slices cover the front face exactly. Being the model's own triangles, the slices are
 * drawn wherever the model is: the wheel, its reflection on the floor.
 *
 * Everything is read from the model as loaded: the front face's plane, its rim, and each picture's texture page,
 * colour and texture coordinates by angle. A slice shows its language's picture: a pack's wheel.png (a page of the
 * batcher's of its own, polybatcher.h); else the game's own for the four the disc has (the game's own languages);
 * else the save screen's empty-slot sheep icon on a plain slice, in the disc's own colour (its rim's). A picture is cropped to the middle of its slice when the
 * slice is narrower than a quarter turn, and stretched across it when wider.
 */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "../sdk/d3d7.h"
#include "../sdk/ddraw.h"
#include "../sdk/win32.h"
#include "scenaric_props.h"
class Mat44;
#include "../sdk/crt.h"
#define IsEqualGUID(a, b) (!memcmp((a), (b), 16)) /* sizeof(GUID); the type is incomplete here */
/* the C library's mathematics, declared as src/sdk/crt.h declares the rest (its own headers would clash with crt.h) */
extern "C" double sin(double);
extern "C" double cos(double);
extern "C" double tan(double);

#define SDW_MEMBERS_RenderPoly                                              \
    RenderPoly();                            /* RenderPoly_Ctor */ \
    /* virtual ~RenderPoly() is generated */ /* RenderPoly_Dtor */
#define SDW_MEMBERS_D3DApp                                                  \
    void CreateVB(D3DVERTEXBUFFERDESC *desc, SdwVertexBuffer **out); \
    IDirect3DDevice7 *GetDevice();
#include "sdw_classes.h"
#define SDW_INLINE_D3DAPP_CREATEVB_D3DVERTEXBUFFERDESC_IDIRECT3DVERTEXBUFFER7 1
#define SDW_INLINE_D3DAPP_GETDEVICE 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_CREATEVB_D3DVERTEXBUFFERDESC_IDIRECT3DVERTEXBUFFER7
#undef SDW_INLINE_D3DAPP_GETDEVICE

#include "../engine/load_dav.h"
#include "../engine/load_warmeshes.h"
#include "../engine/draw2d.h"
#include "../engine/game_state.h"
#include "../engine/tex_table.h"
#include "../engine/interface.h"
#include "../engine/langpack.h"
#include "../engine/polybatcher.h"
#include "../platform/platform.h"
#include "scene_wheel_disc.h"

/* The DAV directory (DavHeader.dir): packed, its pointers sit at +6/+0xa/+0x12 (src/engine/load_dav.cpp). */
#include "sdw_fileptr.h"
#pragma pack(push, 1)
struct DavDirectory {
    u16 indexCount;
    u16 bitmapCount;
    u16 unk04;
    SDW_DAVPTR(u16) indices;
    SDW_DAVPTR(DavBitmapRec) bitmaps;
    u32 fileSize;
    SDW_DAVPTR(u32) idLists;
};
#pragma pack(pop)

uptr *Res_GetValidatedIdList(u16 resId, u16 *outCount);

namespace {

/* a face's vertices (render_poly.cpp): textured, and flat */
struct TexVertex {
    float x, y, z, rhw;
    u32 color, specular;
    float u, v;
};
struct FlatVertex {
    float x, y, z, rhw;
    u32 color, specular;
};
struct Point {
    float x, y, z;
};

const float kPi = 3.14159265f, kQuarter = 1.5707964f;

float Min(float a, float b)
{
    return a < b ? a : b;
}

float Max(float a, float b)
{
    return a > b ? a : b;
}

float Abs(float a)
{
    return a < 0 ? -a : a;
}

float WrapAngle(float a)
{
    while (a > kPi)
        a -= 2 * kPi;
    while (a < -kPi)
        a += 2 * kPi;
    return a;
}

/* one of the model's four pictures: its page and colour, its centre's texture coordinates and its rim's, by angle from
 * the picture's middle */
struct Picture {
    bool found = false;
    u32 page = 0;
    u32 color = 0x808080;
    float apexU = 0, apexV = 0;
    s32 rimCount = 0;
    float rimAngle[8], rimU[8], rimV[8];

    void Add(float angle, float u, float v)
    {
        for (s32 i = 0; i < rimCount; i++)
            if (Abs(rimAngle[i] - angle) < 0.001f)
                return;
        if (rimCount == 8)
            return;
        s32 at = rimCount++;
        while (at > 0 && rimAngle[at - 1] > angle) {
            rimAngle[at] = rimAngle[at - 1];
            rimU[at] = rimU[at - 1];
            rimV[at] = rimV[at - 1];
            at--;
        }
        rimAngle[at] = angle;
        rimU[at] = u;
        rimV[at] = v;
    }

    /* the texture coordinates of the rim at angle a from the picture's middle (held to its two edges) */
    void RimUV(float a, float *u, float *v) const
    {
        if (rimCount == 0) {
            *u = apexU;
            *v = apexV;
            return;
        }
        if (a <= rimAngle[0]) {
            *u = rimU[0];
            *v = rimV[0];
            return;
        }
        for (s32 i = 1; i < rimCount; i++)
            if (a <= rimAngle[i]) {
                float t = (a - rimAngle[i - 1]) / (rimAngle[i] - rimAngle[i - 1]);
                *u = rimU[i - 1] + (rimU[i] - rimU[i - 1]) * t;
                *v = rimV[i - 1] + (rimV[i] - rimV[i - 1]) * t;
                return;
            }
        *u = rimU[rimCount - 1];
        *v = rimV[rimCount - 1];
    }
};

/* the disc as the model has it */
struct Disc {
    float frontZ = 0, outward = 1;
    s32 rimCount = 0;
    float rimAngle[64], rimRadius[64];
    Picture pictures[4];
    float winding = 1; /* the sign of a front-face triangle's (b - a) x (c - a), as the model has its pictures */
};

/* the rim's distance from the centre at angle a: on the chord between the two rim corners around it */
float RimRadiusAt(const Disc &d, float a)
{
    if (d.rimCount < 3)
        return d.rimCount ? d.rimRadius[0] : 0;
    a = WrapAngle(a);
    for (s32 i = 0; i < d.rimCount; i++) {
        s32 j = (i + 1) % d.rimCount;
        float a0 = d.rimAngle[i], a1 = d.rimAngle[j];
        float span = WrapAngle(a1 - a0), off = WrapAngle(a - a0);
        if (span > 0 && off >= 0 && off <= span) {
            float x0 = (float)cos(a0) * d.rimRadius[i], y0 = (float)sin(a0) * d.rimRadius[i];
            float x1 = (float)cos(a1) * d.rimRadius[j], y1 = (float)sin(a1) * d.rimRadius[j];
            float dx = (float)cos(a), dy = (float)sin(a), ex = x1 - x0, ey = y1 - y0, den = dx * ey - dy * ex;
            if (Abs(den) < 1e-6f)
                return d.rimRadius[i];
            return (x0 * ey - y0 * ex) / den; /* the ray t * (dx, dy) meets the chord */
        }
    }
    return d.rimRadius[0];
}

/* the mesh this file last rebuilt, and its vertex count then: the wheel is rebuilt once a load */
Mesh *s_rebuilt = 0;
u32 s_rebuiltVertices = 0;

} /* namespace */

void SceneWheel_BuildDisc(Instance *wheel)
{
    Model *model = wheel->inst_model;
    Mesh *mesh = model ? (Mesh *)Texture_FindByResource(model) : 0;
    s32 languages = LangPack_Count();
    if (!mesh || model->nbJoints < 2 || !mesh->vbPositions || !mesh->faces || languages <= 0)
        return;
    if (mesh == s_rebuilt && mesh->vertexCount == s_rebuiltVertices)
        return; /* already, this load */
    ModelJoint *joints = model->joints;
    u32 discVertices = joints[1].vertexCount; /* the disc's: joint 1's, the mesh's first */
    if (!discVertices || discVertices > mesh->vertexCount)
        return;

    /* the model's vertices */
    u32 oldCount = mesh->vertexCount;
    Point *old = new Point[oldCount];
    {
        void *lock;
        u32 size;
        mesh->vbPositions->Lock(DDLOCK_WAIT, &lock, &size);
        memcpy(old, lock, oldCount * sizeof(Point));
        mesh->vbPositions->Unlock();
    }

    /* the front face: the plane of the vertex at the centre; its rim: the front face's corners away from it */
    Disc d;
    u32 centre = 0;
    float best = 1e30f, maxRadius = 0;
    for (u32 i = 0; i < discVertices; i++) {
        float r = old[i].x * old[i].x + old[i].y * old[i].y;
        if (r < best)
            best = r, centre = i;
        maxRadius = Max(maxRadius, r);
    }
    maxRadius = (float)sqrt(maxRadius);
    d.frontZ = old[centre].z;
    d.outward = d.frontZ >= 0 ? 1.0f : -1.0f;
    float flat = 0.02f * maxRadius;
    for (u32 i = 0; i < discVertices && d.rimCount < 64; i++) {
        float r = (float)sqrt(old[i].x * old[i].x + old[i].y * old[i].y);
        if (Abs(old[i].z - d.frontZ) > flat || r < 0.5f * maxRadius)
            continue;
        float a = (float)atan2(old[i].y, old[i].x);
        bool seen = false;
        for (s32 k = 0; k < d.rimCount; k++)
            seen |= Abs(WrapAngle(d.rimAngle[k] - a)) < 0.001f;
        if (seen)
            continue;
        s32 at = d.rimCount++;
        while (at > 0 && d.rimAngle[at - 1] > a) {
            d.rimAngle[at] = d.rimAngle[at - 1];
            d.rimRadius[at] = d.rimRadius[at - 1];
            at--;
        }
        d.rimAngle[at] = a;
        d.rimRadius[at] = r;
    }
    if (d.rimCount < 3) {
        Platform_Log("Language wheel: the disc's rim was not found; the model's own four pictures are kept");
        delete[] old;
        return;
    }

    /* the pictures: the front face's textured triangles, each the quarter turn its middle is in */
    bool *picture = new bool[mesh->faceCount];
    u32 pictureFaces = 0;
    for (u32 f = 0; f < mesh->faceCount; f++) {
        RenderPoly &face = mesh->faces[f];
        u32 type = face.type & ~RPOLY_F_8000;
        const u32 *idx = face.poly.idx;
        picture[f] = false;
        if (type < RPOLY_TEXTURED_BASE || idx[0] >= discVertices || idx[1] >= discVertices || idx[2] >= discVertices)
            continue;
        float cx = 0, cy = 0;
        bool front = true;
        for (u32 k = 0; k < 3; k++) {
            front &= Abs(old[idx[k]].z - d.frontZ) <= flat;
            cx += old[idx[k]].x;
            cy += old[idx[k]].y;
        }
        if (!front)
            continue;
        picture[f] = true;
        pictureFaces++;
        s32 quarter = ((s32)floor((float)atan2(cy, cx) / kQuarter + 0.5f) + 4) % 4;
        Picture &p = d.pictures[quarter];
        const TexVertex *tv = (const TexVertex *)face.verts;
        p.found = true;
        p.page = type - RPOLY_TEXTURED_BASE;
        p.color = tv[0].color;
        for (u32 k = 0; k < 3; k++) {
            const Point &v = old[idx[k]];
            if ((float)sqrt(v.x * v.x + v.y * v.y) < 0.25f * maxRadius) {
                p.apexU = tv[k].u;
                p.apexV = tv[k].v;
            } else
                p.Add(WrapAngle((float)atan2(v.y, v.x) - quarter * kQuarter), tv[k].u, tv[k].v);
        }
        const Point &a = old[idx[0]], &b = old[idx[1]], &c = old[idx[2]];
        d.winding = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) >= 0 ? 1.0f : -1.0f;
    }

    /* a plain slice's colour: the disc's own, its untextured faces' (the rim's), the most of them */
    u32 plain = 0x808080, plainFaces = 0;
    for (u32 f = 0; f < mesh->faceCount; f++) {
        const RenderPoly &face = mesh->faces[f];
        const u32 *idx = face.poly.idx;
        if ((face.type & ~RPOLY_F_8000) != RPOLY_OPAQUE || idx[0] >= discVertices || idx[1] >= discVertices ||
            idx[2] >= discVertices)
            continue;
        u32 color = ((const FlatVertex *)face.verts)[0].color, same = 0;
        for (u32 g = 0; g < mesh->faceCount; g++) {
            const u32 *gi = mesh->faces[g].poly.idx;
            same += (mesh->faces[g].type & ~RPOLY_F_8000) == RPOLY_OPAQUE && gi[0] < discVertices &&
                    gi[1] < discVertices && gi[2] < discVertices &&
                    ((const FlatVertex *)mesh->faces[g].verts)[0].color == color;
        }
        if (same > plainFaces)
            plain = color, plainFaces = same;
    }

    /* a pack's picture is mapped as the disc's first, in 0..1 of the picture (and drawn in its colour) */
    Picture shape = d.pictures[0];
    if (shape.found) {
        float u0 = shape.apexU, u1 = u0, v0 = shape.apexV, v1 = v0;
        for (s32 i = 0; i < shape.rimCount; i++) {
            u0 = Min(u0, shape.rimU[i]);
            u1 = Max(u1, shape.rimU[i]);
            v0 = Min(v0, shape.rimV[i]);
            v1 = Max(v1, shape.rimV[i]);
        }
        float du = u1 > u0 ? u1 - u0 : 1, dv = v1 > v0 ? v1 - v0 : 1;
        shape.apexU = (shape.apexU - u0) / du;
        shape.apexV = (shape.apexV - v0) / dv;
        for (s32 i = 0; i < shape.rimCount; i++) {
            shape.rimU[i] = (shape.rimU[i] - u0) / du;
            shape.rimV[i] = (shape.rimV[i] - v0) / dv;
        }
    } else { /* no picture on the disc: the shape of the game's */
        shape.apexU = 0.5f;
        shape.apexV = 0;
        shape.Add(-kQuarter / 2, 0, 0.73f);
        shape.Add(-kQuarter / 6, 0.32f, 1);
        shape.Add(kQuarter / 6, 0.68f, 1);
        shape.Add(kQuarter / 2, 1, 0.73f);
        shape.color = 0x808080;
    }

    /* the save screen's empty-slot icon (mcard.cpp: icon 19, frame 9 of the second icon group) */
    DavBitmapRec *icon = 0;
    {
        u16 n = 0;
        uptr *list = Res_GetValidatedIdList(DAV_IDI_IMEMCICB, &n);
        if (list && n) {
            u16 frames = 0;
            void **group = Res_FindBitmapGroup(&g_pDav->header->dir->bitmaps[*(u16 *)*list], &frames);
            if (group && frames > 9)
                icon = (DavBitmapRec *)group[9];
        }
    }

    /* what each slice shows: a picture of the disc's or a pack's (on its page), else the icon on a plain slice */
    s32 slices = languages;
    const Picture **pictures = new const Picture *[slices];
    s32 *pages = new s32[slices];
    s32 packPictures = 0, icons = 0;
    for (s32 s = 0; s < slices; s++) {
        /* a pack's own picture (wheel.png), else the disc's of the game's own language */
        const char *png = LangPack_WheelPicture(s);
        s32 slot = LangPack_WheelSlot(s);
        pictures[s] = 0;
        pages[s] = png && *png ? PolyBatcher_AddImagePage(png, 0) : -1;
        if (pages[s] >= 0) {
            pictures[s] = &shape;
            packPictures++;
        } else if (slot >= 0 && slot < 4 && d.pictures[slot].found) {
            pictures[s] = &d.pictures[slot];
            pages[s] = (s32)pictures[s]->page;
        }
        icons += !pictures[s] && icon;
    }

    /* each slice's outer edge, by angle from its middle: its one side, the rim's corners between in order, its other */
    float step = 2 * kPi / slices, half = step / 2;
    s32 *rimPoints = new s32[slices];
    float *edge = new float[(size_t)slices * (d.rimCount + 2)];
    u32 rimTotal = 0;
    for (s32 s = 0; s < slices; s++) {
        float *e = edge + (size_t)s * (d.rimCount + 2);
        s32 n = 0;
        e[n++] = -half;
        for (s32 k = 0; k < d.rimCount; k++) {
            float off = d.rimAngle[k] - s * step + half; /* from the slice's one side, within a turn */
            while (off < 0)
                off += 2 * kPi;
            while (off >= 2 * kPi)
                off -= 2 * kPi;
            if (off < 1e-4f || off > step - 1e-4f)
                continue; /* on its sides, or outside it */
            s32 at = n;
            while (at > 1 && e[at - 1] > off - half) {
                e[at] = e[at - 1];
                at--;
            }
            e[at] = off - half;
            n++;
        }
        e[n++] = half;
        rimPoints[s] = n;
        rimTotal += (u32)n;
    }

    /* the new vertices, after the disc's: the centre; per slice its outer edge; per icon its four corners */
    u32 added = 1 + rimTotal + (u32)icons * 4;
    Point *fresh = new Point[added];
    u32 at = 0;
    fresh[at++] = {0, 0, d.frontZ};
    s32 *firstRim = new s32[slices], *firstIcon = new s32[slices];
    float lift = 0.004f * maxRadius * d.outward; /* the icon just in front of its slice */
    for (s32 s = 0; s < slices; s++) {
        float middle = s * step;
        const float *e = edge + (size_t)s * (d.rimCount + 2);
        firstRim[s] = (s32)at;
        for (s32 k = 0; k < rimPoints[s]; k++) {
            float a = middle + e[k], r = RimRadiusAt(d, a);
            fresh[at++] = {(float)cos(a) * r, (float)sin(a) * r, d.frontZ};
        }
        firstIcon[s] = -1;
        if (!pictures[s] && icon) {
            /* upright in the slice (its top towards the centre), as large as the slice allows */
            float radius = RimRadiusAt(d, middle);
            float aspect = (float)icon->width / (float)(icon->height ? icon->height : 1);
            float centreR = 0.62f * radius, t = (float)tan(Min(half, 1.2f)) * 0.85f;
            float height = Min(0.45f * radius, 2 * t * centreR / (aspect + t)), width = height * aspect;
            float dx = (float)cos(middle), dy = (float)sin(middle), tx = -dy, ty = dx;
            float top = centreR - height / 2, bottom = centreR + height / 2, z = d.frontZ + lift;
            firstIcon[s] = (s32)at;
            fresh[at++] = {dx * top - tx * width / 2, dy * top - ty * width / 2, z};
            fresh[at++] = {dx * top + tx * width / 2, dy * top + ty * width / 2, z};
            fresh[at++] = {dx * bottom - tx * width / 2, dy * bottom - ty * width / 2, z};
            fresh[at++] = {dx * bottom + tx * width / 2, dy * bottom + ty * width / 2, z};
        }
    }

    /* the new vertex buffers: the disc's vertices, the new ones, then the other joints' (their indices moved on) */
    u32 newCount = oldCount + added;
    SdwVertexBuffer *positions = 0, *transformed = 0;
    {
        D3DVERTEXBUFFERDESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwCaps = D3DVBCAPS_DONOTCLIP;
        desc.dwFVF = D3DFVF_XYZ;
        desc.dwNumVertices = newCount;
        g_pD3DAppMain->CreateVB(&desc, &positions);
        desc.dwFVF = D3DFVF_XYZRHW;
        g_pD3DAppMain->CreateVB(&desc, &transformed);
    }
    if (!positions || !transformed) {
        if (positions)
            positions->Release();
        if (transformed)
            transformed->Release();
        Platform_Log("Language wheel: no vertex buffer for the disc; the model's own four pictures are kept");
        delete[] old;
        delete[] fresh;
        delete[] picture;
        delete[] pictures;
        delete[] pages;
        delete[] firstRim;
        delete[] firstIcon;
        delete[] rimPoints;
        delete[] edge;
        return;
    }
    {
        void *lock;
        u32 size;
        positions->Lock(DDLOCK_WAIT, &lock, &size);
        Point *to = (Point *)lock;
        memcpy(to, old, discVertices * sizeof(Point));
        memcpy(to + discVertices, fresh, added * sizeof(Point));
        memcpy(to + discVertices + added, old + discVertices, (oldCount - discVertices) * sizeof(Point));
        positions->Unlock();
    }
    auto moved = [&](u32 index) { return index < discVertices ? index : index + added; };

    /* the new faces: the model's, but its pictures; then the slices' */
    u32 sliceFaces = rimTotal - (u32)slices + (u32)icons * 2;
    u32 newFaceCount = mesh->faceCount - pictureFaces + sliceFaces;
    RenderPoly *faces = new RenderPoly[newFaceCount];
    u32 nf = 0;
    for (u32 f = 0; f < mesh->faceCount; f++) {
        if (picture[f])
            continue;
        faces[nf].Assign(&mesh->faces[f]);
        for (u32 k = 0; k < 3; k++)
            faces[nf].poly.idx[k] = moved(mesh->faces[f].poly.idx[k]);
        nf++;
    }
    /* a triangle of new vertices a, b, c, wound as the model's front face is */
    auto triangle = [&](u32 a, u32 b, u32 c) -> RenderPoly & {
        const Point &pa = fresh[a], &pb = fresh[b], &pc = fresh[c];
        float w = (pb.x - pa.x) * (pc.y - pa.y) - (pb.y - pa.y) * (pc.x - pa.x);
        RenderPoly &face = faces[nf++];
        bool swap = (w >= 0 ? 1.0f : -1.0f) != d.winding;
        face.poly.idx[0] = discVertices + a;
        face.poly.idx[1] = discVertices + (swap ? c : b);
        face.poly.idx[2] = discVertices + (swap ? b : c);
        face.sortZ = 0;
        return face;
    };
    for (s32 s = 0; s < slices; s++) {
        /* a picture narrower than its slice is stretched across it; one wider is cropped to its middle */
        float scale = half > kQuarter / 2 ? (kQuarter / 2) / half : 1.0f;
        const float *e = edge + (size_t)s * (d.rimCount + 2);
        for (s32 k = 0; k + 1 < rimPoints[s]; k++) {
            u32 b = (u32)firstRim[s] + k, c = b + 1;
            float ub, vb, uc, vc;
            bool swap;
            {
                const Point &pa = fresh[0], &pb = fresh[b], &pc = fresh[c];
                float w = (pb.x - pa.x) * (pc.y - pa.y) - (pb.y - pa.y) * (pc.x - pa.x);
                swap = (w >= 0 ? 1.0f : -1.0f) != d.winding;
            }
            RenderPoly &face = triangle(0, b, c);
            if (pictures[s]) {
                const Picture &p = *pictures[s];
                p.RimUV(e[k] * scale, &ub, &vb);
                p.RimUV(e[k + 1] * scale, &uc, &vc);
                TexVertex *v = (TexVertex *)face.verts;
                float us[3] = {p.apexU, swap ? uc : ub, swap ? ub : uc}, vs[3] = {p.apexV, swap ? vc : vb, swap ? vb : vc};
                for (s32 i = 0; i < 3; i++) {
                    v[i].color = p.color;
                    v[i].specular = 0xff000000;
                    v[i].u = us[i];
                    v[i].v = vs[i];
                }
                face.type = (u32)pages[s] + RPOLY_TEXTURED_BASE;
            } else {
                FlatVertex *v = (FlatVertex *)face.verts;
                for (s32 i = 0; i < 3; i++) {
                    v[i].color = plain;
                    v[i].specular = 0xff000000;
                }
                face.type = RPOLY_OPAQUE;
            }
        }
        if (firstIcon[s] >= 0) {
            float u0 = Tex_CornerUV(0, icon->width - 1, icon->u), u1 = Tex_CornerUV(icon->width - 1, icon->width - 1, icon->u);
            float v0 = Tex_CornerUV(0, icon->height - 1, icon->v), v1 = Tex_CornerUV(icon->height - 1, icon->height - 1, icon->v);
            const float cu[4] = {u0, u1, u0, u1}, cv[4] = {v0, v0, v1, v1};
            const s32 tris[2][3] = {{0, 1, 2}, {1, 3, 2}};
            for (const auto &tri : tris) {
                u32 q = (u32)firstIcon[s];
                RenderPoly &face = triangle(q + tri[0], q + tri[1], q + tri[2]);
                TexVertex *v = (TexVertex *)face.verts;
                for (s32 i = 0; i < 3; i++) {
                    u32 corner = face.poly.idx[i] - discVertices - q;
                    v[i].color = 0x808080;
                    v[i].specular = 0xff000000;
                    v[i].u = cu[corner];
                    v[i].v = cv[corner];
                }
                face.type = TexAtlas_GetPage(icon) + RPOLY_TEXTURED_BASE;
            }
        }
    }

    /* in place of the model's */
    mesh->vbPositions->Release();
    mesh->vbTransformed->Release();
    mesh->vbPositions = positions;
    mesh->vbTransformed = transformed;
    mesh->vertexCount = newCount;
    delete[] mesh->faces;
    mesh->faces = faces;
    mesh->faceCount = nf;
    joints[1].vertexCount = (u16)(discVertices + added);
    s_rebuilt = mesh;
    s_rebuiltVertices = newCount;

    s32 found = 0;
    for (const Picture &p : d.pictures)
        found += p.found;
    Platform_Log("Language wheel: %d slices on the model's disc (%d rim corners, %d of its pictures, %d packs' pictures, "
                 "%d with the empty-slot icon)",
                 slices, d.rimCount, found, packPictures, icons);
    delete[] old;
    delete[] fresh;
    delete[] picture;
    delete[] pictures;
    delete[] pages;
    delete[] firstRim;
    delete[] firstIcon;
    delete[] rimPoints;
    delete[] edge;
}
