#include "sdw_types.h"
class Mat44;

#include "sdw_enums.h"
#include "../sdk/win32.h"
#include "../sdk/d3d7.h"
#include "../sdk/crt.h"


#define SDW_MEMBERS_D3DApp inline void ResetStateFlags(u32 flags);
#include "sdw_classes.h"
#define SDW_INLINE_SCREEN_LAYERS4_U16 1
#define SDW_INLINE_SCREEN_LAYERS60_U16 1
#include "screen_inlines.h"
#undef SDW_INLINE_SCREEN_LAYERS4_U16
#undef SDW_INLINE_SCREEN_LAYERS60_U16
#include "sdw_enums.h"
#include "../sdk/d3d7.h"
#include "../sdk/win32.h"

/* ---- the game's own functions ---- */
#include "pause_menu.h"
#include "list.h"
#include "scenaric.h"
#include "game_state.h"
#include "screen.h"
#include "progress.h"
#include "draw2d.h"
#include "../app/app_main.h"
#include "../platform/platform.h"

/* ---- globals ---- */
extern u32 *g_screenLayerBase0; /* u32 *, as its definition (src/app/app_main.cpp) declares it */
extern u32 *g_screenLayerBase;

/* Game_ReloadLevel expands it with the constant RSF_LIGHTING | RSF_CULL_CW, so every test is on the constant. */
inline void D3DApp::ResetStateFlags(u32 flags)
{ SDW_RSF_CLEAR_HOOK(flags)
    if (flags & RSF_ANTIALIAS)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE);
    if ((flags & RSF_BLEND_ALPHA) || (flags & RSF_BLEND_ADD))
        pD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
    if (flags & RSF_ALPHATEST)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
    if (flags & RSF_CLIPPLANE)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_CLIPPLANEENABLE, FALSE);
    if (flags & RSF_DITHER)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE);
    if (flags & RSF_LIGHTING)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
    if (flags & RSF_SPECULAR)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);
    if (flags & RSF_COLORVERTEX)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_COLORVERTEX, FALSE);
    if ((flags & RSF_CULL_CW) || (flags & RSF_CULL_CCW))
        pD3DDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    if (flags & RSF_ZTEST)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
    if (flags & RSF_ZWRITE_ON)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
    if (flags & RSF_ZWRITE_OFF)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
    if (flags & RSF_TEXTURED)
        pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    if (flags & RSF_FILTER_LINEAR) {
        pD3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);
        pD3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_POINT);
    }
    if (flags & RSF_FOG)
        pD3DDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
}

/* frees the loaded level: both pause menus' items, the level data, the level heap and its 512 KB block,
 * the 32 KB scratch. Nothing happens when no level is loaded. */
void Game_FreeLevel()
{
    if (g_pDav) {
        PauseMenu_ClearItems();
        PausedMenu_ClearItems();
        Load_FreeLevel();
        g_scenaricLevelHeap.Term();
        if (g_levelHeapStorage) {
            free(g_levelHeapStorage);
            g_levelHeapStorage = 0;
        }
        Scratch32k_Free();
    }
}

/* (re)loads the level g_pProgress points at: screen, game state, level heap and the two pause menus, the
 * two draw-layer bases, then Load_DAVnWAR (a failure ends the program), then the default render states. */
void Game_ReloadLevel()
{
    u8 loaded;
    g_screen.Init(0);
    g_screen.Clear(0);
    ((GameState *)&g_animDt)->Game_ResetState();
    Scenaric_AllocLevelHeap();
    PauseMenu_Build();
    PausedMenu_Build();
    g_screenLayerBase0 = g_screen.Layers4(0);
    g_screenLayerBase = g_screen.Layers60(0);
    printf("INFO: Loading scene %s\n", g_pProgress->scenePath);
    loaded = Load_DAVnWAR(g_pProgress->scenePath, &g_levelDav);
    printf("INFO: Scene load %s\n", loaded ? "succeeded" : "failed");
    if (!loaded) {
        MessageBoxA(0, "Unable to load datas : Exiting SheepD3D", "SheepD3D ERROR", MB_ICONHAND);
        Platform_RequestQuit(); /* the main loop reads SDL's events, not the thread's messages */
    }
    g_pPolyBin->SetDefaultStateFlags(RSF_DITHER | RSF_ZTEST);
    g_pD3DAppMain->ResetStateFlags(RSF_LIGHTING | RSF_CULL_CW);
}
