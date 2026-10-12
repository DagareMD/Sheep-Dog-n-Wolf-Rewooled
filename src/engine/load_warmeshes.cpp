/*
 * The .WAR loader, mesh pass: Load_DAVnWAR calls Load_WarMeshes right after Load_WAR with the same .WAR path (and
 * ignores the result). It opens the file a second time as a Black Sheep "V2.6" file (BsFile), walks the WAR resource
 * table and the BsFile's frame directory in step, and for every mesh-type resource (WAR_RES_MESH, WAR_RES_MODEL,
 * WAR_RES_MESH_B, WAR_RES_SKY; bit 0x40 ignored) builds a Mesh from the current BsFile frame into the mesh cache
 * g_texObjects[g_texCount], keyed in g_texKeys by the resource's address in the WAR blob. 1 on success; 0 for a
 * WAR_RES_IGNORED_1 / _2 or unknown resource (the meshes built so far stay in the cache).
 * */
#define SDW_MEMBERS_BsFile BsFile(const char *path); /* BsFile_Ctor */
#define SDW_MEMBERS_Mesh Mesh();                     /* Mesh_Construct */
#include "sdw_classes.h"
#include "sdw_enums.h"

#include "draw2d.h"
#include "tex_table.h"
#include "../sdk/crt.h"

#define WAR_RES_TYPE(e) ((e) >> 24 & 0xff)
#define WAR_RES_OFFSET(e) ((e) & 0xffffff)

/* ----: the angle / scale / texture-coordinate helpers ---- */

/* clockwise 1/4096-turn angle, adjusted once if negative. */
float Math_Angle4096ToRadians(s16 angle)
{
    float radians = -(angle * 2.0f * 3.14159265358979f) / 4096.0f;
    if (radians < 0.0f)
        radians += 6.283185307179586f;
    return radians;
}

/* signed-byte rotation-key counterpart. */
float Math_Angle128ToRadians(s8 angle)
{
    float radians = -(angle * 2.0f * 3.14159265358979f) / 128.0f;
    if (radians < 0.0f)
        radians += 6.283185307179586f;
    return radians;
}

/* and: unsigned packed scale keys. */
float Math_U16ToUnitFloat(u16 value)
{
    return value / 1024.0f;
}
float Math_U8ToUnitFloat(u8 value)
{
    return value / 128.0f;
}

/* half-texel-corrected corner on a 256-pixel texture page. */
float Tex_CornerUV(u32 offset, s32 size, s32 base)
{
    return (offset * ((size - 1) / (float)size) + 0.5f + base) / 256.0f;
}

/* ----: the mesh pass ---- */

/* Resolve actor names through each WAR's directory; resource indices are local to the file. */
static s32 Load_FindNamedModel(BsFile *file, const char *modelName)
{
    if (!file->ok)
        return -1;
    file->FindFrameOfType(WAR_RES_ANIM_NAMES);
    if (file->frameType != WAR_RES_ANIM_NAMES)
        return -1;
    u16 modelCount = file->ReadU16(1);
    while (modelCount--) {
        u16 resource = file->ReadU16(1);
        u16 animCount = file->ReadU16(1);
        const char *name = (const char *)file->data + file->cursor;
        if (!strcmp(name, modelName))
            return resource;
        file->cursor += (strlen(name) + 2) & ~1;
        while (animCount--) {
            file->ReadU16(1);
            name = (const char *)file->data + file->cursor;
            file->cursor += (strlen(name) + 2) & ~1;
        }
    }
    return -1;
}

/* Resolve only the selected tail joints' materials through the level's DAV directory. */
static void Load_OutlineTextures(BsFile *file, const Model *model, u32 cutoutJoints)
{
    u8 selectedVertices[256] = {};
    u32 firstVertex = 0;
    for (u32 joint = 1; joint < model->nbJoints; ++joint) {
        u32 endVertex = firstVertex + model->joints[joint].vertexCount;
        if (endVertex > 256)
            return;
        for (u32 vertex = firstVertex; vertex < endVertex; ++vertex)
            selectedVertices[vertex] = (cutoutJoints & (1u << joint)) != 0;
        firstVertex = endVertex;
    }
    file->cursor += 4;
    u32 entries = file->ReadU32(1);
    file->ReadU16(1); /* vertex count */
    u16 groupCount = file->ReadU16(1);
    file->cursor = entries;
    while (groupCount--) {
        s32 groupStart = file->cursor;
        u16 kind = file->ReadU16(1);
        u16 count = file->ReadU16(1);
        if (kind == BS_ENTRY_FT3) {
            while (count--) {
                s32 faceStart = file->cursor;
                bool selected = false;
                for (u32 corner = 0; corner < 3; ++corner) {
                    u8 vertex = file->ReadU8(1);
                    selected |= selectedVertices[vertex] != 0;
                }
                file->cursor = faceStart + 12;
                u32 material = file->ReadU32(1);
                if (selected)
                    g_pPolyBin->ConfigureOutlineTexture(file->vdx->records[file->vdx->entries[material]]);
                file->cursor = faceStart + 24;
            }
        }
        file->cursor = groupStart;
        file->SkipEntry();
    }
}

/* Ralph's hand borders are twelve untextured black G3 triangles, distinct from the two finger-line
 * textures. Select only solid-black faces entirely in the hand joints (17 and 21). */
static void Load_RalphHandOutlines(Mesh *mesh, const Model *model)
{
    struct FlatVertex {
        float x, y, z, rhw;
        u32 color, specular;
    };
    u8 handVertices[256] = {};
    u32 firstVertex = 0;
    for (u32 joint = 1; joint < model->nbJoints; ++joint) {
        u32 endVertex = firstVertex + model->joints[joint].vertexCount;
        if (endVertex > 256)
            return;
        for (u32 vertex = firstVertex; vertex < endVertex; ++vertex)
            handVertices[vertex] = joint == 17 || joint == 21;
        firstVertex = endVertex;
    }
    u8 *outlineFaces = new u8[mesh->faceCount];
    for (u32 i = 0; i < mesh->faceCount; ++i) {
        const RenderPoly &face = mesh->faces[i];
        bool outline = face.type == RPOLY_OPAQUE;
        if (outline) {
            const FlatVertex *vertices = (const FlatVertex *)face.verts;
            for (u32 corner = 0; corner < 3; ++corner)
                outline &= face.poly.idx[corner] < 256 && handVertices[face.poly.idx[corner]] &&
                           !(vertices[corner].color & 0x00ffffff);
        }
        outlineFaces[i] = outline;
    }
    g_pPolyBin->ConfigureOutlineMesh(mesh, outlineFaces);
    delete[] outlineFaces;
}

int Load_WarMeshes(const char *path, WarFile *war)
{
    u32 i;
    BsFile *bsFile;

    bsFile = new BsFile(path);
    s32 ralphModel = Load_FindNamedModel(bsFile, "ACOYOT01");
    s32 samModel = Load_FindNamedModel(bsFile, "ASAMSH01");
    bsFile->FirstFrame();
    g_texCount = 0;
    for (i = 0; i < war->header->resourceCount; i++) {
        switch (WAR_RES_TYPE(war->table[i]) & WAR_RES_TYPE_MASK) {
            case WAR_RES_IGNORED_1:
            case WAR_RES_IGNORED_2:
                goto fail;
            case WAR_RES_MESH:
            case WAR_RES_MODEL:
            case WAR_RES_MESH_B:
            case WAR_RES_SKY: {
                const Model *ralph = 0;
                if (((s32)i == ralphModel || (s32)i == samModel) && bsFile->frameType == WAR_RES_MODEL) {
                    s32 cursor = bsFile->cursor;
                    const Model *model = (const Model *)(war->blob + WAR_RES_OFFSET(war->table[i]));
                    if ((s32)i == ralphModel && model->nbJoints == 31) {
                        ralph = model;
                        Load_OutlineTextures(bsFile, model, 1u << 30);
                    } else if ((s32)i == samModel && model->nbJoints == 29)
                        Load_OutlineTextures(bsFile, model, 1u << 8);
                    bsFile->cursor = cursor;
                }
                g_texObjects[g_texCount] = new Mesh;
                if (g_texObjects[g_texCount]->BuildFromBsFile(g_pD3DAppMain, bsFile) && ralph)
                    Load_RalphHandOutlines(g_texObjects[g_texCount], ralph);
                g_texKeys[g_texCount] = WAR_RES_OFFSET(war->table[i]) + (uptr)war->blob;
                g_texCount++;
                break;
            }
            case WAR_RES_SCENARIC:
            case WAR_RES_TYPE_8:
            case WAR_RES_TYPE_9:
            case WAR_RES_CINEMATIC:
            case WAR_RES_TYPE_27:
            case WAR_RES_COLL_GRID:
            case WAR_RES_COLL_TRIS:
            case WAR_RES_EXPORTS:
            case WAR_RES_HEADER3:
            case WAR_RES_OBJ_GRID:
            case WAR_RES_PAIRS:
            case WAR_RES_ANIM_NAMES:
                break;
            default:
                goto fail;
        }
        bsFile->NextFrame();
    }
    delete bsFile;
    return 1;
fail:
    delete bsFile;
    return 0;
}

/* ----: Load_FreeWarMeshes ---- */
/* The unused WarFile argument is present in the existing loader caller. */
void Load_FreeWarMeshes(WarFile *war)
{
    u32 index;
    for (index = 0; index < g_texCount; ++index)
        if (g_texObjects[index])
            delete g_texObjects[index];
}
