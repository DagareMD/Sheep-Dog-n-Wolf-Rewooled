/*
 *  - It is a macro for that literal.
 *  - Texture::Surface_LockForWrite is declared returning long, as src/engine/texture.cpp defines it (the return is
 *    ignored, same code; declared void it decorates to a name nothing defines).
 */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "../sdk/crt.h"
#include "../sdk/win32.h"
#include "../sdk/ddraw.h"

#define SDW_MEMBERS_Texture                  \
    Texture(D3DApp *, u32, u32, u32, s32 *); \
    long Surface_LockForWrite(DDSURFACEDESC2 *);
#include "sdw_classes.h"
#include "../engine/maths.h"
#include "../engine/jpeg_mlt.h"
#include "../engine/draw2d.h"
#include "../engine/text.h"
#include "../engine/screen.h"
#define SDW_INLINE_PACKJPEG_GETCOUNT 1
#include "pack_jpeg_inlines.h"
#undef SDW_INLINE_PACKJPEG_GETCOUNT
extern u32 *g_screenLayerBase;
void Draw2D_TexRect_Immediate(float, float, float, float, float, u32, Texture *, float, float, u32, float, float, u32,
                              float, float, u32, float, float, u32);
#define packedJpegErrorTitle "SDW Error" /* a literal in the original (see the header) */

PackJpeg g_packJpeg = {0};            /* the open pack (Bonus gallery) */
Texture *g_packJpegTextures[4] = {0}; /* the four 256x256 quarter textures */
u16 *g_packJpegPixels[4] = {0};       /* their locked texels while decoding */
s32 g_packJpegOpen = 0;               /* 1 while a pack is open */

s32 PackJpeg::Prev()
{
    if (index > 0)
        --index;
    SetIndex(index);
    return index == 0;
}
s32 PackJpeg::Next()
{
    if (index < GetCount() - 1)
        ++index;
    SetIndex(index);
    return index == GetCount() - 1;
}
void PackJpeg::Close()
{
    g_packJpegOpen = 0;
    if (entries) {
        free(entries);
        entries = 0;
    }
    fclose(file);
}
s32 PackJpeg::SelectByName(const char *name)
{
    u32 i = 0;
    while (i < GetCount() && !Str_IsPrefixOf(entries[i].name, name))
        ++i;
    if (i == GetCount())
        return 0;
    SetIndex(i);
    return 1;
}
s32 PackJpeg::SelectAndFetch(const char *name, void *out)
{
    if (SelectByName(name)) {
        memcpy(out, &opaqueValue, 4);
        return 1;
    }
    return 0;
}
s32 PackJpeg::Open(const char *pathNoExt)
{
    char path[SDW_PATH_MAX];
    Str_Concat2(path, pathNoExt, ".SDW");
    file = fopen(path, "rb");
    if (file) {
        fread(this, 1, 12, file);
        entries = (PackJpegEntry *)malloc(count * sizeof(PackJpegEntry));
        fread(entries, 1, count * sizeof(PackJpegEntry), file);
        SetIndex(0);
        g_packJpegOpen = 1;
        return 1;
    }
    return 0;
}
s32 PackJpeg::SetIndex(u32 value)
{
    index = value;
    return 0;
}

void PackJpeg::DecodeToTextures(s32 unused)
{
    struct Work {
        u8 pad[3], slot;
        s32 row, sourceOffset, targetOffset;
        DDSURFACEDESC2 surface;
        s32 unusedSurfacePad, rowBytes;
        u16 *decoded;
    } w;
    w.decoded = new u16[image->byteSize / 2];
    fseek(file, entries[index].fileOffset, SEEK_SET);
    Jpeg_DecodeToRgb555Flipped(file, w.decoded);
    for (w.slot = 0; w.slot < 4; ++w.slot) {
        g_packJpegTextures[w.slot]->Surface_LockForWrite(&w.surface);
        g_packJpegPixels[w.slot] = (u16 *)w.surface.lpSurface;
    }
    w.rowBytes = (image->width / 2) * 2;
    for (w.row = image->height / 2; w.row >= 0; --w.row) {
        w.sourceOffset = w.row * image->width;
        w.targetOffset = (image->height / 2 - w.row) * 256;
        memcpy(g_packJpegPixels[2] + w.targetOffset, w.decoded + w.sourceOffset, w.rowBytes);
        memcpy(g_packJpegPixels[3] + w.targetOffset, w.decoded + (w.sourceOffset + image->width / 2), w.rowBytes);
    }
    for (w.row = image->height / 2; w.row < image->height; ++w.row) {
        w.sourceOffset = w.row * image->width;
        w.targetOffset = (image->height - (w.row + 1)) * 256;
        memcpy(g_packJpegPixels[0] + w.targetOffset, w.decoded + w.sourceOffset, w.rowBytes);
        memcpy(g_packJpegPixels[1] + w.targetOffset, w.decoded + (w.sourceOffset + image->width / 2), w.rowBytes);
    }
    for (w.slot = 0; w.slot < 4; ++w.slot)
        g_packJpegTextures[w.slot]->Surface_Unlock();
}

void PackJpeg::BeginImage()
{
    struct Work {
        s32 result;
        u8 pad[3], slot;
        u32 width, height;
    } w;
    image = (PackJpegImage *)malloc(sizeof(PackJpegImage));
    fseek(file, entries[index].fileOffset, SEEK_SET);
    if (Jpeg_GetDimensions(file, &w.width, &w.height) == 1) {
        image->width = (s16)w.width;
        image->height = (s16)w.height;
        image->byteSize = w.width * w.height * 2;
        for (w.slot = 0; w.slot < 4; ++w.slot)
            if (!g_packJpegTextures[w.slot])
                g_packJpegTextures[w.slot] = new Texture(g_pD3DAppMain, 256, 256, TEXFMT_ARGB1555, &w.result);
    } else
        MessageBoxA(0, "SDW Packed JPEG file invalid : a non-JPEG file has been detected", packedJpegErrorTitle, MB_OK);
}

void PackJpeg::ReleaseImage()
{
    u8 i;
    if (image)
        free(image);
    for (i = 0; i < 4; ++i) {
        if (g_packJpegTextures[i]) {
            delete g_packJpegTextures[i];
            g_packJpegTextures[i] = 0;
        }
    }
}

void PackJpeg::DrawImage()
{
    HudElement hud(HudElement::Stretch);
    struct Work {
        float scale, u;
        u32 color;
        float bottom, v, right, midY, midX, top, left;
    } w;
    w.color = 0x808080;
    w.top = 0;
    w.bottom = g_pViewFrustum->viewportHeight - g_screen.ScaleY(g_pCurFont->glyphHeight) * 2;
    w.left = (1.0f - (float)image->width / (float)image->height) * g_pViewFrustum->viewportWidth * 0.5f;
    w.right = (1.0f + (float)image->width / (float)image->height) * g_pViewFrustum->viewportWidth * 0.5f;
    if (w.left < 0.0f) {
        w.scale = g_pViewFrustum->viewportWidth / (w.right - w.left);
        w.bottom *= w.scale;
        w.left = 0;
        w.right = g_pViewFrustum->viewportWidth - 1.0f;
    }
    w.midY = (w.bottom - w.top) / 2.0f;
    w.midX = (w.right - w.left) / 2.0f;
    w.u = (float)(image->width / 2) / 256.0f;
    w.v = (float)(image->height / 2) / 256.0f;
#define QUAD(tile, x0, y0, x1, y1)                                                                                \
    Draw2D_TexRect_Immediate(g_screen.Draw2D_LayerToZ(g_screenLayerBase + 1), x0, y0, x1, y1, RSF_TEXTURED,       \
                             g_packJpegTextures[tile], 0, 0, w.color, 0, w.v, w.color, w.u, 0, w.color, w.u, w.v, \
                             w.color)
    QUAD(0, w.left, w.top, w.midX, w.midY);
    QUAD(1, w.midX, w.top, w.right, w.midY);
    QUAD(2, w.left, w.midY, w.midX, w.bottom);
    QUAD(3, w.midX, w.midY, w.right, w.bottom);
#undef QUAD
}
