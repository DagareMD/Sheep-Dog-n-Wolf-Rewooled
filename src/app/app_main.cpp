#include "sdw_types.h"
#include "sdw_enums.h"
#include "../sdk/win32.h"
#include "../sdk/ddraw.h"
#include "../sdk/crt.h"
#include "../platform/platform.h"
#include "../platform/options.h"

/* ---- the game's classes: members this file defines or calls ---- */
#define SDW_MEMBERS_D3DApp                                                                          \
    D3DApp(HWND hWnd, HINSTANCE hInstance, u8 filterFlags); /* D3DApp_Construct */         \
    inline HRESULT ShowFrame(); \
    IDirectDrawSurface7 *GetBackBuffer()                                                            \
    {                                                                                               \
        return pBackBuffer;                                                                         \
    }
#define SDW_MEMBERS_SoundDevice SoundDevice();      /* SoundDevice_Construct */
#define SDW_MEMBERS_Frustrum Frustrum(D3DApp *app); /* Frustrum_Construct */
#define SDW_MEMBERS_Progress \
    void SetField80()        \
    {                        \
        field80 = 1;         \
    }
#define SDW_MEMBERS_Mat44 Mat44(); /* Mat44_Ctor */
#include "sdw_classes.h"

inline HRESULT D3DApp::ShowFrame()
{
    return g_renderDevice->Present();
}

/* ---- the game's functions ---- */
#include "../engine/time.h"
#include "../engine/scenaric_loop.h"
#include "../engine/input.h"
#include "../engine/cheat.h"
#include "../engine/fade.h"
#include "../engine/scenaric.h"
#include "../engine/progress.h"
#define SDW_INLINE_PROGRESS_SETLEVELDONE_S8 1
#include "../engine/progress_inlines.h"
#undef SDW_INLINE_PROGRESS_SETLEVELDONE_S8
#include "../engine/debug_draw.h"
#include "../engine/game_level.h"
#include "../engine/registry.h"
#include "../engine/draw2d.h"
#include "../engine/pause_menu.h"
#include "../engine/screen.h"
#include "../engine/game_state.h"
#include "../engine/stream_player.h"
#include "../objects/video_sequence.h"
u8 Video_PlaySequence(FmvList *list);
#include "app_main.h"

/* ---- globals ---- */
extern HWND g_hGameWindow;
extern u32 g_gameFlags;

u16 g_langMask = LANGMASK_ENGLISH; /* App_GetLanguageMask's bit; 1 (English) until Game_Main sets it */
s8 g_startLevelId = -1;            /* the scene App_InitGameSystems starts (-1: the title scene) */
u8 g_launcherInputMode = INPUTMODE_KEYBOARD; /* keyboard only, or a joystick is selected */
u32 g_maxFps = 60;                           /* the frame cap passed to Render_Present */

Dav g_levelDav = {0};                     /* the loaded level's .DAV */
u16 g_worldObjCount = 0;
u16 g_scnActiveBaseCount = 0;
u16 g_scnObjectCount = 0;
u16 g_scnActiveHigh = 0;
u16 g_cineObjectCount = 0;
u16 g_scnActiveCapacity = 0;
u32 *g_screenLayerBase0 = 0;
u32 *g_screenLayerBase = 0;
u16 *g_pWarCollMap = 0;                   /* the WAR collision grid (resource type 0x80) */
CollTri *g_pWarCollTris = 0;              /* the WAR collision triangles (type 0x81) */
void *g_animNameTable = 0;
WorldObj **g_worldObjs = 0;
ScnObject **g_scnActive = 0;
ScnObject **g_scnObjects = 0;
ScnObject **g_cineObjects = 0;

/* the static initialiser of the game camera. Camera's implicit constructor runs its two Mat44
 * members' (empty) constructors, at +0x40 and +0x80. */
Camera g_camera;

/* The game's folder: the executable's, made the working directory so that the game's paths (built in fixed-size
 * buffers) stay short; "." then. Falls back to the absolute path when the folder cannot be entered. */
void App_GetExeDir(char *out)
{
    if (Platform_EnterBaseDir()) {
        strcpy(out, ".");
        return;
    }
    Platform_GetBasePath(out, 512);
    {
        size_t len = strlen(out);
        while (len > 1 && (out[len - 1] == '/' || out[len - 1] == '\\'))
            out[--len] = 0;
    }
}

static void App_Error(const char *message)
{
    Platform_ShowMessage("SheepD3D", message, 1);
}

/* The game: its options (src/platform/options.h: the saved settings, then the command line), the data paths, the
 * window and the renderer, the sound, then the main loop until the window is closed. */
int Game_Main(int argc, char **argv)
{
    char path[SDW_PATH_MAX];
    char szTitle[256];
    char exeDir[SDW_PATH_MAX];

    Options_SetDefaults(&g_options);
    Options_LoadSaved(&g_options);
    Options_ParseArgs(&g_options, argc, argv);

    if (g_options.exeDir[0]) {
        strncpy(exeDir, g_options.exeDir, sizeof(exeDir) - 1);
        exeDir[sizeof(exeDir) - 1] = 0;   /* strncpy doesn't terminate when the source fills the buffer */

    } else {
        App_GetExeDir(exeDir);
    }


    strcpy(g_levelPathFmt, exeDir);
    strcpy(g_pathScene, exeDir);
    strcpy(g_pathWheelDir, exeDir);
    strcpy(g_introDir, exeDir);
    strcpy(g_pathFendDir, exeDir);
    strcpy(g_pathEnding, exeDir);
    strcpy(g_pathDemoDir, exeDir);
    strcpy(g_musicsPath, exeDir);
    strcpy(g_voiceDir, exeDir);
    strcpy(g_dirReference, exeDir);
    strcpy(g_dirBonusGame, exeDir);
    strcat(g_levelPathFmt, "/Levels/Lvl-%02d/Lvl-%02d");
    strcat(g_pathScene, "/Levels/Scene/Scene");
    strcat(g_pathWheelDir, "/Levels/Wheel/Wheel");
    strcat(g_introDir, "/Levels/Intro/Intro");
    strcat(g_pathFendDir, "/Levels/Fend/Fend");
    strcat(g_pathEnding, "/Levels/Ending/Ending");
    strcat(g_pathDemoDir, "/Levels/Demos");
    strcat(g_musicsPath, "/Musics/");
    strcat(g_voiceDir, "/Voices/");
    strcat(g_dirReference, "/References/");
    strcat(g_dirBonusGame, "/Bonus/");
    strncpy(szTitle, "Sheep, Dog'n Wolf: Rewooled", 0xff);

    /* the text the game's start-up errors are shown in */
    strcpy(path, g_dirReference);
    strcat(path, "GREET_txt.BSM");
    if (g_textCatalog.Load(path) == 0) {
        App_Error("The game's data was not found next to the executable (References/GREET_txt.BSM).");
        return 1;
    }
    g_langMask = App_GetLanguageMask();
    if (!Platform_Init(szTitle, IDI_APP_ICON)) {
        App_Error("SDL could not be initialised.");
        return 1;
    }
    /* the chosen controller may not be listed yet right after start-up: give SDL a moment to report it, before the
     * D3DApp below takes the list */
    if (g_options.controller >= 0)
        Platform_WaitForJoystick(g_options.controller, 2000);
    g_pD3DAppMain = new D3DApp(0, 0, 3); /* its window is set below */
    g_pSoundSystem = new SoundDevice;
    if (!Render_CreateBackend(Render_ConfiguredKind())) {
        App_Error("No renderer can run here: the game needs a Vulkan, Direct3D 12 or Metal driver.");
        return 1;
    }
    {
        int width, height;
        unsigned flags = g_renderBackend->WindowFlags();
        g_renderBackend->WindowSize(&width, &height);
        if (g_options.fullscreen)
            flags |= PLATFORM_WINDOW_FULLSCREEN;
        g_hGameWindow = (HWND)Platform_CreateWindow(szTitle, width, height, flags);
    }
    if (!g_hGameWindow) {
        App_Error("The game's window could not be created.");
        return 1;
    }
    g_pD3DAppMain->hWnd = g_hGameWindow;
    if (!g_renderBackend->PrepareDisplay(g_pD3DAppMain)) {
        App_Error("The selected renderer found no display device it can use.");
        return 1;
    }
    /* the sound device (SDL3's default output) and the controller: the keyboard, or one SDL sees */
    g_pSoundSystem->SetDeviceIndex(0);
    if (g_options.controller >= 0 && g_options.controller < g_pD3DAppMain->joystickCount) {
        g_pD3DAppMain->joystickIndex = (u8)g_options.controller;
        g_launcherInputMode = INPUTMODE_JOYSTICK;
    } else {
        g_launcherInputMode = INPUTMODE_KEYBOARD;
    }
    g_startLevelId = (s8)g_options.startLevel;

    long createResult;
    if (!Render_CreateDevice(g_pD3DAppMain, &createResult)) {
        Platform_ShowMessage(g_textCatalog.LoadString(1, g_langMask, 0x10), g_textCatalog.LoadString(8, g_langMask, 0x40),
                             1);
        return 1;
    }
    if (g_pSoundSystem->Init(g_hGameWindow, 22050, 16) < 0)
        Platform_ShowMessage(g_textCatalog.LoadString(1, g_langMask, 0x10), g_textCatalog.LoadString(8, g_langMask, 0x80),
                             1);
    App_InitGameSystems();
    Main_Loop(g_pD3DAppMain);
    App_Shutdown();
    return 0;
}

/* the message pump and one game frame per iteration, until WM_QUIT. While another window is active the
 * screen DC is saved; when the game gets focus back it is restored and the menu noise texture is dropped (it is rebuilt
 * on demand). The frame itself (Render_BeginFrame .. Render_EndFrame) runs only while the D3D device is ready; the
 * present / frame limiter (Render_Present with g_maxFps) runs every iteration. bInactive is written and never read. */
/* SDL's events (src/platform/platform.h) instead of the thread's messages. Losing and getting back
 * the focus replaces the screen DC save / restore, which did nothing else; the menu noise texture is still dropped on
 * the way back. The menu loop's input release (App_WndProc) has no SDL event: DirectInput's foreground devices let go
 * of the input by themselves when the window loses the focus. */
void Main_Loop(D3DApp *app)
{
    u8 quitRequested = 0;
    u8 wasInactive = 0;
    (void)app;
    while (quitRequested != 1) {
        if (!g_pSoundSystem->PollStreamEvents() && (Platform_PollEvents() & PLATFORM_EVENT_QUIT)) {
            quitRequested = 1;
            continue;
        }
        if (!Platform_IsActive()) {
            wasInactive = 1;
        } else if (wasInactive) {
            wasInactive = 0;
            if (g_pMenuNoiseTexture) {
                delete g_pMenuNoiseTexture;
                g_pMenuNoiseTexture = NULL;
            }
        }
        Time_Update();
        if (g_pPolyBin && g_pPolyBin->renderer && g_pPolyBin->renderer->deviceReady) {
            g_pPolyBin->Render_BeginFrame();
            if ((g_gameFlags & GF_LEVEL_LOADED_A) && (g_gameFlags & GF_LEVEL_LOADED_B))
                Game_Frame_2();
            Input_Poll();
            Cheat_Poll();
            if (Cheat_IsLevelSkipRequested())
                Fade_StartLevelExit(0x1000);
            if (g_levelExitFlags)
                Level_ExitUpdate();
            g_pPolyBin->Render_EndFrame(1);
        }
        g_pPolyBin->Render_Present(g_maxFps);
    }
}

/* everything that comes up once the D3D device exists: frustum (60 degree fov, near 8, far 20000), fog, the
 * screen, the input devices and their configuration, the game state, the intro videos, then the first scene. */
s32 App_InitGameSystems()
{
    g_maxImmediateTriangles = 1000;
    g_matUnk6d5428.SetScale(0.125f, 0.125f, 0.125f);
    g_pViewFrustum = new Frustrum(g_pD3DAppMain);
    g_pViewFrustum->SetProjection(8.0f, 20000.0f, 1.0471976f, 12000.0f);
    g_pViewFrustum->SetFog(1, 0, FOG_LINEAR, 2000.0f);
    ShowCursor(1);
    Platform_ShowWindow();
    g_screen.Init(0);
    g_screen.Clear(0);
    g_pD3DAppMain->ShowFrame();
    g_inputMgr.CreateDevices(INPUTDEV_MASK_KEYBOARD | INPUTDEV_MASK_JOYSTICK | INPUTDEV_MASK_MOUSE);
    g_inputMgr.SelectDevice(g_launcherInputMode);
    g_inputMgr.LoadConfig(g_dirReference);
    g_pProgress->SetField80();
    Progress_ResetGlobal();
    // Starting directly in a level gives it the preceding story progress.
    if (g_startLevelId >= SCENE_LVL_00 && g_startLevelId <= SCENE_LVL_17) {
        for (s8 level = SCENE_LVL_00; level < g_startLevelId; ++level) {
            if (level == 5 || level == 10) // B1 (Lvl-05) and B2 (Lvl-10) stay incomplete.
                continue;
            g_pProgress->SetLevelDone(level);
            --g_pProgress->sheepToCatch;
            g_pProgress->slotIcon = level;
        }
    }
    ((GameState *)&g_animDt)->Game_ResetState();
    g_pStreamPlayer = NULL;
    g_fmvListIntro.count = 3;
    strcat(g_fmvListIntro.clips[0], "Intro0.BVS");
    strcat(g_fmvListIntro.clips[1], "Intro1.BVS");
    strcat(g_fmvListIntro.clips[2], "Intro2.BVS");
    g_fmvListCredits.count = 1;
    strcpy(g_fmvListCredits.clips[0], "Credits.BVS");
    Video_PlaySequence(&g_fmvListIntro);
    g_pProgress->GotoScene(g_startLevelId);
    Draw_CreateScratchVertexBuffers();
    return 0;
}

/* the exit path: unload the level, then delete the sound device, the frustum, the D3D app and the menu noise
 * texture. */
void App_Shutdown()
{
    Game_FreeLevel();
    delete g_pSoundSystem;
    delete g_pViewFrustum;
    delete g_pD3DAppMain;
    if (g_pMenuNoiseTexture) {
        delete g_pMenuNoiseTexture;
        g_pMenuNoiseTexture = NULL;
    }
    Platform_Shutdown(); /* the window, after the renderer that drew in it */
}

/* the language bit of the user's preferred language (EN 1, FR 2, DE 4, NL 8, ES 0x10, IT 0x20, PT 0x40); anything else
 * is English. */
u16 App_GetLanguageMask()
{
    switch (Platform_LanguageId() & 0xff) {
        case LANG_FRENCH:
            return LANGMASK_FRENCH;
        case LANG_GERMAN:
            return LANGMASK_GERMAN;
        case LANG_DUTCH:
            return LANGMASK_DUTCH;
        case LANG_ITALIAN:
            return LANGMASK_ITALIAN;
        case LANG_SPANISH:
            return LANGMASK_SPANISH;
        case LANG_PORTUGUESE:
            return LANGMASK_PORTUGUESE;
        default:
            return LANGMASK_ENGLISH;
    }
}
