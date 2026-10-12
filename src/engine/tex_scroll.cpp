/*
 * The texture scrolls: up to four id-lists named by the WAR type-0x83 header (g_texScrollListIds, reversal bits in
 * g_texScrollReverseMask) list DAV bitmap rects; each becomes a TexScroll entry plus a Texture holding a copy of the
 * rect's texels, and TexScroll_Update rotates the rect one row per call by copying from that copy. Game_Frame calls
 * TexScroll_UpdateAll while GF_UPDATE_OBJECTS is set; it advances rows at the reference game rate. Texture::Surface_LockForWrite is
 * declared returning long, as src/engine/texture.cpp defines it (same code).
 */
#include "sdw_enums.h"
#include "../sdk/ddraw.h"
#include "../sdk/crt.h"
#define SDW_MEMBERS_Texture                                                                                        \
    Texture(D3DApp *app, u32 width, u32 height, u32 format, s32 *result);                           \
    void Surface_LockForRead(DDSURFACEDESC2 *desc);  /* Lock(NULL, desc, 0x811 = WAIT|READONLY|NOSYSLOCK, NULL) */ \
    long Surface_LockForWrite(DDSURFACEDESC2 *desc); /* (returns long, as its definition declares it) */
#include "sdw_classes.h"

/* ---- globals ---- */
u16 g_texScrollCount = 0; /* number of TexScroll entries in use (really the texture-scroll count) */
u32 g_texScrollUnref_71af74[2] = {0}; /* unreferenced (placeholder, see above)                    */
TexScroll g_texScrolls[32] = {0};     /* stride 0x18                                                      */
u32 g_texScrollUnref_71b280 = 0;      /* unreferenced (placeholder, see above)                            */
Texture *g_texScrollBackup[16] = {0}; /* per entry: a copy of the rect's texels                          */
#include "game_state.h"
#include "draw2d.h"
#include "id_list.h"
#include "time.h"
static s32 g_texScrollRemainder = 0;
/* type-0x83 header bytes 8..9: bit i reverses list i. */
struct TexScrollReverseBits {
    u16 list0 : 1;
    u16 list1 : 1;
    u16 list2 : 1;
    u16 list3 : 1;
};
extern TexScrollReverseBits g_texScrollReverseMask;

void TexScroll_AddRect(const DavBitmapRec *rec, s8 step);

/* adds every rect of id-list listId as a scroll, stepping +1 row per update, or -1 when reverse is set. */
void TexScroll_AddList(u16 listId, s8 reverse)
{
    uptr *entries;
    u16 count;
    const DavBitmapRec *rec;
    u16 i;
    if (listId != 0) {
        if (reverse == 0)
            reverse = 1;
        else
            reverse = -1;
        entries = IdList_FindWithCount(listId, &count);
        for (i = 0; i < count; i++) {
            rec = (const DavBitmapRec *)*entries;
            entries++;
            TexScroll_AddRect(rec, reverse);
        }
    }
}

/* Sets up scroll from a DAV bitmap rect: the rect on its texture page, and a backup Texture holding a copy of its
 * texels, which TexScroll_Step copies back shifted. On an overridden page (retexture/, PolyBatcher::LoadTexturePages),
 * a whole multiple of the disc's 256-texel page, the rect is scaled to the page. The backup is returned. */
Texture *TexScroll_Setup(TexScroll *scroll, const DavBitmapRec *rec, s8 step)
{
    Texture *page = g_pPolyBin->textures[rec->page];
    Texture *backup;
    s32 res;
    RECT rect;
    scroll->scale = page->GetWidth() > 256 ? (s32)(page->GetWidth() / 256) : 1;
    scroll->x = (s16)(rec->u * scroll->scale);
    scroll->y = (s16)(rec->v * scroll->scale);
    scroll->w = (s16)(rec->width * scroll->scale);
    scroll->h = (s16)(rec->height * scroll->scale);
    scroll->texPage = rec->page;
    scroll->srcX = 0;
    scroll->srcY = 0;
    scroll->srcW = scroll->w;
    scroll->srcH = scroll->h;
    scroll->offset = 0;
    scroll->step = step;
    backup = new Texture(g_pD3DAppMain, (u32)scroll->w, (u32)scroll->h, page->GetFormat(), &res);
    rect.left = scroll->x;
    rect.top = scroll->y;
    rect.right = scroll->x + scroll->w;
    rect.bottom = scroll->y + scroll->h;
    g_renderDevice->CopyTexture(backup->GetSurface(), 0, 0, page->GetSurface(), (RdRect *)&rect);
    return backup;
}

/* Scrolls by step rows of the disc's page (step * scale on an override, the same speed on screen): offset =
 * (offset + step * scale) mod H, then rewrites the rect on its page from the backup: backup rows 0..H-offset-1 go to
 * page rows y+offset.., and the last offset backup rows to the top of the rect. The original kept the offset in an s8
 * and copied 16-bit texels; the offset is now an s16 (rects over 128 rows) and the copies are in bytes (32-bit pages). */
void TexScroll_Step(TexScroll *scroll, Texture *backup)
{
    Texture *page = g_pPolyBin->textures[scroll->texPage];
    DDSURFACEDESC2 pageDesc, backupDesc;
    s32 height = scroll->srcH, bytes = page->GetFormat() == TEXFMT_RGBA8 ? 4 : 2;
    s32 row, offset;
    u8 *to, *from;
    if (height <= 0)
        return;
    offset = (scroll->offset + scroll->step * scroll->scale) % height;
    scroll->offset = (s16)(offset < 0 ? offset + height : offset);
    page->Surface_LockForWrite(&pageDesc);
    backup->Surface_LockForRead(&backupDesc);
    to = (u8 *)pageDesc.lpSurface + (scroll->y + scroll->offset) * pageDesc.lPitch + scroll->x * bytes;
    from = (u8 *)backupDesc.lpSurface;
    for (row = 0; row < height - scroll->offset; row++, to += pageDesc.lPitch)
        memcpy(to, from + row * backupDesc.lPitch, (size_t)scroll->srcW * bytes);
    to = (u8 *)pageDesc.lpSurface + scroll->y * pageDesc.lPitch + scroll->x * bytes;
    for (; row < height; row++, to += pageDesc.lPitch)
        memcpy(to, from + row * backupDesc.lPitch, (size_t)scroll->srcW * bytes);
    page->Surface_Unlock();
    backup->Surface_Unlock();
}

/* adds the next TexScroll entry from a DAV bitmap rect (TexScroll_Setup). Neither the Texture's result code nor the 16
 * backup slots are checked, as in the original. */
void TexScroll_AddRect(const DavBitmapRec *rec, s8 step)
{
    g_texScrollBackup[g_texScrollCount] = TexScroll_Setup(&g_texScrolls[g_texScrollCount], rec, step);
    g_texScrollCount++;
}

/* scrolls entry index by one step. */
void TexScroll_Update(u16 index)
{
    TexScroll_Step(&g_texScrolls[index], g_texScrollBackup[index]);
}

/* at level load (Load_DAVnWAR): builds the scroll list from the four id-lists of the type-0x83 header. */
void TexScroll_Init(void)
{
    s32 total;
    u16 list;
    total = 0;
    g_texScrollCount = 0;
    g_texScrollRemainder = 0;
    for (list = 0; list < 4; list++)
        total += g_texScrollListIds[list];
    if (total != 0) {
        TexScroll_AddList(g_texScrollListIds[0], g_texScrollReverseMask.list0);
        TexScroll_AddList(g_texScrollListIds[1], g_texScrollReverseMask.list1);
        TexScroll_AddList(g_texScrollListIds[2], g_texScrollReverseMask.list2);
        TexScroll_AddList(g_texScrollListIds[3], g_texScrollReverseMask.list3);
    }
}

/* at level unload (Load_FreeLevel): deletes every backup Texture. The slots are not cleared. */
void TexScroll_FreeAll(void)
{
    u32 i;
    for (i = 0; i < g_texScrollCount; i++)
        delete g_texScrollBackup[i];
    g_texScrollCount = 0;
}

/* Scroll the original number of rows per unit of game time. */
void TexScroll_UpdateAll(void)
{
    s32 steps = Time_ScaleStep(1, g_texScrollRemainder);
    while (steps-- > 0)
        for (u16 i = 0; i < g_texScrollCount; i++)
            TexScroll_Update(i);
}
