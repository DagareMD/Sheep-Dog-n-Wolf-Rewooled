/*
 * This file defines g_vdx7Magic and g_davTexturePath.
 */
/*
 * The batcher (g_pPolyBin) is where every RenderPoly ends up. Type 1 (untextured) triangles are appended
 * to one batch; a texture page below immediateTexCount has its own batch; everything else (blend types 2 / 3 and the
 * textures after the immediate prefix) is copied into the sorted pool and drawn back to front by Flush. A batch that
 * fills (batchCapacity triangles) is drawn at once.
 *
 * Render_RestoreLostSurfaces reads the device through an inline accessor that returns a value (see there).
 *
 *, which passes it to the vector-constructor iterator for 0x18-byte elements); it is written as a method
 *    returning `this`, under the table's name.
 */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "../sdk/d3d7.h"
#include "../sdk/ddraw.h"
#include "../sdk/win32.h"
#include "../platform/platform.h"
class Mat44;
#include "../sdk/crt.h"

/* The batcher's vertex formats: FVF 0xc4 (XYZRHW | DIFFUSE | SPECULAR) and 0x1c4 (the same plus one UV pair). Their
 * empty constructors are what make each `new` below run an (empty) element loop. */
struct TlVertexFlat {
    float x, y, z, rhw;
    u32 diffuse, specular;
    TlVertexFlat() {}
};
struct TlVertexTex {
    float x, y, z, rhw;
    u32 diffuse, specular;
    float u, v;
    TlVertexTex() {}
};

#define SDW_MEMBERS_D3DApp                                                                                        \
    void Render_SetStateFlags(u32 flags);                                 /* compiled in emitter.cpp */ \
    void Render_ClearStateFlags(u32 flags);                               /* compiled in emitter.cpp */ \
    void Render_DrawPrimitive(u32 type, u32 fvf, void *verts, u32 count);                          \
    void Render_SetTexture(Texture *tex, u32 stage);                          \
    void SetStateFlagsInline(u32 flags);                                  /* inline twins, defined below */       \
    void ClearStateFlagsInline(u32 flags);                                                                        \
    void DrawPrimitiveInline(u32 type, u32 fvf, void *verts, u32 count);                                          \
    void BindTexture(Texture *tex, s32 stage);                                                                    \
    void ClearTarget(u32 color)                                                                                   \
    {                                                                                                             \
        SDW_RD(pD3DDevice)->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);                              \
    }                                                                                                             \
    void BeginScene()                                                                                             \
    {                                                                                                             \
        SDW_RD(pD3DDevice)->BeginScene();                                                                                 \
    }                                                                                                             \
    void EndScene()                                                                                               \
    {                                                                                                             \
        SDW_RD(pD3DDevice)->EndScene();                                                                                   \
    }                                                                                                             \
    IDirectDraw7 *GetDirectDraw()                                                                                 \
    {                                                                                                             \
        return pDD;                                                                                               \
    } /* inline accessor, see Render_RestoreLostSurfaces */
#define SDW_MEMBERS_PolyBatcher                                                                    \
    PolyBatcher(D3DApp *app, u32 capacity, u32 pageCount); \
    PolyBatcher(D3DApp *app, u32 capacity, const char *davPath, s32 *outPageCount); \
    void SubmitPoly(RenderPoly *poly);
#define SDW_MEMBERS_RenderPoly RenderPoly();                                        /* RenderPoly_Ctor */
#define SDW_MEMBERS_BsStream BsStream(const char *path);                            /* BsStream_Open */
#define SDW_MEMBERS_Texture                                                                                    \
    Texture(D3DApp *app, u32 width, u32 height, u32 format, s32 *result); \
    Texture(const char *fileName, D3DApp *app, u32 width, u32 height, u32 format, s32 *result); \
    void Surface_LockReadWrite(DDSURFACEDESC2 *desc);
#include "sdw_classes.h"

#include "draw2d.h"
#include "mesh.h"
char g_davTexturePath[SDW_PATH_MAX]; /* the .dav path of the loaded texture pages */
char g_vdx7Magic[] = "VDX7"; /* the texture-page file signature */

struct PolyBatcher::OutlineTexture {
    Vdx7Record rect;
    u16 *original;
    u8 outlinesEnabled;
    OutlineTexture *next;
    ~OutlineTexture() { delete[] original; }
};

struct PolyBatcher::OutlineMesh {
    Mesh *mesh;
    u32 normalFaceCount;
    u32 totalFaceCount;
    OutlineMesh *next;
};

int PolyBatcher_CompareSortZ(RenderPoly **a, RenderPoly **b);

/* ---- the inline twins: the engine header's render-device helpers as the expanded sites show them ---- */

#define SDW_INLINE_D3DAPP_DRAWPRIMITIVEINLINE_U32_U32_VOID_U32 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_DRAWPRIMITIVEINLINE_U32_U32_VOID_U32
#define SDW_INLINE_D3DAPP_BINDTEXTURE_TEXTURE_S32 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_BINDTEXTURE_TEXTURE_S32
#define SDW_INLINE_D3DAPP_SETSTATEFLAGSINLINE_U32 1
#include "../app/d3dapp_inlines.h"
#undef SDW_INLINE_D3DAPP_SETSTATEFLAGSINLINE_U32
#define SDW_INLINE_D3DAPP_CLEARSTATEFLAGSINLINE_U32 1
#include "../app/d3dapp_inlines.h"
#include "psx_dav.h"
#undef SDW_INLINE_D3DAPP_CLEARSTATEFLAGSINLINE_U32
/* the constructor without a texture file: pageCount pages, each an immediate page with its own batch
 * (0xc000 state flags). No caller; the loading constructor below is the one Load_DAVnWAR uses. */
PolyBatcher::PolyBatcher(D3DApp *app, u32 capacity, u32 pageCount)
{
    u32 idx;

    outlineTextures = 0;
    outlineMeshes = 0;
    renderer = app;
    batchCapacity = capacity;
    defaultStateFlags = RSF_DITHER | RSF_COLORVERTEX;
    clearColor = 0;
    lineBatchVerts = new TlVertexFlat[4000];
    lineBatchCount = 0;
    lineStateFlags = RSF_ANTIALIAS | RSF_DITHER | RSF_COLORVERTEX;
    flatBatchVerts = new TlVertexFlat[batchCapacity * 3];
    flatBatchCount = 0;
    stateFlags = RSF_DITHER | RSF_COLORVERTEX;
    texturePageCount = pageCount;
    textures = (Texture **)malloc(texturePageCount * sizeof(Texture *));
    texStateFlags = new u32[texturePageCount];
    for (idx = 0; idx < texturePageCount; idx++)
        texStateFlags[idx] = RSF_TEXTURED | RSF_FILTER_LINEAR;
    texBatchVerts = (void **)malloc(texturePageCount * sizeof(void *));
    for (idx = 0; idx < texturePageCount; idx++)
        texBatchVerts[idx] = new TlVertexTex[batchCapacity * 3];
    texBatchCounts = new u32[texturePageCount];
    memset(texBatchCounts, 0, texturePageCount * 4);
    textureDirty = 1;
    sortedPolys = new RenderPoly[6000];
    sortedList = (RenderPoly **)malloc(6000 * sizeof(RenderPoly *));
    computeSortZ = 1;
    sortedCount = 0;
    blendStateFlags = RSF_BLEND_ALPHA;
    addStateFlags = RSF_BLEND_ADD;
}

/* the constructor: the line and untextured batches, then the texture pages from the VDX7 file (its path
 * kept in g_davTexturePath for Render_RestoreLostSurfaces), the sorted pool of 6000 RenderPolys and its pointer list.
 * *outPageCount receives LoadTexturePages' page count (0 on failure). */
PolyBatcher::PolyBatcher(D3DApp *app, u32 capacity, const char *davPath, s32 *outPageCount)
{
    outlineTextures = 0;
    outlineMeshes = 0;
    renderer = app;
    batchCapacity = capacity;
    defaultStateFlags = RSF_DITHER | RSF_COLORVERTEX;
    clearColor = 0;
    lineBatchVerts = new TlVertexFlat[4000];
    lineBatchCount = 0;
    lineStateFlags = RSF_ANTIALIAS | RSF_DITHER | RSF_COLORVERTEX;
    flatBatchVerts = new TlVertexFlat[batchCapacity * 3];
    flatBatchCount = 0;
    stateFlags = RSF_DITHER | RSF_COLORVERTEX;
    strcpy(g_davTexturePath, davPath);
    *outPageCount = LoadTexturePages(davPath);
    textureDirty = 1;
    sortedPolys = new RenderPoly[6000];
    sortedList = (RenderPoly **)malloc(6000 * sizeof(RenderPoly *));
    computeSortZ = 1;
    sortedCount = 0;
    blendStateFlags = RSF_BLEND_ALPHA;
    addStateFlags = RSF_BLEND_ADD;
}

/* frees the batches, the texture pages and the sorted pool. */
PolyBatcher::~PolyBatcher()
{
    u32 i;

    while (outlineTextures) {
        OutlineTexture *next = outlineTextures->next;
        delete outlineTextures;
        outlineTextures = next;
    }
    while (outlineMeshes) {
        OutlineMesh *next = outlineMeshes->next;
        delete outlineMeshes;
        outlineMeshes = next;
    }
    if (lineBatchVerts)
        delete lineBatchVerts;
    for (i = 0; i < texturePageCount; i++) {
        if (textures[i])
            delete textures[i];
    }
    free(textures);
    delete texStateFlags;
    if (flatBatchVerts)
        delete flatBatchVerts;
    for (i = 0; i < immediateTexCount; i++) {
        if (texBatchVerts[i])
            delete texBatchVerts[i];
    }
    free(texBatchVerts);
    delete texBatchCounts;
    if (sortedPolys)
        delete[] sortedPolys;
    if (sortedList)
        free(sortedList);
}

/* the state-flag word of a RenderPoly type: 0 lines, 1 untextured, 2 alpha blend, 3 additive, >= 4 its
 * texture page's. */
u32 PolyBatcher::GetTypeStateFlags(s32 polyType)
{
    u32 page;

    switch (polyType) {
        case RPOLY_LINE:
            return lineStateFlags;
        case RPOLY_OPAQUE:
            return stateFlags;
        case RPOLY_BLEND:
            return blendStateFlags;
        case RPOLY_ADD:
            return addStateFlags;
        default:
            page = polyType - RPOLY_TEXTURED_BASE & ~RPOLY_F_8000;
            return texStateFlags[page];
    }
}

/* sets the state-flag word of a RenderPoly type (the mapping of). */
void PolyBatcher::SetTypeStateFlags(s32 polyType, u32 flags)
{
    u32 page;

    switch (polyType) {
        case RPOLY_LINE:
            lineStateFlags = flags;
            break;
        case RPOLY_OPAQUE:
            stateFlags = flags;
            break;
        case RPOLY_BLEND:
            blendStateFlags = flags;
            break;
        case RPOLY_ADD:
            addStateFlags = flags;
            break;
        default:
            page = polyType - RPOLY_TEXTURED_BASE & ~RPOLY_F_8000;
            texStateFlags[page] = flags;
    }
}

/* clears bits in the state-flag word of a RenderPoly type. */
void PolyBatcher::ClearTypeStateFlags(s32 polyType, u32 flags)
{
    u32 page;

    switch (polyType) {
        case RPOLY_LINE:
            lineStateFlags &= ~flags;
            break;
        case RPOLY_OPAQUE:
            stateFlags &= ~flags;
            break;
        case RPOLY_BLEND:
            blendStateFlags &= ~flags;
            break;
        case RPOLY_ADD:
            addStateFlags &= ~flags;
            break;
        default:
            page = polyType - RPOLY_TEXTURED_BASE & ~RPOLY_F_8000;
            texStateFlags[page] &= ~flags;
    }
}

u32 PolyBatcher::GetDefaultStateFlags()
{
    return defaultStateFlags;
}

void PolyBatcher::SetDefaultStateFlags(u32 flags)
{
    defaultStateFlags = flags;
}

void PolyBatcher::ClearDefaultStateFlags(u32 flags)
{
    defaultStateFlags &= ~flags;
}

u32 PolyBatcher::GetClearColor()
{
    return clearColor;
}

void PolyBatcher::SetClearColor(u32 argb)
{
    clearColor = argb;
}

void PolyBatcher::SetComputeSortZ(u8 enable)
{
    computeSortZ = enable;
}

/* replaces texture page `index` with a texture read from a bitmap file; 4 when the index is out of range,
 * else the constructor's result (on failure the new page is deleted but its pointer is left in the table). */
s32 PolyBatcher::CreateTexture(const char *fileName, u32 index, u32 width, u32 height, u32 format)
{
    s32 result;

    if (index >= texturePageCount) {
        result = TEXRES_OUT_OF_MEMORY;
    } else {
        textures[index] = new Texture(fileName, renderer, width, height, format, &result);
        if (result)
            delete textures[index];
    }
    return result;
}

/* The texture override of page `index` of the page file davPath, made by tools/retexture.py:
 * <data folder>/<folder>/<level folder>/page_NN.png, for <data folder>/Levels/<level folder>/<name>.DAV, RGBA with the
 * usual alpha (opacity); folder is "retexture", or "retexture-psx" for a PlayStation .DAV (converted on load, its pages
 * are not the PC's). Used when it
 * is the page's shape at a whole multiple of its size (2x, 4x...): the page's UVs are fractions of it, so the models
 * draw the same images, sharper. 0 when there is none, or it does not fit (said in the log). */
static Texture *PolyBatcher_LoadPageOverride(D3DApp *renderer, const char *davPath, const char *folder, u32 index,
                                            u32 width, u32 height)
{
    char path[SDW_PATH_MAX];
    s32 seps[3] = {-1, -1, -1}; /* the last three separators: before the file, the level folder, "Levels" */
    s32 i, found = 0, w = 0, h = 0, err, row;
    unsigned char *rgba;
    Texture *page;
    DDSURFACEDESC2 desc;
    for (i = (s32)strlen(davPath) - 1; i >= 0 && found < 3; i--)
        if (davPath[i] == '/' || davPath[i] == '\\')
            seps[found++] = i;
    if (found < 2)
        return 0;
    if (seps[2] >= 0)
        snprintf(path, sizeof(path), "%.*s/%s/%.*s/page_%02u.png", (int)seps[2], davPath, folder,
                 (int)(seps[0] - seps[1] - 1), davPath + seps[1] + 1, (unsigned)index);
    else
        snprintf(path, sizeof(path), "%s/%.*s/page_%02u.png", folder, (int)(seps[0] - seps[1] - 1),
                 davPath + seps[1] + 1, (unsigned)index);
    if (!(rgba = Platform_LoadImageRGBA(path, &w, &h)))
        return 0;
    if (!width || !height || w % (s32)width || h % (s32)height || w / (s32)width != h / (s32)height) {
        Platform_Log("texture override %s: %dx%d is not a whole multiple of the page's %ux%u, not used", path, w, h,
                     (unsigned)width, (unsigned)height);
        Platform_FreeImage(rgba);
        return 0;
    }
    page = new Texture(renderer, (u32)w, (u32)h, TEXFMT_RGBA8, &err);
    if (err) {
        delete page;
        Platform_FreeImage(rgba);
        return 0;
    }
    /* the PNG's alpha is the usual opacity; the game's is transparency (its blending is INVSRCALPHA / SRCALPHA and its
     * alpha test drops high alpha), so it is flipped */
    page->Surface_LockReadWrite(&desc);
    for (row = 0; row < h; row++) {
        u8 *to = (u8 *)desc.lpSurface + row * desc.lPitch;
        const u8 *from = rgba + (size_t)row * w * 4;
        s32 x;
        for (x = 0; x < w * 4; x += 4) {
            to[x] = from[x];
            to[x + 1] = from[x + 1];
            to[x + 2] = from[x + 2];
            to[x + 3] = (u8)(255 - from[x + 3]);
        }
    }
    page->Surface_Unlock();
    Platform_FreeImage(rgba);
    Platform_Log("texture override %s (%dx)", path, w / (s32)width);
    return page;
}

/* page `index` of the page file at stream (its header read: wid x h texels of `format`): its override, or the disc's
 * 16-bit texels. 0 when the texture cannot be made. */
static Texture *PolyBatcher_ReadPage(D3DApp *renderer, BsStream &stream, const char *davPath, u32 index, u32 wid, u32 h,
                                     u32 format, const char *overrideFolder)
{
    Texture *page = PolyBatcher_LoadPageOverride(renderer, davPath, overrideFolder, index, wid, h);
    DDSURFACEDESC2 desc;
    u16 *pix;
    u32 n;
    s32 err;
    if (page) {
        stream.Skip((int)(wid * h * 2)); /* the disc's texels, replaced */
        return page;
    }
    page = new Texture(renderer, wid, h, format & TEXFMT_PIXEL_MASK, &err);
    if (err) {
        delete page;
        return 0;
    }
    page->Surface_LockReadWrite(&desc);
    pix = (u16 *)desc.lpSurface;
    for (n = 0; n < wid * h; n++)
        *pix++ = stream.ReadU16(1);
    page->Surface_Unlock();
    return page;
}

/* reads the VDX7 texture-page file: the immediate pages first (format bits 0x1c == 4), each with its own
 * batch buffer, then the sorted pages (0x08 alpha blend, 0x10 additive), then four blank 4x4 pages. Returns the
 * number of pages read, 0 when the file is missing, not VDX7, or a texture cannot be created. */
s32 PolyBatcher::LoadTexturePages(const char *path)
{
    s32 total;
    u32 iPage;
    u32 j;
    char sig[4];
    bool immediate;
    s32 k;

    total = 0;
    iPage = 0;
    /* the constructor leaves these to this function: cleared, so that a file that is missing or not VDX7 (the
     * PlayStation's .DAV) leaves an empty batcher that the destructor can free */
    texturePageCount = 0;
    immediateTexCount = 0;
    textures = 0;
    texStateFlags = 0;
    texBatchVerts = 0;
    texBatchCounts = 0;
    BsStream stream(path);

    if (stream.ok) {
        sig[0] = stream.ReadU8(1);
        sig[1] = stream.ReadU8(1);
        sig[2] = stream.ReadU8(1);
        sig[3] = stream.ReadU8(1);
        if (strncmp(sig, g_vdx7Magic, 4) == 0) {
            /* a PlayStation .DAV converted on load (psx_dav.h) has its own page overrides: retexture/ is the PC's */
            const char *overrideFolder = stream.ReadU32(1) == PSXDAV_TAG ? "retexture-psx" : "retexture";
            stream.ReadU32(1);
            stream.ReadU32(1);
            stream.ReadU32(1);
            stream.Seek(stream.ReadU32(1));
            stream.ReadU16(1);
            stream.ReadU16(1);
            texturePageCount = stream.ReadU16(1);
            textures = (Texture **)malloc((texturePageCount + 4) * sizeof(Texture *));
            texStateFlags = new u32[texturePageCount + 4];
            stream.ReadU32(1);
            stream.ReadU32(1);
            stream.Seek(stream.ReadU32(1));
            iPage = 0;
            /* the immediate pages (format bits 0x1c == 4) come first */
            do {
                u32 wid;
                u32 format;
                u32 h;

                total++;
                wid = abs(stream.ReadU16(1) - stream.ReadU16(1));
                h = abs(stream.ReadU16(1) - stream.ReadU16(1));
                format = stream.ReadU16(1);
                immediate = (format & TEXFMT_KIND_MASK) == TEXFMT_KIND_IMMEDIATE;
                if (immediate == 1) {
                    Platform_Log("W: %u, H: %u,", wid, h);
                    if (!(textures[iPage] = PolyBatcher_ReadPage(renderer, stream, path, iPage, wid, h, format, overrideFolder)))
                        return 0;
                    texStateFlags[iPage] = RSF_DITHER | RSF_TEXTURED | RSF_FILTER_LINEAR;
                    iPage++;
                }
            } while (iPage < texturePageCount && immediate == 1);
            immediateTexCount = iPage;
            texBatchCounts = new u32[immediateTexCount];
            memset(texBatchCounts, 0, immediateTexCount * 4);
            texBatchVerts = (void **)malloc(immediateTexCount * sizeof(void *));
            for (j = 0; j < immediateTexCount; j++)
                texBatchVerts[j] = new TlVertexTex[batchCapacity * 3];
            /* back to the first sorted page's header (five u16s) */
            stream.Skip(-10);
            do {
                u32 wid;
                u32 format;
                u32 h;

                total++;
                wid = abs(stream.ReadU16(1) - stream.ReadU16(1));
                h = abs(stream.ReadU16(1) - stream.ReadU16(1));
                format = stream.ReadU16(1);
                if (!(textures[iPage] = PolyBatcher_ReadPage(renderer, stream, path, iPage, wid, h, format, overrideFolder)))
                    return 0;
                switch (format & TEXFMT_KIND_MASK) {
                    case TEXFMT_KIND_BLEND:
                        texStateFlags[iPage] = RSF_BLEND_ALPHA | RSF_TEXTURED | RSF_FILTER_LINEAR;
                        break;
                    case TEXFMT_KIND_ADD:
                        texStateFlags[iPage] = RSF_BLEND_ADD | RSF_TEXTURED | RSF_FILTER_LINEAR;
                        break;
                    default:
                        total = 0;
                }
                iPage++;
            } while (iPage < texturePageCount);
            /* four blank 4x4 pages after the file's */
            for (k = 0; k < 4; k++) {
                s32 xx;
                s32 y;
                DDSURFACEDESC2 *ddsd;
                u8 *pixels;
                s32 err;

                total++;
                textures[iPage] = new Texture(renderer, 4, 4, TEXFMT_ARGB4444, &err);
                if (err)
                    return 0;
                ddsd = new DDSURFACEDESC2;
                textures[iPage]->Surface_LockReadWrite(ddsd);
                pixels = (u8 *)ddsd->lpSurface;
                for (y = 0; y < 4; y++)
                    for (xx = 0; xx < 4; xx++)
                        ((u16 *)(y * ddsd->lPitch + (uptr)pixels))[xx] = 0;
                textures[iPage]->Surface_Unlock();
                delete ddsd;
                texStateFlags[iPage] = RSF_BLEND_ALPHA | RSF_TEXTURED | RSF_FILTER_LINEAR;
                iPage++;
                texturePageCount++;
            }
        }
    }
    Platform_Log("LoadTexturePages: %s (%u pages)", path, (unsigned)texturePageCount);
    return total;
}

/* Retain only the explicitly selected tails, once per rectangle even when several faces share it. */
void PolyBatcher::ConfigureOutlineTexture(const Vdx7Record &rect)
{
    if (rect.w <= 0 || rect.h <= 0 || rect.page >= texturePageCount)
        return;
    for (OutlineTexture *entry = outlineTextures; entry; entry = entry->next)
        if (entry->rect.page == rect.page && entry->rect.x == rect.x && entry->rect.y == rect.y &&
            entry->rect.w == rect.w && entry->rect.h == rect.h)
            return;
    Texture *page = textures[rect.page];
    if (!page || page->format != TEXFMT_ARGB4444 || rect.x < 0 || rect.y < 0 ||
        (u32)(rect.x + rect.w) > page->width || (u32)(rect.y + rect.h) > page->height)
        return;
    RdLockedRect locked;
    if (g_renderDevice->LockTexture(page->surface, RD_TEXLOCK_READ, &locked) < 0)
        return;
    OutlineTexture *entry = new OutlineTexture;
    entry->rect = rect;
    entry->original = new u16[rect.w * rect.h];
    entry->outlinesEnabled = 1;
    entry->next = outlineTextures;
    outlineTextures = entry;
    for (s32 y = 0; y < rect.h; ++y) {
        const u16 *row = (const u16 *)((const u8 *)locked.pixels + (rect.y + y) * locked.pitch);
        memcpy(entry->original + y * rect.w, row + rect.x, rect.w * sizeof(u16));
    }
    g_renderDevice->UnlockTexture(page->surface);
}

/* Group the mesh's authored outline triangles at the end of its face array. Every mesh drawing
 * path already respects faceCount, so changing the draw count removes the border geometry without
 * altering any hand texture, vertex position, or animation. Mesh retains ownership of all faces. */
void PolyBatcher::ConfigureOutlineMesh(Mesh *mesh, const u8 *outlineFaces)
{
    u32 normalCount = 0;
    for (u32 i = 0; i < mesh->faceCount; ++i)
        if (!outlineFaces[i])
            ++normalCount;
    if (!normalCount || normalCount == mesh->faceCount)
        return;
    RenderPoly *faces = new RenderPoly[mesh->faceCount];
    u32 normalIndex = 0, outlineIndex = normalCount;
    for (u32 i = 0; i < mesh->faceCount; ++i) {
        const RenderPoly &source = mesh->faces[i];
        RenderPoly &dest = faces[outlineFaces[i] ? outlineIndex++ : normalIndex++];
        dest.type = source.type;
        for (u32 corner = 0; corner < 3; ++corner)
            dest.poly.idx[corner] = source.poly.idx[corner];
        memcpy(dest.verts, source.verts, (source.type & ~RPOLY_F_8000) >= RPOLY_TEXTURED_BASE ? 0x60 : 0x48);
        dest.sortZ = 0; /* computed when a face is submitted for drawing */
    }
    delete[] mesh->faces;
    mesh->faces = faces;
    OutlineMesh *entry = new OutlineMesh;
    entry->mesh = mesh;
    entry->normalFaceCount = normalCount;
    entry->totalFaceCount = mesh->faceCount;
    entry->next = outlineMeshes;
    outlineMeshes = entry;
}

/* Remove authored outline geometry from the draw range and cut dark border pixels out of the
 * selected tail textures. Restore the geometry and original texels when outlines are enabled. */
void PolyBatcher::UpdateObjectOutlines()
{
    for (OutlineMesh *entry = outlineMeshes; entry; entry = entry->next)
        entry->mesh->faceCount = g_meshOutlinesEnabled ? entry->totalFaceCount : entry->normalFaceCount;
    for (OutlineTexture *entry = outlineTextures; entry; entry = entry->next) {
        if (entry->outlinesEnabled == g_meshOutlinesEnabled)
            continue;
        const Vdx7Record &rect = entry->rect;
        Texture *page = textures[rect.page];
        RdLockedRect locked;
        if (g_renderDevice->LockTexture(page->surface, RD_TEXLOCK_READWRITE, &locked) < 0)
            continue;
        for (s32 y = 0; y < rect.h; ++y) {
            u16 *row = (u16 *)((u8 *)locked.pixels + (rect.y + y) * locked.pitch) + rect.x;
            for (s32 x = 0; x < rect.w; ++x) {
                u16 pixel = entry->original[y * rect.w + x];
                if (!g_meshOutlinesEnabled && !(pixel & 0x0888)) /* all RGB channels below 128 */
                    pixel |= 0xf000;
                row[x] = pixel;
            }
        }
        g_renderDevice->UnlockTexture(page->surface);
        entry->outlinesEnabled = g_meshOutlinesEnabled;
    }
}

/* after DDERR_SURFACELOST: restores every DirectDraw surface, frees the texture pages and their batches
 * and reloads them from g_davTexturePath; E_FAIL when the page count changed (or the restore failed).
 *
 * The first statement goes through D3DApp::GetDirectDraw, an inline accessor. A plain free inline `return
 * g_pD3DAppMain->pDD;` matches the same way. Earlier attempts (an inline parameter or local, ~30 shapes) put the
 * pointer before `this`. */
long PolyBatcher::Render_RestoreLostSurfaces()
{
    long hr;
    u32 n;
    u32 oldCount;

    hr = g_pD3DAppMain->GetDirectDraw()->RestoreAllSurfaces();
    if (hr >= 0) {
        OutlineTexture *savedOutlines = outlineTextures;
        outlineTextures = 0;
        oldCount = texturePageCount;
        for (n = 0; n < texturePageCount; n++) {
            if (textures[n]) {
                delete textures[n];
                textures[n] = 0;
            }
        }
        if (textures) {
            free(textures);
            textures = 0;
        }
        for (n = 0; n < immediateTexCount; n++) {
            if (texBatchVerts[n]) {
                delete texBatchVerts[n];
                texBatchVerts[n] = 0;
            }
        }
        if (texBatchVerts) {
            free(texBatchVerts);
            texBatchVerts = 0;
        }
        if (texStateFlags) {
            delete texStateFlags;
            texStateFlags = 0;
        }
        if (texBatchCounts) {
            delete texBatchCounts;
            texBatchCounts = 0;
        }
        s32 loaded = LoadTexturePages(g_davTexturePath);
        if (!loaded || texturePageCount != oldCount)
            hr = E_FAIL;
        while (savedOutlines) {
            OutlineTexture *next = savedOutlines->next;
            if (hr >= 0)
                ConfigureOutlineTexture(savedOutlines->rect);
            delete savedOutlines;
            savedOutlines = next;
        }
    }
    return hr;
}

/* whether a texture page has its own immediate batch. */
bool PolyBatcher::IsImmediateTexture(u32 textureIndex)
{
    return textureIndex < immediateTexCount;
}

/* clears target and z (clearColor, z = 1.0), begins the scene and applies the default state flags. */
void PolyBatcher::Render_BeginFrame()
{
    UpdateObjectOutlines();
    renderer->ClearTarget(clearColor);
    renderer->BeginScene();
    textureDirty = 1;
    renderer->SetStateFlagsInline(defaultStateFlags);
}

/* flush == 1 draws what is queued; anything else drops it. Ends the scene either way. */
void PolyBatcher::Render_EndFrame(u8 flush)
{
    if (flush == 1) {
        Flush();
    } else {
        lineBatchCount = 0;
        flatBatchCount = 0;
        sortedCount = 0;
        memset(texBatchCounts, 0, immediateTexCount * 4);
    }
    renderer->EndScene();
}

/* presents the frame (Blt into the window's client rectangle when the mode is the desktop-compatible
 * windowed one, Flip otherwise), recovers lost surfaces, then waits out the frame limiter. */
void PolyBatcher::Render_Present(u32 maxFps)
{
    D3DApp *d3dApp;
    long hr;

    d3dApp = renderer;
    hr = g_renderDevice->Present(); /* the renderer's present, in place of the Blt / Flip below */
    if (hr == DDERR_SURFACELOST)
        Render_RestoreLostSurfaces();
    renderer->Frame_LimitFps(maxFps);
}

/* draws everything still queued: the untextured batch, each immediate page's batch, the lines, then the
 * sorted list back to front (qsort by sortZ, descending), and empties all of them. */
void PolyBatcher::Flush()
{
    u32 page;
    u32 tex;
    u32 k;

    if (flatBatchCount > 0) {
        renderer->SetStateFlagsInline(stateFlags);
        renderer->DrawPrimitiveInline(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                      flatBatchVerts, flatBatchCount * 3);
        renderer->ClearStateFlagsInline(stateFlags);
        textureDirty = 1;
    }
    for (tex = 0; tex < immediateTexCount; tex++) {
        if (texBatchCounts[tex] > 0) {
            if (tex != lastTextureIndex || textureDirty == 1) {
                renderer->BindTexture(textures[tex], 0);
                lastTextureIndex = tex;
                textureDirty = 0;
            }
            renderer->Render_SetStateFlags(texStateFlags[tex]);
            renderer->DrawPrimitiveInline(D3DPT_TRIANGLELIST,
                                          D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                          texBatchVerts[tex], texBatchCounts[tex] * 3);
            renderer->Render_ClearStateFlags(texStateFlags[tex]);
        }
    }
    if (lineBatchCount > 0) {
        renderer->Render_SetStateFlags(lineStateFlags);
        renderer->DrawPrimitiveInline(D3DPT_LINELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR, lineBatchVerts,
                                      lineBatchCount * 2);
        renderer->Render_ClearStateFlags(lineStateFlags);
        textureDirty = 1;
    }
    renderer->Render_SetStateFlags(RSF_ZWRITE_OFF);
    qsort(sortedList, sortedCount, sizeof(RenderPoly *), (int (*)(const void *, const void *))PolyBatcher_CompareSortZ);
    for (k = 0; k < sortedCount; k++) {
        switch (sortedList[k]->type) {
            case RPOLY_BLEND:
                renderer->Render_SetStateFlags(blendStateFlags);
                renderer->DrawPrimitiveInline(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                              sortedList[k]->verts, 3);
                renderer->Render_ClearStateFlags(blendStateFlags);
                textureDirty = 1;
                break;
            case RPOLY_ADD:
                renderer->Render_SetStateFlags(addStateFlags);
                renderer->DrawPrimitiveInline(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR,
                                              sortedList[k]->verts, 3);
                renderer->Render_ClearStateFlags(addStateFlags);
                textureDirty = 1;
                break;
            default:
                page = sortedList[k]->type - RPOLY_TEXTURED_BASE & ~RPOLY_F_8000;
                if (page != lastTextureIndex || textureDirty == 1) {
                    renderer->BindTexture(textures[page], 0);
                    lastTextureIndex = page;
                    textureDirty = 0;
                }
                renderer->Render_SetStateFlags(texStateFlags[page]);
                renderer->DrawPrimitiveInline(D3DPT_TRIANGLELIST,
                                              D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,
                                              sortedList[k]->verts, 3);
                renderer->Render_ClearStateFlags(texStateFlags[page]);
        }
    }
    renderer->Render_ClearStateFlags(RSF_ZWRITE_OFF | RSF_TEXTURED);
    lineBatchCount = 0;
    flatBatchCount = 0;
    sortedCount = 0;
    memset(texBatchCounts, 0, immediateTexCount * 4);
}

/* qsort comparator: larger sortZ first (back to front). */
int PolyBatcher_CompareSortZ(RenderPoly **a, RenderPoly **b)
{
    if ((*a)->sortZ == (*b)->sortZ)
        return 0;
    else if ((*a)->sortZ > (*b)->sortZ)
        return -1;
    else
        return 1;
}
