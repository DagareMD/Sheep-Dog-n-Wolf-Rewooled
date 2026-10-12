#include "sdw_types.h"
#include "screen.h"
#include "sdw_enums.h"
#include "scenaric_props.h"


#define SDW_MEMBERS_ScnObject void SetPos(Vec3s *p);

#define SDW_MEMBERS_Mat44 Mat44(); /* Mat44_Ctor */
#include "sdw_classes.h"
#define SDW_INLINE_PROGRESS_SETLEVELDONE_S8 1
#include "progress_inlines.h"
#undef SDW_INLINE_PROGRESS_SETLEVELDONE_S8
#define SDW_INLINE_SCNOBJECT_SETPOS_VEC3S 1
#include "scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_SETPOS_VEC3S
#define SDW_INLINE_SCNOBJECT_GETCLASSID 1
#define SDW_INLINE_SCNOBJECT_ISKEPT 1
#define SDW_INLINE_SCNOBJECT_SETKEPT_S32 1
#include "scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETCLASSID
#undef SDW_INLINE_SCNOBJECT_ISKEPT
#undef SDW_INLINE_SCNOBJECT_SETKEPT_S32
#include "sdw_empty_call.h"

#include "../sdk/windef.h"
#include "../sdk/crt.h"

/* ---- globals (named in the tables) ---- */
#include "progress.h"
#include "draw2d.h"
#include "stream_player.h"
#include "../objects/video_sequence.h"
#include "interface.h"
#include "../app/app_main.h"
#include "scenaric.h"
#include "sfx_volume.h"
#include "input.h"
#include "game_level.h"
#include "list.h"
#include "text.h"
#include "scn_tools.h"
#include "approach.h"
#include "lerp.h"
extern const float g_viewDistFar;  /* 12000.0, the view distance at setting 0 */
extern const float g_viewDistNear; /* 4000.0, the view distance at setting 255 */
extern u32 g_gameFlags;
extern s32 g_dt;
extern s32 g_frameCount2;
extern u32 *g_screenLayerBase;

#define g_camPos (g_camera.pos)
extern "C" s16 g_sinTable4096[5122];
extern "C" const s16 *g_pCosTable;

/* ---- functions ---- */
LONG Reg_CreateSubKey(HKEY *out, const char *name);
void Reg_CloseKey(HKEY *key);
DWORD Reg_ReadBinary(HKEY key, const char *name, void *buf, DWORD bufSize);
char Video_PlaySequence(FmvList *list);
s32 Scenaric_FindByClass(u16 classId, ScnObject **out, s32 maximum);

/* a colour word whose channel bytes are also read and written one by one */
union ColorBytes {
    u32 value;
    u8 c[4];
};

u16 Inventory_NextClass(u16 classId);
u16 Inventory_PrevClass(u16 classId);
void Inventory_Add(ScnObject *obj);
void Inventory_Remove(ScnObject *obj);
ScnObject *Inventory_GetSelectedObject();
void InvWheel_DrawSlot(u16 classId, s32 x, s32 y, u32 color, u8 crayonFrame);
void InvWheel_GetSlotPosColor(s32 *outXY, ColorBytes *outColor, s16 angle, s32 slideY);
void InvWheel_Draw(s16 angle, u8 crayonFrame, s32 slideY);
void ItemFly_Stop();

/* inline: a scene number that is a real level (0..31), materialised as 0/1 */
#define SDW_INLINE_FREE_ISLEVELSCENE_S8 1
#include "progress_inlines.h"
#undef SDW_INLINE_FREE_ISLEVELSCENE_S8

/* inline: the attract-demo alternation bit, stored as `on != 0` */
#define SDW_INLINE_PROGRESS_SETSECONDDEMONEXT_S32 1
#include "progress_inlines.h"
#undef SDW_INLINE_PROGRESS_SETSECONDDEMONEXT_S32

/* inline: runtime flag 0 as a 0/1 value, clear = 1 (the branchy 0/1 temporary) */
#define SDW_INLINE_PROGRESS_FIELDACFLAGCLEAR 1
#include "progress_inlines.h"
#undef SDW_INLINE_PROGRESS_FIELDACFLAGCLEAR

/* inline: the first node of a list */
#define SDW_INLINE_FREE_LIST_FIRST_LISTNODE 1
#include "list_inlines.h"
#undef SDW_INLINE_FREE_LIST_FIRST_LISTNODE

/* inline: clears game flags; the mask is complemented at run time */
#define SDW_INLINE_FREE_GAMEFLAGS_CLEAR_U32 1
#include "game_state_inlines.h"
#undef SDW_INLINE_FREE_GAMEFLAGS_CLEAR_U32

#define SDW_INLINE_FREE_SCENARIC_CLASSFLAGS_U16 1
#include "scenaric_inlines.h"
#undef SDW_INLINE_FREE_SCENARIC_CLASSFLAGS_U16

u16 g_invSelClass;                     /* g_invSelectedClass: 0xffff = empty hands          bucket 93 */
u16 g_invSpare_6cfc2a;                 /* unreferenced                                        bucket 102 */
u8 g_invFlyDir;                        /* g_itemFlyMode                                       bucket 123 */
ScnObject *g_itemFlyObj;               /* g_itemFlyObject                                     bucket 164 */
Vec3s g_itemFlyStart;                  /* g_itemFlyFrom                                       bucket 213 */
Vec3s g_itemFlightViewOffset;          /* g_itemFlyCamOffset                                  bucket 225 */
Vec3s g_itemFlyCur;                    /* g_itemFlyPos                                        bucket 278 */
u16 g_invUnused6cfc4a;                 /* unreferenced                                        bucket 313 */
u16 g_itemFlightTime;                  /* g_itemFlyT                                          bucket 326 */
u16 g_invUnused_6cfc4e;                /* unreferenced                                        bucket 441 */
u8 g_invMailboxCount;                  /* bucket 450 */
ScnObject *g_inventoryMailboxTable[6]; /* g_invMailboxes                                  bucket 486 */
u8 g_invWheelTurn;                     /* g_invWheelSide                                      bucket 544 */
s16 g_inventoryWheelYaw;               /* g_invWheelAngle                                     bucket 567 */
s16 g_invWheelSlide;                   /* g_invWheelSlideY                                    bucket 568 */
u8 g_inventoryAllCommit;               /* g_invAllCommitted                                   bucket 616 */
ListNode *g_inventoryLists[200];       /* one list head per class id                          bucket 699 */

/* ======================================================================== Inventory */

u32 Inventory_CountClass(u16 classId)
{
    return List_Count(&g_inventoryLists[classId]);
}

/* the first class after classId that holds an item; from empty hands (0xffff) the first one of all */
u16 Inventory_NextClass(u16 classId)
{
    s32 i;

    if (classId != CLASSID_NONE) {
        for (i = (u16)(classId + 1); i < 200; i++)
            if (List_First(&g_inventoryLists[i]))
                return i;
    } else {
        for (i = 0; i < 200; i++)
            if (List_First(&g_inventoryLists[i]))
                return i;
    }
    return CLASSID_NONE;
}

/* the last class before classId that holds an item; from empty hands the last one of all (down to 1) */
u16 Inventory_PrevClass(u16 classId)
{
    s32 i;

    if (classId != CLASSID_NONE) {
        for (i = classId - 1; i >= 0; i--)
            if (List_First(&g_inventoryLists[i]))
                return i;
    } else {
        for (i = 199; i > 0; i--)
            if (List_First(&g_inventoryLists[i]))
                return i;
    }
    return CLASSID_NONE;
}

/* carries an object. A composite whose two parts are both kept is kept itself; an unkept object means
 * the inventory has pickups a checkpoint has not committed yet. */
void Inventory_Add(ScnObject *obj)
{
    ListNode *node;
    ScnObject *parts[2];

    List_AllocateNode(&node);
    node->data = obj;
    List_PushFront(&g_inventoryLists[obj->GetClassId()], node);
    if ((Scenaric_ClassFlags(obj->GetClassId()) & SCN_CF_COMPOSITE_ITEM) &&
        obj->HandleMessage(0, MSG_ITEM_SPLIT_QUERY, parts) && parts[0]->IsKept() && parts[1]->IsKept())
        obj->SetKept(1);
    if (!obj->IsKept())
        g_inventoryAllCommit = 0;
}

/* drops an object from its class list; the selection falls back to empty hands when that list empties */
void Inventory_Remove(ScnObject *obj)
{
    ListNode *n;
    ListNode **list;

    list = &g_inventoryLists[obj->GetClassId()];
    for (n = *list; n; n = n->next) {
        if (n->data == obj) {
            List_Remove(list, n);
            List_FreeNode(n);
            if (g_invSelClass == obj->GetClassId() && !List_First(list))
                g_invSelClass = CLASSID_NONE;
            break;
        }
    }
}

/* the object in hand: the first of the selected class, or 0 for empty hands */
ScnObject *Inventory_GetSelectedObject()
{
    ListNode **list;

    if (g_invSelClass == CLASSID_NONE)
        return 0;
    list = &g_inventoryLists[g_invSelClass];
    return (ScnObject *)List_First(list)->data;
}

void Inventory_SelectClass(u16 classId)
{
    if (List_First(&g_inventoryLists[classId]))
        g_invSelClass = classId;
    else
        g_invSelClass = CLASSID_NONE;
}

/* one step forward on the carousel. With only two stops (one class and empty hands) the side flag picks
 * which neighbour is shown, and from empty hands the first press only turns the wheel. */
ScnObject *Inventory_SelectNext()
{
    u16 after;
    u16 newSel;
    u16 selected;

    newSel = Inventory_NextClass(g_invSelClass);
    if (newSel != g_invSelClass) {
        selected = g_invSelClass;
        after = Inventory_NextClass(newSel);
        if (selected == after) {
            if (g_invSelClass != CLASSID_NONE) {
                g_invWheelTurn = 0;
                g_invSelClass = newSel;
            } else if (g_invWheelTurn) {
                g_invWheelTurn = 0;
                g_invSelClass = newSel;
            } else {
                g_invWheelTurn = 1;
            }
        } else {
            g_invSelClass = newSel;
        }
        g_inventoryWheelYaw = 0xd55;
    }
    return Inventory_GetSelectedObject();
}

/* the mirror of Inventory_SelectNext */
ScnObject *Inventory_SelectPrev()
{
    u16 prev;
    u16 before;
    u16 cur;

    prev = Inventory_PrevClass(g_invSelClass);
    if (prev != g_invSelClass) {
        before = Inventory_PrevClass(prev);
        cur = g_invSelClass;
        if (before == cur) {
            if (g_invSelClass != CLASSID_NONE) {
                g_invWheelTurn = 1;
                g_invSelClass = prev;
            } else if (g_invWheelTurn) {
                g_invWheelTurn = 0;
            } else {
                g_invSelClass = prev;
            }
        } else {
            g_invSelClass = prev;
        }
        g_inventoryWheelYaw = 0xaab;
    }
    return Inventory_GetSelectedObject();
}

void Inventory_ClearSelection()
{
    g_invSelClass = CLASSID_NONE;
}

/* one carousel slot: the class icon with its count, over the crayon frame */
void InvWheel_DrawSlot(u16 classId, s32 x, s32 y, u32 color, u8 crayonFrame)
{
    s32 top;
    s32 sx;
    u32 *dst;
    u32 count;
    ListNode **list;

    if (classId != CLASSID_NONE) {
        list = &g_inventoryLists[classId];
        count = List_Count(list);
        if (count > 1) {
            Text_SetCursor(x + 2, y + 2);
            Text_PrintFmt("x%d", count);
        }
        Scenaric_DrawClassIcon(g_screenLayerBase + 10, classId, x, y, 0, 0, color);
    }
    top = y - (g_animSpriteCrayon1.height >> 1);
    sx = x - (g_animSpriteCrayon1.width >> 1);
    dst = g_screenLayerBase + 10;
    g_animSpriteCrayon1.Draw(dst, sx, top, sx + g_animSpriteCrayon1.width, top + g_animSpriteCrayon1.height, color,
                             crayonFrame, 0);
}

s32 InvWheel_IsRotating()
{
    return g_inventoryWheelYaw != 0xc00;
}

/* the object the item-fly effect is carrying, while it runs */
ScnObject *ItemFly_GetObject()
{
    if (g_gameFlags & GF_ITEM_FLY)
        return g_itemFlyObj;
    return 0;
}

/* opening the wheel resets its angle */
void Inventory_SetWheelOpen(s32 open)
{
    if (open) {
        if (!(g_gameFlags & GF_ITEM_WHEEL_OPEN)) {
            g_gameFlags |= GF_ITEM_WHEEL_OPEN;
            g_inventoryWheelYaw = 0xc00;
        }
    } else {
        GameFlags_Clear(GF_ITEM_WHEEL_OPEN);
    }
}

/* a slot's screen position on the wheel's ellipse, and its colour: 0x808080 at the front (angle 0xc00),
 * fading to 0x303030 over a third of a slot step (0x155) either side. */
void InvWheel_GetSlotPosColor(s32 *outXY, ColorBytes *outColor, s16 angle, s32 slideY)
{
    ColorBytes hi;
    ColorBytes dim;
    s32 d;

    outXY[0] = 256 + (g_pCosTable[angle] * 150 >> 12);
    outXY[1] = -150 - (g_sinTable4096[angle] * 200 >> 12) + slideY;
    if (1) {
        d = (s16)((s16)((angle - 0x400) & 0xfff) - 0x800);
        if (d != 0) {
            d = d >= 0 ? d : -d;
            if (d < 0x155) {
                hi.value = 0x808080;
                dim.value = 0x303030;
                d = (d << 12) / 0x155;
                outColor->c[0] = hi.c[0] + ((dim.c[0] - hi.c[0]) * d >> 12);
                outColor->c[1] = hi.c[1] + ((dim.c[1] - hi.c[1]) * d >> 12);
                outColor->c[2] = hi.c[2] + ((dim.c[2] - hi.c[2]) * d >> 12);
            } else {
                outColor->value = 0x303030;
            }
        } else {
            outColor->value = 0x808080;
        }
    } else {
        outColor->value = 0x303030;
    }
}

/* the carousel: the selected class in front, its neighbours either side, and while the wheel turns the
 * class coming into view. */
void InvWheel_Draw(s16 angle, u8 crayonFrame, s32 slideY)
{
    s32 a[2], b[2];
    ColorBytes centreColor;
    InvWheel_GetSlotPosColor(a, &centreColor, 0, slideY);
    InvWheel_GetSlotPosColor(b, &centreColor, 0x800, slideY);
    HudElement hud(HudElement::Centre, HudElement::Start,
                   (a[0] + b[0]) * 0.5f, (a[1] + b[1]) * 0.5f);
    s32 a4;
    u16 next;
    s32 pos[2];
    u16 prev;
    ColorBytes color;
    u16 extra;
    s32 single;

    Text_SetFont(FONT_DEBUG);
    Text_SetColor(0x505090);
    Text_SetWindow(g_screenLayerBase + 8, 0, 0, 0x200, 0xf0, 1);
    prev = Inventory_PrevClass(g_invSelClass);
    next = Inventory_NextClass(g_invSelClass);
    single = 0;
    if (prev == next) {
        if (prev == g_invSelClass)
            single = 1;
        else if (g_invWheelTurn)
            prev = CLASSID_NONE;
        else
            next = CLASSID_NONE;
    }
    InvWheel_GetSlotPosColor(pos, &color, angle & 0xfff, slideY);
    InvWheel_DrawSlot(g_invSelClass, pos[0], pos[1], color.value, crayonFrame);
    if (!single) {
        InvWheel_GetSlotPosColor(pos, &color, (angle - 0x155) & 0xfff, slideY);
        InvWheel_DrawSlot(prev, pos[0], pos[1], color.value, 0);
        InvWheel_GetSlotPosColor(pos, &color, (angle + 0x155) & 0xfff, slideY);
        InvWheel_DrawSlot(next, pos[0], pos[1], color.value, 0);
    }
    if (angle != 0xc00) {
        if (angle > 0xc00) {
            extra = Inventory_PrevClass(prev);
            a4 = angle - 0x2aa;
        } else {
            extra = Inventory_NextClass(next);
            a4 = angle + 0x2aa;
        }
        if (extra == g_invSelClass)
            extra = CLASSID_NONE;
        InvWheel_GetSlotPosColor(pos, &color, a4 & 0xfff, slideY);
        InvWheel_DrawSlot(extra, pos[0], pos[1], color.value, 0);
    }
    Text_SetColor(0x808080);
    Hud_EndBox_stub();
}

/* once per frame from Scenaric_RenderAll, while the wheel is at least partly on screen */
void InvWheel_Render()
{
    if (g_invWheelSlide > -0x20)
        InvWheel_Draw(g_inventoryWheelYaw, ((u32)g_frameCount2 >> 2) & 0xff, g_invWheelSlide);
}

/* starts the item-fly effect: mode 2 carries obj from fromPos into the camera (picked up), mode 1 from the
 * camera back to fromPos (taken out). The carried class becomes the selection. */
void ItemFly_Start(ScnObject *obj, u8 mode, const Vec3s *fromPos)
{
    g_itemFlyObj = obj;
    g_invFlyDir = mode;
    g_itemFlyStart.x = fromPos->x;
    g_itemFlyStart.y = fromPos->y;
    g_itemFlyStart.z = fromPos->z;
    g_itemFlightViewOffset.x = 0;
    g_itemFlightViewOffset.y = -0x78;
    g_itemFlightViewOffset.z = 0x180;
    if (mode == ITEMFLY_TO_INVENTORY)
        g_itemFlightTime = 0;
    else
        g_itemFlightTime = 0xffe;
    g_gameFlags |= GF_ITEM_FLY;
    g_inventoryWheelYaw = 0xc00;
    if (List_First(&g_inventoryLists[g_itemFlyObj->GetClassId()]))
        g_invSelClass = g_itemFlyObj->GetClassId();
    else
        g_invSelClass = CLASSID_NONE;
}

void ItemFly_SetFromPos(const Vec3s *pos)
{
    g_itemFlyStart.x = pos->x;
    g_itemFlyStart.y = pos->y;
    g_itemFlyStart.z = pos->z;
}

void ItemFly_Stop()
{
    g_invFlyDir = ITEMFLY_IDLE;
    GameFlags_Clear(GF_ITEM_FLY);
}

/* once per frame at the end of Game_Frame: moves the flying item along its path (the camera-space point
 * (0, -120, 384) turned into world space, and fromPos) and draws it; turns the wheel back to rest; slides the wheel in
 * while it is open or an item flies, and out otherwise. */
void Inventory_UpdateFx()
{
    s16 dt;
    Vec3s target;

    dt = 0;
    switch (g_invFlyDir) {
        case ITEMFLY_TO_WOLF:
        case ITEMFLY_TO_INVENTORY:
            dt = g_dt * 0x2580 >> 12;
            if (g_invFlyDir != ITEMFLY_TO_INVENTORY)
                dt = -dt;
            if (g_itemFlightTime < 0xfff) {
                Mat44 m;

                g_camera.viewMat.Transpose3x3(&m);
                m.m[3][0] = m.m[3][1] = m.m[3][2] = 0;
                target.x = g_itemFlightViewOffset.x * m.m[0][0] + g_itemFlightViewOffset.y * m.m[1][0] +
                           g_itemFlightViewOffset.z * m.m[2][0] + m.m[3][0];
                target.y = g_itemFlightViewOffset.x * m.m[0][1] + g_itemFlightViewOffset.y * m.m[1][1] +
                           g_itemFlightViewOffset.z * m.m[2][1] + m.m[3][1];
                target.z = g_itemFlightViewOffset.x * m.m[0][2] + g_itemFlightViewOffset.y * m.m[1][2] +
                           g_itemFlightViewOffset.z * m.m[2][2] + m.m[3][2];
                target.x += g_camPos.x;
                target.y += g_camPos.y;
                target.z += g_camPos.z;
                Lerp_SetVecTarget(&target);
                Vec3s_LerpToTarget(&g_itemFlyCur, &g_itemFlyStart, g_itemFlightTime);
                g_itemFlightTime += (u16)dt;
                g_itemFlyObj->SetPos(&g_itemFlyCur);
                g_itemFlyObj->Render(&g_camera);
            } else {
                ItemFly_Stop();
            }
            break;
    }
    g_inventoryWheelYaw = Math_StepAngleTowards(g_inventoryWheelYaw, 0xc00, 0x400);
    if ((g_gameFlags & GF_ITEM_FLY) || (g_gameFlags & GF_ITEM_WHEEL_OPEN)) {
        if (g_invWheelSlide < 0) {
            g_invWheelSlide += (s16)(g_dt * 0xb4 >> 12);
            if (g_invWheelSlide > 0)
                g_invWheelSlide = 0;
        }
    } else if (g_invWheelSlide > -0x20) {
        g_invWheelSlide -= (s16)(g_dt * 0xb4 >> 12);
        if (g_invWheelSlide <= -0x20)
            g_invWheelSlide = -0x20;
    }
}

/* per level: empty hands, all lists emptied (their nodes live in the level heap), the level's Mailboxes */
void Inventory_Init()
{
    s32 i;

    g_invSelClass = CLASSID_NONE;
    for (i = 0; i < 200; i++)
        g_inventoryLists[i] = 0;
    g_invMailboxCount = Scenaric_FindByClass(CLASSID_MAILBOX, g_inventoryMailboxTable, 6);
    g_invWheelSlide = -0x20;
    g_inventoryWheelYaw = 0xc00;
    g_invWheelTurn = 0;
    g_inventoryAllCommit = 1;
}

/* on a restart (Wolf_Reset): everything picked up since the last checkpoint goes back. Pass 1 splits the
 * unkept composites into their parts; pass 2 puts every unkept object back into the world and resets it (msg 0x55).
 * The Mailboxes are told to roll back as well. */
void Inventory_DropUncommitted()
{
    ScnObject *parts[2];
    s32 i;
    s32 ret;
    ListNode *walk;
    ScnObject *item;
    ListNode **head;

    if (!g_inventoryAllCommit) {
        for (i = 0, head = g_inventoryLists; i < 200; i++, head++) {
            walk = *head;
            while (walk) {
                item = (ScnObject *)walk->data;
                walk = walk->next;
                if ((Scenaric_ClassFlags(item->GetClassId()) & SCN_CF_COMPOSITE_ITEM) && !item->IsKept() &&
                    item->HandleMessage(0, MSG_ITEM_SPLIT_QUERY, parts)) {
                    Inventory_Add(parts[0]);
                    parts[0]->HandleMessage(0, MSG_INVENTORY_STORED, 0);
                    Inventory_Add(parts[1]);
                    parts[1]->HandleMessage(0, MSG_INVENTORY_STORED, 0);
                    item->HandleMessage(0, MSG_ITEM_CONSUMED, 0);
                    item->HandleMessage(0, MSG_INVENTORY_TAKE_OUT, 0);
                    Inventory_Remove(item);
                }
            }
        }
        for (i = 0, head = g_inventoryLists; i < 200; i++, head++) {
            walk = *head;
            while (walk) {
                item = (ScnObject *)walk->data;
                walk = walk->next;
                if (!item->IsKept()) {
                    Inventory_Remove(item);
                    item->AddToWorld(0);
                    ret = item->HandleMessage(0, MSG_CHECKPOINT_ROLLBACK, 0);
                }
            }
        }
    }
    for (i = 0; i < g_invMailboxCount; i++)
        g_inventoryMailboxTable[i]->HandleMessage(0, MSG_CHECKPOINT_ROLLBACK, (void *)1);
}

/* at a checkpoint (Wolf message 0x402): every carried object, and both parts of a carried composite,
 * become kept. The Mailboxes are told to commit (msg 0x600) every time. */
void Inventory_CommitAtCheckpoint()
{
    ScnObject *parts[2];
    s32 i;
    ListNode *walk;
    ScnObject *item;
    ListNode **head;

    if (!g_inventoryAllCommit) {
        head = g_inventoryLists;
        for (i = 0; i < 200; i++) {
            for (walk = *head; walk; walk = walk->next) {
                item = (ScnObject *)walk->data;
                if (!item->IsKept()) {
                    item->SetKept(1);
                    if ((Scenaric_ClassFlags(item->GetClassId()) & SCN_CF_COMPOSITE_ITEM) &&
                        item->HandleMessage(0, MSG_ITEM_SPLIT_QUERY, parts)) {
                        parts[0]->SetKept(1);
                        parts[1]->SetKept(1);
                    }
                }
            }
            head++;
        }
        g_inventoryAllCommit = 1;
    }
    for (i = 0; i < g_invMailboxCount; i++)
        g_inventoryMailboxTable[i]->HandleMessage(0, MSG_CHECKPOINT_COMMIT, (void *)1);
}
