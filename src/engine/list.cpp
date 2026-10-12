/*
 * - A node is a ListNode {void *data; ListNode *prev; ListNode *next} - 24 bytes, from g_listNodeHeap.
 */
#include "sdw_enums.h"
#include "timer.h"
#include "fixed_math.h"

#define SDW_MEMBERS_PolyBatcher \
    PolyBatcher(D3DApp *app, u32 capacity, const char *davPath, s32 *outPageCount); /* PolyBatcher_Construct */
#define SDW_MEMBERS_StreamPlayer StreamPlayer(); /* StreamPlayer_Construct */
#include "scenaric_props.h"
#include "sdw_classes.h"
#define SDW_INLINE_CINE_ISACTIVE 1
#include "cine_inlines.h"
#undef SDW_INLINE_CINE_ISACTIVE

/* this TU's copy of the cinematic header's static opcode stride table (payload bytes per key, by opcode; the cinematic
 * code indexes the copies in src/engine/cine1.cpp and src/engine/cine.cpp). Local definition standing in for that header
 * (the loader uses g_cinePlayer). */
static u8 g_cineOpStride[9] = {0, 8, 8, 4, 2, 2, 4, 2, 2};

/* "\LEV", referenced by no instruction (a level-directory prefix nothing uses any more). */
char g_strLevDir[] = "\\LEV";

Heap g_listNodeHeap; /* the list-node heap (every ListNode and list head) */
void *g_scratch32k;  /* its arena, malloc'd by Scratch32k_Alloc */

#include "../sdk/crt.h"

/* The DAV directory (DavHeader.dir): packed, its pointers sit at +6/+0xa/+0x12 (Load_DAV), which
 * the struct generator cannot lay out, so it is declared here. */
#include "sdw_fileptr.h"
#pragma pack(push, 1)
struct DavDirectory {
    u16 indexCount;  /* entries of `indices`; Res_GetValidatedIdList bounds an entry's index by it */
    u16 bitmapCount; /* records in `bitmaps`; Res_GetValidatedIdList bounds an entry's value by it */
    u16 unk04;
    SDW_DAVPTR(u16) indices;          /* relocated by Load_DAV; the id-list entries point into it */
    SDW_DAVPTR(DavBitmapRec) bitmaps; /* relocated by Load_DAV; 10-byte records (TexAtlas_GetPage) */
    u32 fileSize;          /* size of the whole .DAV file */
    SDW_DAVPTR(u32) idLists;          /* relocated by Load_DAV: {u32 count; id-list records} -> g_idListBlob */
};
#pragma pack(pop)

/* ---- globals ---- */
#include "game_state.h"
#include "pause_menu.h"
#include "../app/app_main.h"
#include "id_list.h"
#include "obj_grid.h"
#include "cine.h"
#include "draw2d.h"
#include "stream_player.h"
#include "time.h"
#include "progress.h"
#include "maths.h"
#include "text.h"
#include "sound_mgr.h"
#include "jpeg_mlt.h"
#include "load_war.h"
#include "load_warmeshes.h"
#include "scn_register.h"
#include "scenaric.h"
#include "../objects/instance.h"
#include "interface.h"
#include "progress_inventory.h"
#include "emitter.h"
#include "collide.h"
#include "shadow.h"
#include "../objects/camera.h"
#include "tex_scroll.h"
#include "transition.h"
#include "weather.h"
#include "../objects/menu.h"
#include "map.h"
#include "input.h"
#include "../objects/mcard.h"
#include "prompt.h"
#include "file.h"
#include "load_dav.h"
extern u32 g_gameFlags; /* 4 = WAR loaded, 8 = DAV loaded */

#define g_camPos (g_camera.pos)

/* ---- functions ---- */
void Debug_Printf(const char *fmt, ...); /* a no-op stub */
void Dialogue_Reset();

/* ======================================================================================================================
 * the arena of the list-node heap (g_listNodeHeap), malloc'd once.
 */

/* The list-node heap's size: four times the original's 0x8000, for the larger 64-bit nodes and 16-byte heap headers */
#define LIST_NODE_HEAP_SIZE (0x8000 * 4)

void Scratch32k_Alloc()
{
    g_scratch32k = malloc(LIST_NODE_HEAP_SIZE);
}

/* tears the list-node heap down and frees its arena. */
void Scratch32k_Free()
{
    g_listNodeHeap.Term();
    if (g_scratch32k) {
        free(g_scratch32k);
        g_scratch32k = 0;
    }
}

/* (re)initialises the list-node heap over the arena; old nodes are simply forgotten. */
void Scratch32k_Install()
{
    g_listNodeHeap.Init((u8 *)g_scratch32k, LIST_NODE_HEAP_SIZE);
}

/* ======================================================================================================================
 * the doubly-linked list primitives over the list-node heap.
 */

/* the number of nodes in a list (walks the next chain). */
u32 List_Count(ListNode **list)
{
    u32 count = 0;
    ListNode *node;

    for (node = *list; node; node = node->next)
        count++;
    return count;
}

/* allocates the head cell of an empty list (one pointer, zeroed). */
void List_Create(ListNode ***listOut)
{
    *listOut = (ListNode **)g_listNodeHeap.Alloc(sizeof(ListNode *));
    **listOut = 0;
}

/* allocates one node (three pointers); the caller fills its data pointer. */
void List_AllocateNode(ListNode **nodeOut)
{
    *nodeOut = (ListNode *)g_listNodeHeap.Alloc(3 * sizeof(void *));
}

/* pushes a node in front of the list's first node. */
void List_PushFront(ListNode **list, ListNode *node)
{
    node->next = *list;
    node->prev = 0;
    if (*list)
        (*list)->prev = node;
    *list = node;
}

/* the node whose next is 0, or 0 for an empty list. */
ListNode *List_GetLast(ListNode **list)
{
    ListNode *node;

    if (!*list)
        return 0;
    node = *list;
    while (node->next)
        node = node->next;
    return node;
}

/* unlinks a node; the head advances when the node is the first one. Does not free it. */
void List_Remove(ListNode **list, ListNode *node)
{
    ListNode *n = node;

    if (n == *list)
        *list = n->next;
    else
        n->prev->next = n->next;
    if (n->next)
        n->next->prev = n->prev;
}

/* returns a node to the list-node heap. */
void List_FreeNode(void *node)
{
    g_listNodeHeap.Free(node);
}

/* frees every node of a list (next is read before each free) and empties the head cell. */
void List_Clear(ListNode **list)
{
    ListNode *node = *list;

    while (node) {
        ListNode *cur = node;
        node = node->next;
        List_FreeNode(cur);
    }
    *list = 0;
}

/* List_Clear, then frees the head cell itself: the counterpart of List_Create. */
void List_Destroy(ListNode **list)
{
    List_Clear(list);
    g_listNodeHeap.Free(list);
}

/* the node at position index, or 0 past the end. */
ListNode *List_GetAt(ListNode **head, u16 index)
{
    ListNode *node = *head;
    while (node) {
        if (!index)
            return node;
        --index;
        node = node->next;
    }
    return 0;
}

/* ======================================================================================================================
 * the level loader, first part.
 */

/* Load_DAVnWAR evaluates it and drops the value. */
inline s32 Progress_OnFrontEndScreen()
{
    s8 level = g_pProgress->currentLevel;
    return level == SCENE_DEMO_A || level == SCENE_DEMO_B;
}

#define SDW_INLINE_FREE_GAME_CLEARFLAGS_U32 1
#include "game_state_inlines.h"
#undef SDW_INLINE_FREE_GAME_CLEARFLAGS_U32

/* which pass of Load_DAVnWAR takes WAR resource resIndex: 0 world object (mesh), 1 scenaric object,
 * 2 cinematic object, 3 installed by Install_WarResource, 4 none. The type is the table entry's top byte. */
u8 GetResourceType(int resIndex)
{
    u8 *blob = g_pDav->war.blob;
    u8 result = RESCLASS_IGNORE;
    switch ((g_pDav->war.table[resIndex] >> 24) & 0xff) {
        case WAR_RES_IGNORED_1:
        case WAR_RES_IGNORED_2:
            break;
        case WAR_RES_MESH:
        case WAR_RES_MODEL:
        case WAR_RES_MESH_B:
        case WAR_RES_SKY:
            result = RESCLASS_MESH;
            break;
        case WAR_RES_SCENARIC:
            result = RESCLASS_SCENARIC;
            break;
        case WAR_RES_CINEMATIC:
            result = RESCLASS_CINEMATIC;
            break;
        case WAR_RES_TYPE_8:
        case WAR_RES_TYPE_9:
        case WAR_RES_TYPE_27:
        case WAR_RES_COLL_GRID:
        case WAR_RES_COLL_TRIS:
        case WAR_RES_EXPORTS:
        case WAR_RES_HEADER3:
        case WAR_RES_OBJ_GRID:
        case WAR_RES_PAIRS:
        case WAR_RES_ANIM_NAMES:
            result = RESCLASS_DATA;
            break;
        default:
            if (((g_pDav->war.table[resIndex] >> 24) & 0xff) & WAR_RES_UNCOUNTED)
                result = RESCLASS_DATA;
            else
                Debug_Printf("Error in GetResourceType(): Unknown Object Type %x!\n",
                             (g_pDav->war.table[resIndex] >> 24) & 0xff);
            break;
    }
    return result;
}

/* publishes one class-3 WAR resource: the type (top byte) picks the global, the low 24 bits are its offset
 * in the WAR blob. Types 8 and 9 (and those with bit 0x40) have nothing to install. */
void Install_WarResource(int resIndex)
{
    u32 i;
    u8 *blob = g_pDav->war.blob;
    switch ((g_pDav->war.table[resIndex] >> 24) & 0xff) {
        case WAR_RES_COLL_GRID:
            g_pWarCollMap = (u16 *)(blob + (g_pDav->war.table[resIndex] & 0xffffff));
            break;
        case WAR_RES_COLL_TRIS:
            g_pWarCollTris = (CollTri *)(blob + (g_pDav->war.table[resIndex] & 0xffffff));
            break;
        /* Pointer-sized relocated copies of the two tables (freed with the WAR by Load_FreeWAR) */
        case WAR_RES_EXPORTS: {
            u32 *records = (u32 *)(blob + (g_pDav->war.table[resIndex] & 0xffffff)); /* {u32 count; records} */
            g_warExportCount = *records;
            if (g_warExportTable)
                free(g_warExportTable);
            g_warExportTable = Res_WidenIdLists(records + 1, g_warExportCount, blob);
            break;
        }
        case WAR_RES_PAIRS: {
            u32 *pairs = (u32 *)(blob + (g_pDav->war.table[resIndex] & 0xffffff)); /* {u32 count; pairs} */
            g_warRelocCount = *pairs;
            if (g_warRelocTable)
                free(g_warRelocTable);
            g_warRelocTable = Res_WidenPairs(pairs + 1, g_warRelocCount, blob);
            break;
        }
        case WAR_RES_ANIM_NAMES:
            g_animNameTable = blob + (g_pDav->war.table[resIndex] & 0xffffff);
            break;
        case WAR_RES_HEADER3:
            Game_SetWarLevelHeader((WarLevelHeader *)(blob + (g_pDav->war.table[resIndex] & 0xffffff)));
            break;
        case WAR_RES_OBJ_GRID:
            g_pWarObjGrid = (u16 *)(blob + (g_pDav->war.table[resIndex] & 0xffffff));
            break;
        case WAR_RES_TYPE_8:
        case WAR_RES_TYPE_9:
            return;
    }
}

/* loads a level: levelPath is the path without extension; .DAV (textures, id lists).SND.MLT (strings)
 * and .WAR (everything else), then builds the level's objects and brings up every level subsystem. 1 on success. */
u8 Load_DAVnWAR(const char *levelPath, Dav *dav)
{
    char davStr[5] = ".DAV";
    char extWar[5] = ".WAR";
    char szMlt[5] = ".MLT";
    char szSnd[5] = ".SND";
    u16 dummy2 = 0;
    u16 unused1 = 0;
    s32 texPages;
    char base[SDW_PATH_MAX];
    u32 rgb;
    char fullPath[SDW_PATH_MAX];
    u16 i;
    u16 j;
    u16 w;
    u16 iCine;
    u16 scnI;

    strcpy(base, levelPath);
    g_pDav = dav;
    g_cinePlayer.Reset();
    Dialogue_Reset();
    Rand_Reset();
    strcpy(fullPath, base);
    Debug_Printf("{Loading DAV...\n");
    strcat(fullPath, davStr);
    g_pPolyBin = new PolyBatcher(g_pD3DAppMain, g_maxImmediateTriangles, fullPath, &texPages);
    if (texPages == 0)
        goto error;
    if (Load_DAV(fullPath, g_pDav) < 0) {
        Debug_Printf("Load_DAVWAR():  File %s not found\n", fullPath);
        goto error;
    }
    g_gameFlags |= GF_LEVEL_LOADED_B;
    if (Font_LoadFromRes(FONT_DEBUG, DAV_IDI_IGLFONTE, FONT_KIND_DEBUG) < 0)
        Debug_Printf("Load_DAVnWAR(): Debug Fonte Loading\n");
    if (Font_LoadFromRes(FONT_GAME, DAV_IDI_IGLTYPO_, FONT_KIND_GAME) < 0)
        Debug_Printf("Load_DAVnWAR(): Game Fonte Loading\n");
    Font_CloneResized(FONT_GAME, FONT_GAME_SMALL, 10, 12);
    Debug_Printf("{Font loaded\n");
    Text_SetFont(FONT_DEBUG);

    Debug_Printf("{Loading SND...\n");
    strcpy(fullPath, base);
    strcat(fullPath, szSnd);
    if (Load_SND(fullPath) < 0)
        Debug_Printf("Load_DAVWAR(): File %s not found\n", fullPath);

    Debug_Printf("{Loading MLT...\n");
    strcpy(fullPath, base);
    strcat(fullPath, szMlt);
    if (Load_MLT(fullPath, &g_pDav->strings) < 0) {
        Debug_Printf("Load_DAVWAR(): File %s not found\n", fullPath);
        g_pDav->strings.listCount = 0;
        g_pDav->strings.lists = 0;
    }

    Debug_Printf("{Loading WAR...\n");
    strcpy(fullPath, base);
    strcat(fullPath, extWar);
    if (Load_WAR(fullPath, &g_pDav->war) < 0) {
        Debug_Printf("Load_DAVWAR():  File %s not found\n", fullPath);
        goto error;
    }
    Load_WarMeshes(fullPath, &g_pDav->war);
    g_gameFlags |= GF_LEVEL_LOADED_A;

    Debug_Printf("{Allocating Objects...\n");
    Time_Init();
    Scenaric_RegisterAllClasses();
    g_worldObjCount = 0;
    g_scnObjectCount = 0;
    g_cineObjectCount = 0;
    g_worldObjs = 0;
    g_scnActive = 0;
    g_scnObjects = 0;
    g_cineObjects = 0;
    i = 0;
    while (i < g_pDav->war.header->resourceCount) {
        switch (GetResourceType(i)) {
            case RESCLASS_MESH:
                g_worldObjCount++;
                break;
            case RESCLASS_SCENARIC:
                g_scnObjectCount++;
                break;
            case RESCLASS_CINEMATIC:
                g_cineObjectCount++;
                break;
        }
        i++;
    }
    if (g_worldObjCount != 0)
        g_worldObjs = (WorldObj **)malloc(g_worldObjCount * sizeof(WorldObj *));
    i = 0;
    while (i < g_pDav->war.header->resourceCount) {
        switch (GetResourceType(i)) {
            case RESCLASS_DATA:
                Install_WarResource(i);
                break;
        }
        i++;
    }
    g_scnActiveCapacity = g_scnObjectCount + 10;
    Scenaric_InitLevelState();
    g_scnActive = (ScnObject **)malloc((g_scnObjectCount + 10) * sizeof(ScnObject *));
    if (g_scnObjectCount != 0)
        g_scnObjects = (ScnObject **)malloc(g_scnObjectCount * sizeof(ScnObject *));
    if (g_cineObjectCount != 0)
        g_cineObjects = (ScnObject **)malloc(g_cineObjectCount * sizeof(ScnObject *));
    memset(g_scnActive, 0, (g_scnObjectCount + 10) * sizeof(ScnObject *));
    memset(g_scnObjects, 0, g_scnObjectCount * sizeof(ScnObject *));
    w = 0;
    scnI = 0;
    iCine = 0;
    i = 0;
    while (i < g_pDav->war.header->resourceCount) {
        switch (GetResourceType(i)) {
            case RESCLASS_MESH:
                g_worldObjs[w] = (WorldObj *)WorldObj_CreateFromResource(i);
                w++;
                break;
            case RESCLASS_SCENARIC:
                g_scnObjects[scnI] = Install_ScenaricResource(i);
                scnI++;
                break;
            case RESCLASS_CINEMATIC:
                g_cineObjects[iCine] = Install_CinematicResource(i);
                iCine++;
                break;
        }
        i++;
    }
    Debug_Printf("{Initialising links...\n");
    AttachLink_InitPool();
    Debug_Printf("{Initialising interface...\n");
    Interface_Init();
    Debug_Printf("{Initialising inventory...\n");
    Inventory_Init();
    Debug_Printf("{Initialising billboard sfx...\n");
    Sfx_InitSpriteSheets();
    Debug_Printf("{Initialising Collisions...\n");
    Collide_InitLevel();
    Debug_Printf("{Initialising Shadows...\n");
    Shadow_LoadLevel();
    Debug_Printf("{Initialising Camera...\n");
    Camera_InitSettings();
    Debug_Printf("{Initialising Clusters...\n");
    ObjGrid_Init(g_pWarObjGrid);
    TexScroll_Init();
    Transition_Init();
    Weather_LevelInit();
    if (g_weatherType == WEATHER_RAIN)
        Weather_InitRain(&g_camPos);
    else if (g_weatherType == WEATHER_SNOW)
        Weather_InitSnow(&g_camPos);
    rgb = Color_ExpandPs1((g_pDav->war.header->clearR << 16) + (g_pDav->war.header->clearG << 8) +
                          g_pDav->war.header->clearB);
    g_pPolyBin->SetClearColor(rgb);
    g_pViewFrustum->SetFogColor(rgb);
    g_scnActiveBaseCount = g_scnObjectCount;
    g_scnActiveHigh = 0;
    for (j = 0; j < g_scnObjectCount; j++) {
        if (g_scnObjects[j] != 0)
            g_scnObjects[j]->AddToWorld(0);
    }
    for (j = 0; j < g_scnObjectCount; j++) {
        if (g_scnObjects[j] != 0)
            g_scnObjects[j]->PostLoadInit();
    }
    Debug_Printf("{Initialising Menus...\n");
    Menu_Init(&g_pauseMenu);
    Debug_Printf("{Initialising Map...\n");
    Map_Init();
    Menus_LoadLevelUi();
    g_pStreamPlayer = new StreamPlayer();
    g_pStreamPlayer->Load_MusicVoiceBank();
    g_pStreamPlayer->LoadLevelMusic();
    Input_Init();
    MCard_Init();
    g_pTimer->Start();
    Text_ResetWindow();
    Progress_OnFrontEndScreen();
    return 1;

error:
    Debug_Printf("LoadDAVnWAR(): Error\n");
    return 0;
}

/* frees what Load_DAVnWAR built, in roughly the reverse order. The PolyBatcher and the StreamPlayer are
 * deleted without clearing their pointers, and g_pStreamPlayer is used without a NULL test. */
void Load_FreeLevel()
{
    if (g_cinePlayer.IsActive())
        g_cinePlayer.Stop();
    Prompt_End();
    Input_Unacquire();
    ObjGrid_Free();
    Camera_FreeLevel();
    Shadow_FreeLevel();
    Collide_ShutdownLevel();
    TexScroll_FreeAll();
    Menu_Init(&g_pauseMenu);
    /* Most likely a compiled-out debug check. */
    if (0)
        ;
    if (g_pDav != 0)
        Load_FreeMLT(&g_pDav->strings);
    if (g_gameFlags & GF_LEVEL_LOADED_A) {
        Load_FreeWarMeshes(&g_pDav->war);
        Load_FreeWAR(&g_pDav->war);
        Game_ClearFlags(GF_LEVEL_LOADED_A);
    }
    if (g_gameFlags & GF_LEVEL_LOADED_B) {
        Dav_Free(g_pDav);
        delete g_pPolyBin;
        Game_ClearFlags(GF_LEVEL_LOADED_B);
    }
    Sound_ShutdownChannels();
    if (g_worldObjs != 0) {
        free(g_worldObjs);
        g_worldObjs = 0;
    }
    if (g_scnActive != 0) {
        free(g_scnActive);
        g_scnActive = 0;
    }
    if (g_scnObjects != 0) {
        free(g_scnObjects);
        g_scnObjects = 0;
    }
    if (g_cineObjects != 0) {
        free(g_cineObjects);
        g_cineObjects = 0;
    }
    Text_Disable();
    g_pStreamPlayer->Halt();
    delete g_pStreamPlayer;
}

/* no callers. Meant to hand out the DAV bitmap-record table, but it assigns to its own by-value argument. */
void Dav_GetBitmapTable_Dead(DavBitmapRec *out)
{
    if (out != 0)
        out = g_pDav->header->dir->bitmaps;
}

/* the DAV id list resId, or NULL (count 0) when it is missing or an entry is out of range. The range check
 * reads the list's FIRST entry on every pass (list[0], never list[i]), so only that entry is really validated. */
uptr *Res_GetValidatedIdList(u16 resId, u16 *outCount)
{
    u16 i;
    uptr *list;
    DavDirectory *davDir = g_pDav->header->dir;
    s32 valid = 1;
    list = IdList_FindWithCount(resId, outCount);
    if (list != 0) {
        for (i = 0; i < *outCount; i++) {
            if (*(u16 *)*list >= davDir->bitmapCount || (u32)((u16 *)*list - (u16 *)davDir->indices) >= davDir->indexCount)
                valid = 0;
        }
    }
    if (valid)
        return list;
    *outCount = 0;
    return 0;
}
