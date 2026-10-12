/*
 * The "V2.6" Black Sheep model / animation file: BsFile 0x24 { vtbl; u8 ok; u32 frameCount; u8 frameType;
 * u8 *data; s32 size; s32 cursor; u32 frameIndex; Vdx7 *vdx; }. A BsFile is opened from "<name>.xxx" and opens
 * "<name>.DAV" as its Vdx7 (the last three characters replaced). The frame directory starts at file offset 0x10: one
 * u32 per frame, type in the top byte (bit 0x40 masked off by the walkers, but NOT by the constructor's read of entry
 * 0), record offset in the low 24 bits.
 *
 * Mesh_BuildFromBsFile calls one decoder per group of entry kinds. Each walks the current geometry record's entry table
 * ({u16 kind; u16 count; payload[count * stride]}, the table offset at record +4 and the entry count at record +0xa),
 * skips entries of other kinds with BsFile_SkipEntry, and turns every triangle / quad of its kinds into polygons at
 * dest[firstIndex + n], adding `bias` to the byte vertex indices. A quad kind is split into the two triangles (a, d, c)
 * and (a, b, d). The textured kinds look their texture id up in the companion .DAV container (a Vdx7): Bs_TexelToUV
 * maps each byte texel into that rectangle of the 256-texel page.
 *
 * The conversions (Bs_S16ToFloat ..
 * */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "../sdk/win32.h"

#define SDW_MEMBERS_MeshAnimFrame MeshAnimFrame();
#define SDW_MEMBERS_MeshPartPose MeshPartPose();
#define SDW_MEMBERS_Vdx7 Vdx7(const char *path);     /* Vdx7_Open */
#define SDW_MEMBERS_BsFile BsFile(const char *path); /* BsFile_Open */
#include "sdw_classes.h"

#include "../sdk/crt.h"

#include "bs_io.h"
#include "fixed_math.h"

/* ------------------------------------------------------------------------------------------------ BsFile */

/* loads the file, checks the "V2.6" magic at offset 4, reads the frame count at 0xc, positions on frame 0 and
 * opens the .DAV companion; ok is the companion's. */
BsFile::BsFile(const char *path)
{
    char *davPath;
    u32 nlen;
    char sig[4];
    s32 got;
    u32 entry;

    data = 0;
    vdx = 0;
    cursor = 0;
    data = (u8 *)Bs_LoadFile(path, (u32 *)&got);
    if (got <= 0) {
        ok = 0;
        return;
    }
    size = got;
    ReadU32(1);
    sig[0] = ReadU8(1);
    sig[1] = ReadU8(1);
    sig[2] = ReadU8(1);
    sig[3] = ReadU8(1);
    if (strncmp(sig, "V2.6", 4)) {
        ok = 0;
        return;
    }
    cursor += 4;
    frameCount = ReadU32(1);
    cursor = 0x10;
    frameIndex = 0;
    entry = ReadU32(1);
    frameType = entry >> 24;
    cursor = entry & 0xffffff;
    nlen = strlen(path);
    davPath = (char *)malloc(nlen + 1);
    strcpy(davPath, path);
    davPath[nlen - 3] = 'D';
    davPath[nlen - 2] = 'A';
    davPath[nlen - 1] = 'V';
    vdx = new Vdx7(davPath);
    ok = vdx->ok;
    free(davPath);
}

BsFile::~BsFile()
{
    if (data)
        delete data;
    if (vdx)
        delete vdx;
}

/* the current frame's type is one of the geometry types (3, 4, 0xb, 0x26, 0x27). */
bool BsFile::Bs_IsGeometryResource()
{
    return frameType == WAR_RES_MODEL || frameType == WAR_RES_MESH || frameType == WAR_RES_MESH_B ||
           frameType == WAR_RES_SKY || frameType == WAR_RES_TYPE_27;
}

/* steps to the next frame-directory entry; 1, or 2 after wrapping to frame 0; 0 when the file is not open. */
int BsFile::NextFrame()
{
    int r;
    u32 entry;

    r = BSFRAME_NOT_OPEN;
    if (ok == 1) {
        cursor = 0x10;
        frameIndex += 1;
        if (frameIndex >= frameCount) {
            frameIndex = 0;
            r = BSFRAME_FIRST;
        } else {
            r = BSFRAME_NEXT;
        }
        cursor += frameIndex * 4;
        entry = ReadU32(0);
        frameType = (entry >> 24) & 0xbf;
        cursor = entry & 0xffffff;
    }
    return r;
}

/* steps forward until a frame of the given type, stopping at the wrap. */
int BsFile::SeekFrameOfType(u8 type)
{
    int r;

    do {
        r = NextFrame();
    } while (r != BSFRAME_NOT_OPEN && r != BSFRAME_FIRST && frameType != type);
    return r;
}

/* rewinds to frame 0; returns 2 (0 when the file is not open). */
int BsFile::FirstFrame()
{
    int r;
    u32 entry;

    r = BSFRAME_NOT_OPEN;
    if (ok == 1) {
        cursor = 0x10;
        frameIndex = 0;
        entry = ReadU32(0);
        frameType = (entry >> 24) & 0xbf;
        cursor = entry & 0xffffff;
        r = BSFRAME_FIRST;
    }
    return r;
}

/* rewinds, then steps until a frame of the given type (frame 0 included). */
int BsFile::FindFrameOfType(u8 type)
{
    int r;

    r = BSFRAME_NEXT;
    FirstFrame();
    while (r != BSFRAME_NOT_OPEN && r != BSFRAME_FIRST && frameType != type)
        r = NextFrame();
    return r;
}

/* the u16 at geometry record +8 (the vertex count), read without moving the cursor. */
u32 BsFile::PeekEntryCount()
{
    s32 save;
    u32 n;

    n = 0;
    save = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 8;
        n = ReadU16(1);
    }
    cursor = save;
    return n;
}

/* PeekEntryCount summed over every frame; the current frame is restored. */
int BsFile::TotalVerticesAllFrames()
{
    int total;
    u32 i;
    u32 target;
    u32 idx;

    total = 0;
    target = frameIndex;
    FirstFrame();
    for (i = 0; i < frameCount; i++) {
        total += PeekEntryCount();
        NextFrame();
    }
    FirstFrame();
    for (idx = 0; idx < target; idx++)
        NextFrame();
    return total;
}

/* walks the entry table of a geometry record (offset at +4, count at +0xa) for an entry whose kind is `id`
 * and returns the u16 that follows it (0 if none). */
u32 BsFile::FindEntryValue(u16 id)
{
    u32 off;
    u16 nEntries;
    s32 mark;
    u16 e;
    u32 value;

    value = 0;
    e = 0;
    mark = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        off = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = off;
        do {
            if (ReadU16(0) == id) {
                cursor += 2;
                value = ReadU16(1);
            } else {
                SkipEntry();
            }
            e++;
        } while (e < nEntries && value == 0);
    }
    cursor = mark;
    return value;
}

/* FindEntryValue(kind) summed over every frame; the current frame is restored. No callers. */
int BsFile::CountKindAcrossFrames(u16 kind)
{
    int total;
    u32 i;
    u32 target;
    u32 idx;

    total = 0;
    target = frameIndex;
    FirstFrame();
    for (i = 0; i < frameCount; i++) {
        total += FindEntryValue(kind);
        NextFrame();
    }
    FirstFrame();
    for (idx = 0; idx < target; idx++)
        NextFrame();
    return total;
}

/* the u16 at type-4 record +0x18 (the part count); 0 for other types. */
u32 BsFile::Type4_GetField18()
{
    s32 save;
    u32 n;

    n = 0;
    save = cursor;
    if (frameType == WAR_RES_MODEL) {
        cursor += 0x18;
        n = ReadU16(1);
    }
    cursor = save;
    return n;
}

/* Type4_GetField18 summed over every frame. No callers. */
int BsFile::TotalField18AllFrames()
{
    int total;
    u32 f;
    u32 target;
    u32 idx;

    total = 0;
    target = frameIndex;
    FirstFrame();
    for (f = 0; f < frameCount; f++) {
        total += Type4_GetField18();
        NextFrame();
    }
    FirstFrame();
    for (idx = 0; idx < target; idx++)
        NextFrame();
    return total;
}

/* the first u16 of the table a type-4 record points at from +0x10 (the sequence count); 0 for other types. */
u32 BsFile::Type4_GetTableCount()
{
    s32 save;
    u32 n;

    n = 0;
    save = cursor;
    if (frameType == WAR_RES_MODEL) {
        cursor += 0x10;
        cursor = ReadU32(0);
        n = ReadU16(1);
    }
    cursor = save;
    return n;
}

/* Type4_GetTableCount summed over every frame. */
int BsFile::TotalTableCountAllFrames()
{
    int total;
    u32 i;
    u32 target;
    u32 idx;

    total = 0;
    target = frameIndex;
    FirstFrame();
    for (i = 0; i < frameCount; i++) {
        total += Type4_GetTableCount();
        NextFrame();
    }
    FirstFrame();
    for (idx = 0; idx < target; idx++)
        NextFrame();
    return total;
}

/* the record's vertex table (offset at +0, count at +8): three s16 + one pad u16 each, stored as three floats
 * per vertex from dest[firstIndex] on. Returns the count. */
u32 BsFile::ReadVertices(float *dest, u16 firstIndex)
{
    u16 n;
    s32 saved;
    u32 table;
    int v;

    saved = cursor;
    table = ReadU32(1);
    cursor += 4;
    n = ReadU16(1);
    cursor = table;
    for (v = 0; v < n; v++) {
        ((float (*)[3])dest)[v + firstIndex][0] = Bs_S16ToFloat(ReadU16(1));
        ((float (*)[3])dest)[v + firstIndex][1] = Bs_S16ToFloat(ReadU16(1));
        ((float (*)[3])dest)[v + firstIndex][2] = Bs_S16ToFloat(ReadU16(1));
        ReadU16(1);
    }
    cursor = saved;
    return n;
}

/* kinds 0 (flat triangle) and 1 (flat quad) -> BsPolyFlat. */
int BsFile::BsDecode_Kind0_1(BsPolyFlat *dest, int firstIndex, int bias)
{
    u32 i;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_F3:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[i + firstIndex].idx[0] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[1] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[2] = ReadU8(1) + bias;
                        ReadU8(1);
                        dest[i + firstIndex].colour = ReadRgbExpanded(1);
                    }
                    decoded += count;
                    firstIndex += count;
                    break;
                case BS_ENTRY_F4:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].colour = ReadRgbExpanded(0);
                        dest[firstIndex + i * 2 + 1].colour = ReadRgbExpanded(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 2 (Gouraud triangle) and 3 (Gouraud quad) -> BsPolyGouraud. */
int BsFile::BsDecode_Kind2_3(BsPolyGouraud *dest, int firstIndex, int bias)
{
    u32 i;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_G3:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[i + firstIndex].idx[0] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[1] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[2] = ReadU8(1) + bias;
                        ReadU8(1);
                        dest[i + firstIndex].colour[0] = ReadRgbExpanded(1);
                        dest[i + firstIndex].colour[1] = ReadRgbExpanded(1);
                        dest[i + firstIndex].colour[2] = ReadRgbExpanded(1);
                    }
                    decoded += count;
                    firstIndex += count;
                    break;
                case BS_ENTRY_G4:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].colour[0] = ReadRgbExpanded(0);
                        dest[firstIndex + i * 2 + 1].colour[0] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2 + 1].colour[1] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2].colour[2] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2].colour[1] = ReadRgbExpanded(0);
                        dest[firstIndex + i * 2 + 1].colour[2] = ReadRgbExpanded(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 0x10 (flat blended triangle) and 0x11 (quad) -> BsPolyBlendFlat; the blend mode is a u32. */
int BsFile::BsDecode_Kind10_11(BsPolyBlendFlat *dest, int firstIndex, int bias)
{
    u32 i;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_BF3:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[i + firstIndex].idx[0] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[1] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[2] = ReadU8(1) + bias;
                        ReadU8(1);
                        dest[i + firstIndex].colour = ReadRgbExpanded(1);
                        dest[i + firstIndex].blendMode = ReadU32(1);
                    }
                    decoded += count;
                    firstIndex += count;
                    break;
                case BS_ENTRY_BF4:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].colour = ReadRgbExpanded(0);
                        dest[firstIndex + i * 2 + 1].colour = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2].blendMode = ReadU32(0);
                        dest[firstIndex + i * 2 + 1].blendMode = ReadU32(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 0x12 / 0x16 (Gouraud blended triangle, one payload shape) and 0x13 (quad) -> BsPolyBlendGouraud. */
int BsFile::BsDecode_Kind12(BsPolyBlendGouraud *dest, int firstIndex, int bias)
{
    u32 i;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_BG3:
                case BS_ENTRY_BG3_ALT:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[i + firstIndex].idx[0] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[1] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[2] = ReadU8(1) + bias;
                        ReadU8(1);
                        dest[i + firstIndex].colour[0] = ReadRgbExpanded(1);
                        dest[i + firstIndex].colour[1] = ReadRgbExpanded(1);
                        dest[i + firstIndex].colour[2] = ReadRgbExpanded(1);
                        dest[i + firstIndex].blendMode = ReadU32(1);
                    }
                    decoded += count;
                    firstIndex += count;
                    break;
                case BS_ENTRY_BG4:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].colour[0] = ReadRgbExpanded(0);
                        dest[firstIndex + i * 2 + 1].colour[0] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2 + 1].colour[1] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2].colour[2] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2].colour[1] = ReadRgbExpanded(0);
                        dest[firstIndex + i * 2 + 1].colour[2] = ReadRgbExpanded(1);
                        dest[firstIndex + i * 2].blendMode = ReadU32(0);
                        dest[firstIndex + i * 2 + 1].blendMode = ReadU32(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 4 / 0x14 (textured flat triangle) and 5 (quad) -> BsPolyTexFlat. Each polygon carries one byte (u, v) texel pair
 * per corner, a u32 texture id that the Vdx7 maps to a texture rectangle, and a 24-bit colour; Bs_TexelToUV turns each
 * texel into a page UV inside that rectangle. */
int BsFile::BsDecode_Stride24_Pass1(BsPolyTexFlat *dest, int firstIndex, int bias)
{
    u32 i;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_FT3:
                case BS_ENTRY_BFT3:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        u8 ku1;
                        u8 ku0;
                        u32 rectIndex;
                        u32 id;
                        u8 tv2;
                        u8 tv1;
                        u8 tv0;
                        u8 tu2;
                        Vdx7Record *rec;

                        dest[i + firstIndex].idx[0] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[1] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[2] = ReadU8(1) + bias;
                        ReadU8(1);
                        ku0 = ReadU8(1);
                        tv0 = ReadU8(1);
                        ku1 = ReadU8(1);
                        tv1 = ReadU8(1);
                        tu2 = ReadU8(1);
                        tv2 = ReadU8(1);
                        ReadU16(1);
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        rec = vdx->records + rectIndex;
                        dest[i + firstIndex].colour = ReadU24BE(1);
                        ReadU32(1);
                        dest[i + firstIndex].uv[0] = Bs_TexelToUV(ku0, rec->w, rec->x);
                        dest[i + firstIndex].uv[1] = Bs_TexelToUV(tv0, rec->h, rec->y);
                        dest[i + firstIndex].uv[2] = Bs_TexelToUV(ku1, rec->w, rec->x);
                        dest[i + firstIndex].uv[3] = Bs_TexelToUV(tv1, rec->h, rec->y);
                        dest[i + firstIndex].uv[4] = Bs_TexelToUV(tu2, rec->w, rec->x);
                        dest[i + firstIndex].uv[5] = Bs_TexelToUV(tv2, rec->h, rec->y);
                        dest[i + firstIndex].texIndex = rec->page;
                    }
                    decoded += count;
                    firstIndex += count;
                    break;
                case BS_ENTRY_FT4:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        u8 ku1;
                        u8 ku0;
                        u32 rectIndex;
                        u32 id;
                        u8 tv3;
                        u8 tv2;
                        u8 tv1;
                        u8 tv0;
                        u8 tu3;
                        u8 tu2;
                        Vdx7Record *rec;

                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        ku0 = ReadU8(1);
                        tv0 = ReadU8(1);
                        ku1 = ReadU8(1);
                        tv1 = ReadU8(1);
                        tu2 = ReadU8(1);
                        tv2 = ReadU8(1);
                        tu3 = ReadU8(1);
                        tv3 = ReadU8(1);
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        rec = vdx->records + rectIndex;
                        dest[firstIndex + i * 2].colour = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour = ReadU24BE(1);
                        ReadU32(1);
                        dest[firstIndex + i * 2].uv[0] = Bs_TexelToUV(ku0, rec->w, rec->x);
                        dest[firstIndex + i * 2].uv[1] = Bs_TexelToUV(tv0, rec->h, rec->y);
                        dest[firstIndex + i * 2].uv[2] = Bs_TexelToUV(tu3, rec->w, rec->x);
                        dest[firstIndex + i * 2].uv[3] = Bs_TexelToUV(tv3, rec->h, rec->y);
                        dest[firstIndex + i * 2].uv[4] = Bs_TexelToUV(tu2, rec->w, rec->x);
                        dest[firstIndex + i * 2].uv[5] = Bs_TexelToUV(tv2, rec->h, rec->y);
                        dest[firstIndex + i * 2 + 1].uv[0] = Bs_TexelToUV(ku0, rec->w, rec->x);
                        dest[firstIndex + i * 2 + 1].uv[1] = Bs_TexelToUV(tv0, rec->h, rec->y);
                        dest[firstIndex + i * 2 + 1].uv[2] = Bs_TexelToUV(ku1, rec->w, rec->x);
                        dest[firstIndex + i * 2 + 1].uv[3] = Bs_TexelToUV(tv1, rec->h, rec->y);
                        dest[firstIndex + i * 2 + 1].uv[4] = Bs_TexelToUV(tu3, rec->w, rec->x);
                        dest[firstIndex + i * 2 + 1].uv[5] = Bs_TexelToUV(tv3, rec->h, rec->y);
                        dest[firstIndex + i * 2].texIndex = rec->page;
                        dest[firstIndex + i * 2 + 1].texIndex = rec->page;
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 6 / 0x15 (textured Gouraud triangle) and 7 (quad) -> BsPolyTexGouraud: as Stride24_Pass1 with a
 * 24-bit colour per corner, and one more skipped u32 per corner-colour block (three per triangle, four per quad). */
int BsFile::BsDecode_Stride48_Pass1(BsPolyTexGouraud *dest, int firstIndex, int bias)
{
    u32 i;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_GT3:
                case BS_ENTRY_BGT3:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        u8 ku1;
                        u8 ku0;
                        u32 rectIndex;
                        u32 id;
                        u8 tv2;
                        u8 tv1;
                        u8 tv0;
                        u8 tu2;
                        Vdx7Record *rec;

                        dest[i + firstIndex].idx[0] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[1] = ReadU8(1) + bias;
                        dest[i + firstIndex].idx[2] = ReadU8(1) + bias;
                        ReadU8(1);
                        ku0 = ReadU8(1);
                        tv0 = ReadU8(1);
                        ku1 = ReadU8(1);
                        tv1 = ReadU8(1);
                        tu2 = ReadU8(1);
                        tv2 = ReadU8(1);
                        ReadU16(1);
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        rec = vdx->records + rectIndex;
                        dest[i + firstIndex].colour[0] = ReadU24BE(1);
                        dest[i + firstIndex].colour[1] = ReadU24BE(1);
                        dest[i + firstIndex].colour[2] = ReadU24BE(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                        dest[i + firstIndex].uv[0] = Bs_TexelToUV(ku0, rec->w, rec->x);
                        dest[i + firstIndex].uv[1] = Bs_TexelToUV(tv0, rec->h, rec->y);
                        dest[i + firstIndex].uv[2] = Bs_TexelToUV(ku1, rec->w, rec->x);
                        dest[i + firstIndex].uv[3] = Bs_TexelToUV(tv1, rec->h, rec->y);
                        dest[i + firstIndex].uv[4] = Bs_TexelToUV(tu2, rec->w, rec->x);
                        dest[i + firstIndex].uv[5] = Bs_TexelToUV(tv2, rec->h, rec->y);
                        dest[i + firstIndex].texIndex = rec->page;
                    }
                    decoded += count;
                    firstIndex += count;
                    break;
                case BS_ENTRY_GT4:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        u8 ku1;
                        u8 ku0;
                        u32 rectIndex;
                        u32 id;
                        u8 tv3;
                        u8 tv2;
                        u8 tv1;
                        u8 tv0;
                        u8 tu3;
                        u8 tu2;
                        Vdx7Record *rec;

                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        ku0 = ReadU8(1);
                        tv0 = ReadU8(1);
                        ku1 = ReadU8(1);
                        tv1 = ReadU8(1);
                        tu2 = ReadU8(1);
                        tv2 = ReadU8(1);
                        tu3 = ReadU8(1);
                        tv3 = ReadU8(1);
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        rec = vdx->records + rectIndex;
                        dest[firstIndex + i * 2].colour[0] = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour[0] = ReadU24BE(1);
                        dest[firstIndex + i * 2 + 1].colour[1] = ReadU24BE(1);
                        dest[firstIndex + i * 2].colour[2] = ReadU24BE(1);
                        dest[firstIndex + i * 2].colour[1] = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour[2] = ReadU24BE(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                        dest[firstIndex + i * 2].uv[0] = Bs_TexelToUV(ku0, rec->w, rec->x);
                        dest[firstIndex + i * 2].uv[1] = Bs_TexelToUV(tv0, rec->h, rec->y);
                        dest[firstIndex + i * 2].uv[2] = Bs_TexelToUV(tu3, rec->w, rec->x);
                        dest[firstIndex + i * 2].uv[3] = Bs_TexelToUV(tv3, rec->h, rec->y);
                        dest[firstIndex + i * 2].uv[4] = Bs_TexelToUV(tu2, rec->w, rec->x);
                        dest[firstIndex + i * 2].uv[5] = Bs_TexelToUV(tv2, rec->h, rec->y);
                        dest[firstIndex + i * 2 + 1].uv[0] = Bs_TexelToUV(ku0, rec->w, rec->x);
                        dest[firstIndex + i * 2 + 1].uv[1] = Bs_TexelToUV(tv0, rec->h, rec->y);
                        dest[firstIndex + i * 2 + 1].uv[2] = Bs_TexelToUV(ku1, rec->w, rec->x);
                        dest[firstIndex + i * 2 + 1].uv[3] = Bs_TexelToUV(tv1, rec->h, rec->y);
                        dest[firstIndex + i * 2 + 1].uv[4] = Bs_TexelToUV(tu3, rec->w, rec->x);
                        dest[firstIndex + i * 2 + 1].uv[5] = Bs_TexelToUV(tv3, rec->h, rec->y);
                        dest[firstIndex + i * 2].texIndex = rec->page;
                        dest[firstIndex + i * 2 + 1].texIndex = rec->page;
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 0xc-0xf, the fixed-square textured flat quads -> BsPolyTexFlat, appended after Pass1's polygons
 * (Mesh_BuildFromBsFile passes Pass1's count as firstIndex). They carry no texels: each quad covers one square
 * of `cell` texels at the origin of its texture rectangle, 0x40 for kinds 0xc / 0xd and 0x20 for 0xe / 0xf. The
 * two quad kinds of each size differ in how the square is split into triangles (0xc / 0xe along one diagonal,
 * 0xd / 0xf along the other) and in where the two skipped u32s sit. `cell` is not reset per entry; only these
 * four kinds read it. */
int BsFile::BsDecode_Stride24_Pass2(BsPolyTexFlat *dest, int firstIndex, int bias)
{
    u32 rectIndex;
    u32 id;
    int cell;
    u32 i;
    Vdx7Record *pRec;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_FT4_SQ64_A:
                case BS_ENTRY_FT4_SQ64_B:
                    cell = 0x40;
                    break;
                case BS_ENTRY_FT4_SQ32_A:
                case BS_ENTRY_FT4_SQ32_B:
                    cell = 0x20;
                    break;
            }
            switch (ReadU16(0)) {
                case BS_ENTRY_FT4_SQ64_A:
                case BS_ENTRY_FT4_SQ32_A:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        pRec = vdx->records + rectIndex;
                        dest[firstIndex + i * 2].texIndex = pRec->page;
                        dest[firstIndex + i * 2 + 1].texIndex = pRec->page;
                        ReadU32(1);
                        ReadU32(1);
                        dest[firstIndex + i * 2].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[2] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[3] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[4] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[5] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[2] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[3] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[4] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[5] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].colour = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour = ReadU24BE(1);
                        ReadU32(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                case BS_ENTRY_FT4_SQ64_B:
                case BS_ENTRY_FT4_SQ32_B:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        pRec = vdx->records + rectIndex;
                        dest[firstIndex + i * 2].texIndex = pRec->page;
                        dest[firstIndex + i * 2 + 1].texIndex = pRec->page;
                        dest[firstIndex + i * 2].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[2] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[3] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[4] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[5] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[2] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[3] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[4] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[5] = Bs_TexelToUV(cell, cell, pRec->y);
                        ReadU32(1);
                        ReadU32(1);
                        dest[firstIndex + i * 2].colour = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour = ReadU24BE(1);
                        ReadU32(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* kinds 8-0xb, the fixed-square textured Gouraud quads -> BsPolyTexGouraud, appended after Pass1's
 * polygons: Stride24_Pass2 with a colour per corner. 0x40-texel squares for kinds 8 / 9, 0x20 for 0xa / 0xb;
 * kinds 8 / 0xa and 9 / 0xb differ in the diagonal. */
int BsFile::BsDecode_Stride48_Pass2(BsPolyTexGouraud *dest, int firstIndex, int bias)
{
    u32 rectIndex;
    u32 id;
    int cell;
    u32 i;
    Vdx7Record *pRec;
    s32 offset;
    u16 nEntries;
    s32 cursor0;
    u16 e;
    int decoded;
    u32 count;

    decoded = 0;
    count = 0;
    e = 0;
    cursor0 = cursor;
    if (Bs_IsGeometryResource() == 1) {
        cursor += 4;
        offset = ReadU32(1);
        cursor += 2;
        nEntries = ReadU16(1);
        cursor = offset;
        do {
            switch (ReadU16(0)) {
                case BS_ENTRY_GT4_SQ64_A:
                case BS_ENTRY_GT4_SQ64_B:
                    cell = 0x40;
                    break;
                case BS_ENTRY_GT4_SQ32_A:
                case BS_ENTRY_GT4_SQ32_B:
                    cell = 0x20;
                    break;
            }
            switch (ReadU16(0)) {
                case BS_ENTRY_GT4_SQ64_A:
                case BS_ENTRY_GT4_SQ32_A:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        pRec = vdx->records + rectIndex;
                        dest[firstIndex + i * 2].texIndex = pRec->page;
                        dest[firstIndex + i * 2 + 1].texIndex = pRec->page;
                        ReadU32(1);
                        ReadU32(1);
                        dest[firstIndex + i * 2].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[2] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[3] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[4] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[5] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[2] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[3] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[4] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[5] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].colour[0] = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour[0] = ReadU24BE(1);
                        dest[firstIndex + i * 2 + 1].colour[1] = ReadU24BE(1);
                        dest[firstIndex + i * 2].colour[2] = ReadU24BE(1);
                        dest[firstIndex + i * 2].colour[1] = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour[2] = ReadU24BE(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                case BS_ENTRY_GT4_SQ64_B:
                case BS_ENTRY_GT4_SQ32_B:
                    cursor += 2;
                    count = ReadU16(1);
                    for (i = 0; i < count; i++) {
                        dest[firstIndex + i * 2].idx[0] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[0] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2 + 1].idx[1] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[2] = ReadU8(1) + bias;
                        dest[firstIndex + i * 2].idx[1] = ReadU8(0) + bias;
                        dest[firstIndex + i * 2 + 1].idx[2] = ReadU8(1) + bias;
                        id = ReadU32(1);
                        rectIndex = vdx->entries[id];
                        pRec = vdx->records + rectIndex;
                        dest[firstIndex + i * 2].texIndex = pRec->page;
                        dest[firstIndex + i * 2 + 1].texIndex = pRec->page;
                        ReadU32(1);
                        ReadU32(1);
                        dest[firstIndex + i * 2].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[2] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[3] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].uv[4] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2].uv[5] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[0] = Bs_TexelToUV(0, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[1] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[2] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[3] = Bs_TexelToUV(0, cell, pRec->y);
                        dest[firstIndex + i * 2 + 1].uv[4] = Bs_TexelToUV(cell, cell, pRec->x);
                        dest[firstIndex + i * 2 + 1].uv[5] = Bs_TexelToUV(cell, cell, pRec->y);
                        dest[firstIndex + i * 2].colour[0] = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour[0] = ReadU24BE(1);
                        dest[firstIndex + i * 2 + 1].colour[1] = ReadU24BE(1);
                        dest[firstIndex + i * 2].colour[2] = ReadU24BE(1);
                        dest[firstIndex + i * 2].colour[1] = ReadU24BE(0);
                        dest[firstIndex + i * 2 + 1].colour[2] = ReadU24BE(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                        ReadU32(1);
                    }
                    decoded += count * 2;
                    firstIndex += count * 2;
                    break;
                default:
                    SkipEntry();
                    break;
            }
            e++;
        } while (e < nEntries);
    }
    cursor = cursor0;
    return decoded;
}

/* the part table of a type-4 record (offset at +0x14, u16 count after it): per part five u16s - parent index,
 * three s16 offsets widened to floats, vertex count. Always returns 0. */
u32 BsFile::Type4_ReadTable14(MeshPart *parts)
{
    u32 addr;
    u32 i;
    u32 count;
    u32 r;
    s32 back;

    r = 0;
    back = cursor;
    if (frameType == WAR_RES_MODEL) {
        cursor += 0x14;
        addr = ReadU32(1);
        count = ReadU16(1);
        cursor = addr;
        for (i = 0; i < count; i++) {
            parts[i].parentIndex = ReadU16(1);
            parts[i].offset[0] = Bs_S16ToFloat(ReadU16(1));
            parts[i].offset[1] = Bs_S16ToFloat(ReadU16(1));
            parts[i].offset[2] = Bs_S16ToFloat(ReadU16(1));
            parts[i].vertexCount = ReadU16(1);
        }
    }
    cursor = back;
    return r;
}

/* the animation sequences of a type-4 record (table offset at +0x10: u32 count, then one u32 offset per
 * sequence; offset 0 = empty slot "No anim"). A sequence is an 8-char name, a u16 frame count and the frame track; each
 * frame gets its own MeshPartPose[nParts]. Returns the number of sequences read. */
int BsFile::ReadAnimNames(MeshAnimSeq *seqs, u32 nParts)
{
    int result;
    u32 nAnim;
    s32 back;
    u32 slot;
    u32 seq;
    u16 keys;
    char label[8];
    u32 ci;
    u32 k;

    result = 0;
    back = cursor;
    if (frameType == WAR_RES_MODEL) {
        cursor += 0x10;
        cursor = ReadU32(1);
        nAnim = ReadU32(1);
        slot = cursor;
        for (seq = 0; seq < nAnim; seq++) {
            cursor = slot;
            cursor = ReadU32(0);
            if (cursor != 0) {
                for (ci = 0; ci < 8; ci++)
                    label[ci] = ReadU8(1);
                strncpy(seqs[seq].name, label, 8);
                keys = ReadU16(1);
                seqs[seq].frameCount = keys;
                seqs[seq].frames = new MeshAnimFrame[keys];
                for (k = 0; k < keys; k++)
                    seqs[seq].frames[k].poses = new MeshPartPose[nParts];
                ReadTrack12(seqs[seq].frames, keys, nParts);
                result++;
            } else {
                strncpy(seqs[seq].name, "No anim", 8);
            }
            slot += 4;
        }
    }
    cursor = back;
    return result;
}

u8 BsFile::ReadU8(u8 advance)
{
    u8 v;

    v = data[cursor];
    if (advance == 1)
        cursor += 1;
    return v;
}

/* little-endian. */
u16 BsFile::ReadU16(u8 advance)
{
    u16 v;

    v = data[cursor];
    v += (u16)(data[cursor + 1] << 8);
    if (advance == 1)
        cursor += 2;
    return v;
}

/* little-endian. */
u32 BsFile::ReadU32(u8 advance)
{
    u32 v;

    v = data[cursor];
    v += data[cursor + 1] << 8;
    v += data[cursor + 2] << 16;
    v += data[cursor + 3] << 24;
    if (advance == 1)
        cursor += 4;
    return v;
}

/* three bytes BIG-endian in a four-byte slot (a PlayStation colour). */
u32 BsFile::ReadU24BE(u8 advance)
{
    u32 v;

    v = data[cursor + 2];
    v += data[cursor + 1] << 8;
    v += data[cursor] << 16;
    if (advance == 1)
        cursor += 4;
    return v;
}

/* the whole colour, expanded by ColorFix's 1.03 factor for untextured faces. */
u32 BsFile::ReadRgbExpanded(u8 advance)
{
    return Color_ExpandPs1(ReadU24BE(advance));
}

/* (no callers) */
void BsFile::WriteU8(u8 value, u8 advance)
{
    data[cursor] = value;
    if (advance == 1)
        cursor += 1;
}

/* (no callers) */
void BsFile::WriteU16(u16 value, u8 advance)
{
    data[cursor + 1] = value >> 8;
    data[cursor] = value & 0xff;
    if (advance == 1)
        cursor += 2;
}

/* (no callers) */
void BsFile::WriteU32(u32 value, u8 advance)
{
    data[cursor + 3] = value >> 24;
    data[cursor + 2] = (value >> 16) & 0xff;
    data[cursor + 1] = (value >> 8) & 0xff;
    data[cursor] = value & 0xff;
    if (advance == 1)
        cursor += 4;
}

float BsFile::Bs_S16ToFloat(s16 v)
{
    return v;
}

/* 4096 units per turn, wrapped into [0, 2pi). */
float BsFile::Bs_Angle12ToRadians(s16 a)
{
    float r;

    r = a * 2.0f * 3.1415927f / 4096.0f;
    if (r < 0.0f)
        r += 6.2831855f;
    return r;
}

/* 128 units per half turn (a signed byte), wrapped into [0, 2pi). */
float BsFile::Bs_Angle8ToRadians(s8 a)
{
    float r;

    r = a * 2.0f * 3.1415927f / 128.0f;
    if (r < 0.0f)
        r += 6.2831855f;
    return r;
}

/* 4.10 fixed point. */
float BsFile::Bs_Fixed12ToFloat(u16 v)
{
    return v / 1024.0f;
}

float BsFile::Bs_Normal8ToFloat(u8 v)
{
    return v / 128.0f;
}

/* a texel of a `size`-pixel image placed at `origin` in a 256-pixel page, as a half-texel-corrected U or V. */
float BsFile::Bs_TexelToUV(u8 texel, s16 size, s16 origin)
{
    return (texel * ((float)(size - 1) / size) + 0.5f + origin) / 256.0f;
}

/* steps over one entry of a geometry record. */
void BsFile::SkipEntry()
{
    u16 type;
    u16 n;

    type = ReadU16(1);
    n = ReadU16(1);
    switch (type) {
        case BS_ENTRY_F3:
            cursor += n * 8;
            break;
        case BS_ENTRY_F4:
            cursor += n * 8;
            break;
        case BS_ENTRY_G3:
            cursor += n * 16;
            break;
        case BS_ENTRY_G4:
            cursor += n * 0x14;
            break;
        case BS_ENTRY_FT3:
        case BS_ENTRY_BFT3:
            cursor += n * 0x18;
            break;
        case BS_ENTRY_FT4:
            cursor += n * 0x18;
            break;
        case BS_ENTRY_GT3:
        case BS_ENTRY_BGT3:
            cursor += n * 0x28;
            break;
        case BS_ENTRY_GT4:
            cursor += n * 0x30;
            break;
        case BS_ENTRY_GT4_SQ64_A:
        case BS_ENTRY_GT4_SQ64_B:
        case BS_ENTRY_GT4_SQ32_A:
        case BS_ENTRY_GT4_SQ32_B:
            cursor += n * 0x30;
            break;
        case BS_ENTRY_FT4_SQ64_A:
        case BS_ENTRY_FT4_SQ64_B:
        case BS_ENTRY_FT4_SQ32_A:
        case BS_ENTRY_FT4_SQ32_B:
            cursor += n * 0x18;
            break;
        case BS_ENTRY_BF3:
            cursor += n * 0xc;
            break;
        case BS_ENTRY_BF4:
            cursor += n * 0xc;
            break;
        case BS_ENTRY_BG3:
        case BS_ENTRY_BG3_ALT:
            cursor += n * 0x14;
            break;
        case BS_ENTRY_BG4:
            cursor += n * 0x18;
            break;
        default:
            MessageBoxA(0, "Error in polygon format", "Black Sheep Error", MB_OK);
    }
}

/* the frame track of a sequence: per frame a u16 duration, the joint-entry count, the record length in u16
 * words and one unused u16, then the joint entries. Each frame starts as a copy of the previous frame's poses. */
void BsFile::ReadTrack12(MeshAnimFrame *frames, u32 count, u32 nParts)
{
    u16 n;
    u16 len;
    s32 saved;
    u32 i;

    saved = cursor;
    for (i = 0; i < count; i++) {
        frames[i].halfDurationMs = ReadU16(1);
        n = ReadU16(1);
        len = ReadU16(1) * 2;
        ReadU16(1);
        ReadJointTable(frames[i].poses, n);
        if (i < count - 1)
            memcpy(frames[i + 1].poses, frames[i].poses, nParts * sizeof(MeshPartPose));
        cursor += len;
    }
    cursor = saved;
}

/* joint entries: a u16 control word (low 6 bits = part index, 0x40/0x80/0x100 rotation, 0x200/0x400/0x800
 * translation, 0x1000/0x2000/0x4000 scale present; 0x8000 = byte-packed values, padded to an even count), then the
 * values. Absent channels are reset (rotation and translation 0, scale 1). */
void BsFile::ReadJointTable(MeshPartPose *poses, u32 count)
{
    u8 bytes;
    u32 index;
    u16 mask;
    s32 saved;
    u32 i;

    saved = cursor;
    for (i = 0; i < count; i++) {
        bytes = 0;
        mask = ReadU16(1);
        index = mask & ANIMKEY_JOINT_MASK;
        poses[index].rot[0] = 0.0f;
        poses[index].rot[1] = 0.0f;
        poses[index].rot[2] = 0.0f;
        poses[index].pos[0] = 0.0f;
        poses[index].pos[1] = 0.0f;
        poses[index].pos[2] = 0.0f;
        poses[index].scale[0] = 1.0f;
        poses[index].scale[1] = 1.0f;
        poses[index].scale[2] = 1.0f;
        if (mask & ANIMKEY_8BIT) {
            if (mask & ANIMKEY_ROT_X) {
                poses[index].rot[0] = Bs_Angle8ToRadians(ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_ROT_Y) {
                poses[index].rot[1] = Bs_Angle8ToRadians(ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_ROT_Z) {
                poses[index].rot[2] = Bs_Angle8ToRadians(ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_POS_X) {
                poses[index].pos[0] = Bs_S16ToFloat((s8)ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_POS_Y) {
                poses[index].pos[1] = Bs_S16ToFloat((s8)ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_POS_Z) {
                poses[index].pos[2] = Bs_S16ToFloat((s8)ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_SCALE_X) {
                poses[index].scale[0] = Bs_Normal8ToFloat(ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_SCALE_Y) {
                poses[index].scale[1] = Bs_Normal8ToFloat(ReadU8(1));
                bytes++;
            }
            if (mask & ANIMKEY_SCALE_Z) {
                poses[index].scale[2] = Bs_Normal8ToFloat(ReadU8(1));
                bytes++;
            }
            if (bytes % 2)
                ReadU8(1);
        } else {
            if (mask & ANIMKEY_ROT_X)
                poses[index].rot[0] = Bs_Angle12ToRadians(ReadU16(1));
            if (mask & ANIMKEY_ROT_Y)
                poses[index].rot[1] = Bs_Angle12ToRadians(ReadU16(1));
            if (mask & ANIMKEY_ROT_Z)
                poses[index].rot[2] = Bs_Angle12ToRadians(ReadU16(1));
            if (mask & ANIMKEY_POS_X)
                poses[index].pos[0] = Bs_S16ToFloat(ReadU16(1));
            if (mask & ANIMKEY_POS_Y)
                poses[index].pos[1] = Bs_S16ToFloat(ReadU16(1));
            if (mask & ANIMKEY_POS_Z)
                poses[index].pos[2] = Bs_S16ToFloat(ReadU16(1));
            if (mask & ANIMKEY_SCALE_X)
                poses[index].scale[0] = Bs_Fixed12ToFloat(ReadU16(1));
            if (mask & ANIMKEY_SCALE_Y)
                poses[index].scale[1] = Bs_Fixed12ToFloat(ReadU16(1));
            if (mask & ANIMKEY_SCALE_Z)
                poses[index].scale[2] = Bs_Fixed12ToFloat(ReadU16(1));
        }
    }
    cursor = saved;
}
