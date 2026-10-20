/*
 *   -, between g_settingNames and g_confirmPending, is four bytes no instruction refers to (defined here as
 *   g_pauseBss_6def98, a name by address).
 *
 * The handlers are flat void __cdecl(u8 msg, MenuPage *self) (MenuMsg) with no vtable.
 *
 * InputBindingIdx is the {u8 devIdx; u16 code} binding the input manager fills.
 */
#include "sdw_types.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
#include "../sdk/dinput.h"
#include "../sdk/win32.h"
#include "../sdk/ddraw.h"
#include "../sdk/crt.h"
#define SDW_MEMBERS_Progress inline void ToggleAutoSave();

#define SDW_MEMBERS_Texture                                                              \
    Texture(D3DApp *app, u32 width, u32 height, u32 format, s32 *result); \
    long Surface_LockForWrite(DDSURFACEDESC2 *desc);
#include "sdw_classes.h"
#define SDW_INLINE_PAD_SETACTUATOR_INT 1
#include "input_inlines.h"
#undef SDW_INLINE_PAD_SETACTUATOR_INT
#include "sdw_empty_call.h"
#include "sdw_global_views.h"

typedef void (*MenuHandler)(u8 msg, MenuPage *self);

/* ---- globals of other objects (named in the tables) ---- */
#include "progress.h"
#include "langpack.h"
#include "../objects/rewooled_menu_strings.h"
#include "game_state.h"
#include "scenaric.h"
#include "stream_player.h"
#include "draw2d.h"
#include "text.h"
#include "interface.h"
#include "input.h"
#include "screen.h"
#include "../objects/menu.h"
#include "../objects/camera.h"
#include "prompt.h"
#include "sound_mgr.h"
#include "sfx_volume.h"
#include "fade.h"
#include "approach.h"
#include "mesh.h"
#include "../platform/save_store.h"
#include "../platform/options.h"

/* The sample counts the renderer can do, as bits: 1 = 2x, 2 = 4x, 4 = 8x (sdl3_render_device.cpp). The anti-aliasing row
 * shows a choice that is not in it greyed out, as unsupported. */
extern u8 g_msaaSupportMask;

static int PauseMenu_MsaaSupported(int count) /* 0, off, always is */
{
    return count == 0 || (count == 2 && (g_msaaSupportMask & 1)) || (count == 4 && (g_msaaSupportMask & 2)) ||
           (count == 8 && (g_msaaSupportMask & 4));
}

/* wanted if it is supported, else the largest supported count below it (0: off) */
static int PauseMenu_SupportedMsaa(int wanted)
{
    static const int counts[3] = {8, 4, 2};
    int i;
    for (i = 0; i < 3; i++)
        if (counts[i] <= wanted && PauseMenu_MsaaSupported(counts[i]))
            return counts[i];
    return 0;
}

/* saves the screen percent and the anti-aliasing; an unsupported anti-aliasing choice is saved as the supported one it runs as */
static void PauseMenu_SaveRenderSettings()
{
    Options_SaveRenderSettings(g_options.renderScale, PauseMenu_SupportedMsaa(g_options.msaa));
}
extern u32 g_gameFlags;
extern u32 *g_screenLayerBase; /* 2D draw layers: layer n is g_screenLayerBase + n */

extern TextPort g_textPort;
#define g_textCursorY (*(u16 *)&g_textPort.cursorY)

extern TextPort g_textPort;
#define g_textWinY (g_textPort.winY)
extern const float g_viewDistFar;    /* 12000.0, the view distance at setting 0 */
extern const float g_viewDistNear;   /* 4000.0, the view distance at setting 255 */

extern TextPort g_textPort;
#define g_textCursorX (g_textPort.cursorX)

extern TextPort g_textPort;
#define g_textClipOffX (g_textPort.clipOffX)
extern s32 g_dtRawMs;
extern s32 g_dt;
extern s32 g_dtMs;

char *g_onOffNames[2] = {0};          /* UI strings 0x10, 0xf */
char *g_soundModeNames[3] = {0};      /* BSM group 0x10, 0x1000 / 0x2000 / 0x4000 */
u8 g_pauseCfg[7] = {0};               /* "CfgGame" registry image written by PauseMenu_Exit */
MenuPage g_pausedMenuItems[50] = {0}; /* node storage of g_pausedMenu */
char *g_uiString1B = 0;
UiCursor g_menuCursors[6] = {0};      /* slider cursors: 0..2 volumes, 3 fog, 4 camera speed, 5 camera acceleration */
s16 g_pauseBoxRect[4] = {0};          /* {x, y, w, h} of the pause box on the 512 x 240 screen */
s16 g_pauseTextRect[4] = {0};         /* the title / footer text window PauseMenu_Update sets */
u8 g_progressPauseCopy[0x2a] = {
    0}; /* the saved progress record, snapshotted on open; 0x2c bytes with the next two (see the header) */
u8 g_pauseViewDistance = 0;          /* view distance to restore when the display page is left */
static u8 g_pauseMeshOutlines = 1;   /* accepted outline setting; Done keeps the preview */
/* Rewooled Settings' languages while the page is open (langpack.h): saved by Done, dropped otherwise */
static s32 g_pauseTextLanguage = 0;
static s32 g_pauseVoiceLanguage = -1; /* -1: the text language's own voices */
static u8 g_pauseTextureFilterMode = 1; /* accepted texture filter (0 nearest, 1 bilinear, 2 sharp bilinear); Done keeps the preview */
s16 g_pauseResumeIndex = 0;          /* node index of "resume", where the cursor returns */
u8 g_menuFooterStyle = 0;            /* which footer the menu draws */
u8 g_menuFooterStylePrev = 0;        /* last frame's g_menuFooterStyle; nothing reads it */
u8 g_pauseRebinding = 0;             /* 1 while a key-binding item captures input */
u8 g_pauseFlag89 = 0;
u8 g_pauseSlidersShown = 0;          /* sliders are drawn on unselected rows too */
u8 g_pauseFlag8b = 0;
u8 g_pauseExitRequested = 0;         /* "exit" confirmed: leave the level when the menu closes */
u8 g_pauseRestartRequested = 0;      /* "restart level" confirmed */
u8 g_pauseFlag8e = 0;
u8 g_pauseFlag8f = 0;
char g_msgBoxText[0x80] = {0};       /* the message box's text (strcpy, unbounded) */
s16 g_msgBoxRect[4] = {0};           /* the message box's {x, y, w, h} */
u8 g_msgBoxAlign = 0;
UiFrame g_msgBoxFrame = {0};         /* the message box's four frame strips */
s32 g_rebindCooldown = 0;            /* Menu_RebindControl waits while > 0; the message box's ms */
u8 g_quitCfg[7] = {0};               /* the same image written by PauseMenu_QuitConfirm */
u16 g_uiLangMask = 0;                /* language mask of the BSM strings */
MenuPage g_pauseMenuItems[50] = {0}; /* node storage of g_pauseMenu */
Texture *g_pMenuNoiseTexture = 0;
char *g_settingNames[3] = {0};       /* UI strings 0xc, 0xd, 0xe */
u32 g_pauseBss_6def98 = 0;
s32 g_confirmPending = 0;
s32 g_quitPending = 0;

Menu g_pauseMenu = {g_pauseMenuItems, 0, 50, 2, 0, 0, 0, 0};
s32 g_confirmChoice = 1;                                     /* yes/no box of restart / exit: 1 = no */
s32 g_quitChoice = 1;                                        /* yes/no box of quit */
u8 g_bindUpShown = 1;       /* the binding items' "show the current binding" flags */
u8 g_bindRightShown = 1;
u8 g_bindDownShown = 1;
u8 g_bindLeftShown = 1;
u8 g_bindActionShown = 1;
u8 g_bindJumpShown = 1;
u8 g_bindWolfEyeShown = 1;
u8 g_bindRunShown = 1;
u8 g_bindQuickInvShown = 1;
u8 g_bindSneakShown = 1;
u8 g_bindViewMapShown = 1;
u8 g_bindCamLeftShown = 1;
u8 g_bindCamRightShown = 1;
Menu g_pausedMenu = {g_pausedMenuItems, 0, 50, 2, 0, 0, 0, 0};

/* ---- functions ---- */
void Menu_Printf(MenuPage *item, u8 align, const char *fmt, ...);
void Menu_PrintfSelected(int blink, u8 align, const char *fmt, ...);
s16 Menu_AddItems(MenuPage *page, MenuHandler handler, ...);
void Menu_DrawBindingText(u8 actionId, MenuPage *item);
void Menu_BuildValidateCancelFooter();
void Menu_BuildValidateBackFooter();
void Menu_BuildQuitFooter();
void Menu_BuildPauseFooter();
void Menu_BuildNavFooter();
void Menu_BuildAutoSaveFooter();
void Menu_BuildControlsFooter();
void Menu_ShowMessageBox(const char *text, u8 align, s32 durationMs);
long Menu_CreateNoiseTexture();
void PauseMenu_OnOpen_stub();
LONG Reg_CreateSubKey(HKEY *out, const char *name);
void Reg_CloseKey(HKEY *key);
u8 Reg_WriteBinary(HKEY key, const char *name, const void *data, DWORD size);

static const char *PauseMenu_RewooledText(u16 index);
void PauseMenu_ItemAutoSave(u8 msg, MenuPage *self);
void PauseMenu_PageOptions(u8 msg, MenuPage *self);
void PauseMenu_PageSoundOptions(u8 msg, MenuPage *self);
void PauseMenu_ItemResume(u8 msg, MenuPage *self);
void PauseMenu_ItemExit(u8 msg, MenuPage *self);
void PauseMenu_ItemRestartLevel(u8 msg, MenuPage *self);
void PauseMenu_ItemSpeakerMode(u8 msg, MenuPage *self);
void PauseMenu_ItemSfxVolume(u8 msg, MenuPage *item);
void PauseMenu_ItemVoiceVolume(u8 msg, MenuPage *item);
void PauseMenu_ItemMusicVolume(u8 msg, MenuPage *item);
void PauseMenu_ItemSubTitles(u8 msg, MenuPage *item);
void PauseMenu_ItemSoundDone(u8 msg, MenuPage *item);
void PauseMenu_PageControllerSetting(u8 msg, MenuPage *item);
void PauseMenu_PageDisplaySettings(u8 msg, MenuPage *item);
static void PauseMenu_PageRewooledSettings(u8 msg, MenuPage *item);
static void PauseMenu_ItemRewooledDone(u8 msg, MenuPage *item);
void PauseMenu_PageEditConfig(u8 msg, MenuPage *item);
void PauseMenu_ItemQuit(u8 msg, MenuPage *item);
void PauseMenu_QuitConfirm(u8 msg, MenuPage *item);
void PauseMenu_ItemSoundsEnabled(u8 msg, MenuPage *item);
void PauseMenu_ItemFog(u8 msg, MenuPage *item);
static void PauseMenu_ItemMeshOutlines(u8 msg, MenuPage *item);
static void PauseMenu_ItemTextLanguage(u8 msg, MenuPage *item);
static void PauseMenu_ItemVoiceLanguage(u8 msg, MenuPage *item);
static void PauseMenu_ItemTextureFiltering(u8 msg, MenuPage *item);
static void PauseMenu_ItemScreenPercent(u8 msg, MenuPage *item);
static void PauseMenu_ItemAntiAliasing(u8 msg, MenuPage *item);
static void PauseMenu_ItemFpsLimit(u8 msg, MenuPage *item);
static void PauseMenu_ItemVSync(u8 msg, MenuPage *item);
void PauseMenu_ItemDisplayDone(u8 msg, MenuPage *item);
void PauseMenu_ItemControlDevice(u8 msg, MenuPage *item);
void PauseMenu_ItemCameraSpeed(u8 msg, MenuPage *item);
void PauseMenu_ItemCameraAcceleration(u8 msg, MenuPage *item);
void PauseMenu_ItemInvertCameraJoystick(u8 msg, MenuPage *item);
void PauseMenu_ItemBindUp(u8 msg, MenuPage *item);
void PauseMenu_ItemBindRight(u8 msg, MenuPage *item);
void PauseMenu_ItemBindDown(u8 msg, MenuPage *item);
void PauseMenu_ItemBindLeft(u8 msg, MenuPage *item);
void PauseMenu_ItemBindAction(u8 msg, MenuPage *item);
void PauseMenu_ItemBindJump(u8 msg, MenuPage *item);
void PauseMenu_ItemBindWolfEyeView(u8 msg, MenuPage *item);
void PauseMenu_ItemBindRun(u8 msg, MenuPage *item);
void PauseMenu_ItemBindQuickInventory(u8 msg, MenuPage *item);
void PauseMenu_ItemBindWalkStealthily(u8 msg, MenuPage *item);
void PauseMenu_ItemBindViewMap(u8 msg, MenuPage *item);
void PauseMenu_ItemBindCamLeft(u8 msg, MenuPage *item);
void PauseMenu_ItemBindCamRight(u8 msg, MenuPage *item);
void PauseMenu_ItemSaveConfig(u8 msg, MenuPage *item);
void Menu_RebindControl(u8 actionId);

/* ---- the menu tail's declarations not already above ---- */
void PauseMenu_Exit();
void Text_PrintFmtStyled(u8 style, s32 blink, const char *fmt, va_list ap);
u16 Text_CountWrappedLines(const char *s);
void Draw2D_TexRect(float z, float x0, float y0, float x1, float y1, s32 texIndex, float uTL, float vTL, u32 cTL,
                    float uBL, float vBL, u32 cBL, float uTR, float vTR, u32 cTR, float uBR, float vBR,
                    u32 cBR);
void Draw2D_TexRect_Immediate(float z, float x0, float y0, float x1, float y1, u32 renderFlags, Texture *texture,
                              float uTL, float vTL, u32 cTL, float uBL, float vBL, u32 cBL, float uTR, float vTR,
                              u32 cTR, float uBR, float vBR, u32 cBR);

void Menu_Printf(MenuPage *item, u8 align, const char *fmt, ...);
void Menu_DrawMessageBox();
void Menu_BuildValidateBackFooter();
void Menu_BuildBackFooter();
void Menu_BuildNavFooter();
void PauseMenu_OnOpen_stub();
void PauseMenu_DrawNoiseOverlay();
void PausedMenu_ItemPaused(u8 msg, MenuPage *item);

#define GAME_STATE ((GameState *)&g_animDt)
#define RECORD_OPTIONS(rec) ((ProgressOptionBits *)((rec) + 0x14))

#define SDW_INLINE_FREE_GAME_CLEARFLAGS_U32 1
#include "game_state_inlines.h"
#include "../platform/platform.h"
#undef SDW_INLINE_FREE_GAME_CLEARFLAGS_U32

/* The constants reach the code unfolded. */
inline void PauseMenu_SetBox(s32 w, s32 h)
{
    g_pauseBoxRect[0] = (0x200 - w) / 2;
    g_pauseBoxRect[1] = (0xf0 - h) / 2;
    g_pauseBoxRect[2] = w;
    g_pauseBoxRect[3] = h;
}

inline u8 *Progress_Record()
{
    return g_pProgress->timeKeeperBits; /* the saved record; its option byte is at +0x14 */
}
inline ProgressOptionBits *Progress_Options()
{
    return &g_pProgress->optionBits;
}

/* a Yes/No setting stored as a one-byte REG_BINARY value in the "CfgGame" key; missing or invalid means Yes */
static u8 PauseMenu_LoadBoolSetting(const char *name, u8 maxValue)
{
    if (!Save_KeyExists("CfgGame"))
        return 1;
    SaveKey *key = Save_OpenKey("CfgGame");
    if (!key)
        return 1;
    u8 enabled = 1;
    unsigned type = 0, size = sizeof(enabled);
    if (Save_QueryValue(key, name, &type, &enabled, &size) != SAVE_OK ||
        type != 3 || size != sizeof(enabled) || enabled > maxValue)
        enabled = 1;
    Save_CloseKey(key);
    return enabled;
}

static u8 PauseMenu_LoadOutlineSetting()
{
    return PauseMenu_LoadBoolSetting("CelShadingOutlines", 1);
}

static u8 PauseMenu_LoadTextureFilteringSetting()
{
    return PauseMenu_LoadBoolSetting("TextureFiltering", 2); /* 0 and 1 are what the old Yes/No row saved */
}

/* per-level UI set-up, called once by Load_DAVnWAR: the pause box, the slider cursors' skins, the UI
 * strings the option rows print, and the BSM language mask for the player's language. */
void Menus_LoadLevelUi()
{
    u16 i;
    u8 playerLang;

    g_meshOutlinesEnabled = g_pauseMeshOutlines = PauseMenu_LoadOutlineSetting();
    g_textureFilterMode = g_pauseTextureFilterMode = PauseMenu_LoadTextureFilteringSetting();
    g_pauseFlag89 = 1;
    g_pauseSlidersShown = 0;
    PauseMenu_SetBox(0x1c0, 0xa0);
    g_pauseBoxRect[3] -= g_pCurFont->lineHeight / 2;
    for (i = 0; i < sizeof(g_menuCursors) / sizeof(g_menuCursors[0]); i++)
        g_menuCursors[i].UiFrame_LoadSkin(0, 0);
    g_settingNames[0] = Text_GetUiString(UISTR_MONO);
    g_settingNames[1] = Text_GetUiString(UISTR_STEREO);
    g_settingNames[2] = Text_GetUiString(UISTR_REVERSE_STEREO);
    g_onOffNames[0] = Text_GetUiString(UISTR_NO);
    g_onOffNames[1] = Text_GetUiString(UISTR_YES);
    g_uiString1B = Text_GetUiString(UISTR_CUSTOMIZE);
    Menu_CreateNoiseTexture();
    playerLang = g_pProgress->language;
    switch (playerLang) {
        case GAME_LANG_FRENCH:
            g_uiLangMask = LANGMASK_FRENCH;
            break;
        case GAME_LANG_ENGLISH:
            g_uiLangMask = LANGMASK_ENGLISH;
            break;
        case GAME_LANG_SPANISH:
            g_uiLangMask = LANGMASK_SPANISH;
            break;
        case GAME_LANG_ITALIAN:
            g_uiLangMask = LANGMASK_ITALIAN;
            break;
        case GAME_LANG_GERMAN:
            g_uiLangMask = LANGMASK_GERMAN;
            break;
        case GAME_LANG_DUTCH:
            g_uiLangMask = LANGMASK_DUTCH;
            break;
        case GAME_LANG_BRAZILIAN:
            g_uiLangMask = LANGMASK_PORTUGUESE;
            break;
        default:
            g_uiLangMask = LANGMASK_ENGLISH;
            break;
    }
    g_soundModeNames[0] = g_textCatalog.LoadString(0x10, g_uiLangMask, 0x1000);
    g_soundModeNames[1] = g_textCatalog.LoadString(0x10, g_uiLangMask, 0x2000);
    g_soundModeNames[2] = g_textCatalog.LoadString(0x10, g_uiLangMask, 0x4000);
}

/* back to the root page with the cursor on "resume". */
void PauseMenu_ReturnToRoot()
{
    Menu_SetCapture(MENU_CAPTURE_NONE, 0);
    Menu_ResetToRoot(&g_pauseMenu, g_pauseResumeIndex);
    g_pauseSlidersShown = 1;
    g_pauseFlag8f = 1;
    g_pauseFlag8e = 0;
}

/* opens the pause menu: snapshots the saved record, pauses the sounds and the game (g_gameFlags 0x40, clears
 * 0x4001), makes the pause menu current at its root. */
void PauseMenu_Open()
{
    g_pProgress->CopyRecord(g_progressPauseCopy);
    Sound_PauseAll();
    g_pad.SetActuator(0);
    g_pProgress->controls.actuatorEnable = 0;
    g_gameFlags |= GF_PAUSED;
    Menu_SetCurrent(&g_pauseMenu);
    Game_ClearFlags(GF_BIT0 | GF_UPDATE_OBJECTS);
    Menu_ResetToRoot(&g_pauseMenu, -1);
    PauseMenu_OnOpen_stub();
    g_menuFooterStyle = MENU_FOOTER_NONE;
    g_pauseFlag89 = 1;
    g_pauseSlidersShown = 1;
    g_pauseFlag8f = 1;
    g_pauseRebinding = 0;
    g_pauseFlag8b = 0;
    g_pauseExitRequested = 0;
    g_pauseRestartRequested = 0;
    g_pauseFlag8e = 0;
    g_inputMgr.SetActiveFlags(0);
    g_inputMgr.ResetEdges();
}

/* closes the pause menu: preserves the music position, resumes the sounds and the game (clears 0x60, sets
 * 0xc000 unconditionally), starts the level exit / restart fade that was confirmed, restores the saved record and
 * writes the options to the registry key "CfgGame". */
void PauseMenu_Exit()
{
    HKEY key;
    DWORD len;

    Menu_Close();
    g_meshOutlinesEnabled = g_pauseMeshOutlines;
    g_textureFilterMode = g_pauseTextureFilterMode;
    // Keep the existing music stream unless the sound mode disables it or it needs starting.
    if (g_pProgress->optionBits.soundMode != SOUNDMODE_ALL)
        g_pStreamPlayer->Halt();
    else if (g_pStreamPlayer->state == STREAM_ST_IDLE)
        g_pStreamPlayer->RestartLevelMusic();
    g_pStreamPlayer->ApplyVolume();
    Sound_ResumeAll();
    Game_ClearFlags(GF_PAUSE_TOGGLE | GF_PAUSED);
    g_gameFlags |= GF_UPDATE_OBJECTS | GF_RENDER_WORLD;
    if (g_pauseExitRequested) {
        g_levelExitFlags |= LEVEL_EXIT_TO_MENU;
        Fade_StartLevelExit(0x1000);
        g_pauseExitRequested = 0;
    }
    if (g_pauseRestartRequested) {
        g_levelExitFlags |= LEVEL_EXIT_RESTART;
        Fade_StartLevelExit(0x1000);
        g_pauseRestartRequested = 0;
    }
    g_pProgress->LoadRecord(g_progressPauseCopy);
    g_pad.SetActuator(g_progressPauseCopy[0x17]);
    len = 7;
    g_pauseCfg[0] = g_pProgress->streamVolumeA;
    g_pauseCfg[1] = g_pProgress->streamVolumeB;
    g_pauseCfg[2] = g_pProgress->sfxVolume;
    g_pauseCfg[3] = g_pProgress->optionBits.soundMode;
    g_pauseCfg[4] = g_pProgress->optionBits.setting;
    g_pauseCfg[5] = g_pProgress->optionBits.gate;
    g_pauseCfg[6] = g_pProgress->viewDistanceSetting;
    if (Reg_CreateSubKey(&key, "CfgGame") == 0) {
        if (Reg_WriteBinary(key, 0, g_pauseCfg, len) == 1)
            Reg_CloseKey(&key);
    }
    g_inputMgr.SetActiveFlags(1);
    g_inputMgr.ResetEdges();
}

u32 Game_IsPaused()
{
    return g_gameFlags & GF_PAUSED;
}

/* empties the pause menu's node array. */
void PauseMenu_ClearItems()
{
    memset(g_pauseMenu.items, 0, g_pauseMenu.capacity * sizeof(MenuPage));
    g_pauseMenu.count = 0;
}

inline void Progress::ToggleAutoSave()
{
    runtimeBits.autoSaveOn ^= 1;
}

inline s32 Progress_AutoSaveAvailable()
{
    return g_pProgress->runtimeBits.saveSlotValid;
}

/* "auto save" on/off (Progress.runtimeFlags bit 5), greyed out unless bit 4 is set. */
void PauseMenu_ItemAutoSave(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_PREDRAW:
            self->disabled = Progress_AutoSaveAvailable() != 1;
            break;
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildAutoSaveFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(self, TEXTALIGN_CENTER, "%s %s", Text_GetUiString(UISTR_AUTOMATIC_SAVE),
                        g_onOffNames[g_pProgress->runtimeBits.autoSaveOn]);
            break;
        case MENU_MSG_VALUE_CHANGED:
            Ui_PlayMoveSound();
            g_pProgress->ToggleAutoSave();
            break;
    }
}

/* the "options" page. */
void PauseMenu_PageOptions(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildPauseFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(self, TEXTALIGN_CENTER, Text_GetUiString(UISTR_OPTIONS));
            break;
    }
}

/* the "sound options" page; while it has the cursor the volumes are those of the snapshot. */
void PauseMenu_PageSoundOptions(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildValidateBackFooter();
            *(u32 *)&g_pProgress->streamVolumeA = *(u32 *)&g_progressPauseCopy[0x11];
            Sound_SetSfxVolume(g_pProgress->sfxVolume);
            g_pStreamPlayer->ApplyVolume();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(self, TEXTALIGN_CENTER, Text_GetUiString(UISTR_SOUND_OPTIONS));
            break;
    }
}

/* "resume": clears GF_PAUSE_TOGGLE (0x20), which closes the menu. */
void PauseMenu_ItemResume(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildPauseFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(self, TEXTALIGN_CENTER, Text_GetUiString(UISTR_RESUME));
            break;
        case MENU_MSG_CONFIRM:
            GAME_STATE->Game_SetFlags(GF_PAUSE_TOGGLE, 0);
            break;
    }
}

/* the yes/no box shared by "exit" and "restart level": *outConfirmed is set when "yes" is validated. */
void PauseMenu_HandleConfirm(u8 msg, u8 *outConfirmed)
{
    s32 choice;

    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildPauseFooter();
            break;
        case MENU_MSG_CONFIRM:
            if (g_confirmPending == 0) {
                g_pauseSlidersShown = 1;
                g_pauseFlag8b = 0;
                Menu_SetCapture(MENU_CAPTURE_HIDDEN, 0);
            } else {
                g_pauseSlidersShown = 1;
                g_pauseFlag8b = 0;
                GAME_STATE->Game_SetFlags(GF_PAUSED, g_confirmChoice);
                g_confirmChoice = 1;
            }
            g_confirmPending = 0;
            break;
        case MENU_MSG_CAPTURE_TICK:
            Text_CenterVertically(4);
            Text_Printf(TEXTALIGN_CENTER, Text_GetUiString(UISTR_ARE_YOU_SURE));
            Text_NewLine(2);
            Menu_PrintfSelected(g_confirmChoice, TEXTALIGN_CENTER, Text_GetUiString(UISTR_NO));
            Text_NewLine(1);
            Menu_PrintfSelected(g_confirmChoice == 0, TEXTALIGN_CENTER, Text_GetUiString(UISTR_YES));
            if (Pad_MenuPressed((u16)~PAD_CROSS)) {
                choice = g_confirmChoice; /* the release message resets it */
                g_confirmPending = 0;
                Menu_SetCapture(MENU_CAPTURE_NONE, 1);
                g_confirmChoice = choice;
                Ui_PlayConfirmSound();
                Menu_SetDirty(1);
                *outConfirmed = g_confirmChoice == 0;
                GAME_STATE->Game_SetFlags(GF_PAUSE_TOGGLE, g_confirmChoice);
            } else if (Pad_MenuPressed((u16)~PAD_TRIANGLE)) {
                g_confirmPending = 0;
                Menu_SetCapture(MENU_CAPTURE_NONE, 1);
                Ui_PlayCancelSound();
                Menu_SetDirty(1);
            } else if (Pad_MenuRepeat((u16)~PAD_UP) || Pad_MenuRepeat((u16)~PAD_DOWN)) {
                Ui_PlayMoveSound();
                g_confirmChoice ^= 1;
            }
            break;
        case MENU_MSG_CAPTURE_RELEASE:
            g_pauseSlidersShown = 1;
            g_confirmChoice = 1;
            break;
    }
}

/* "exit". */
void PauseMenu_ItemExit(u8 msg, MenuPage *self)
{
    PauseMenu_HandleConfirm(msg, &g_pauseExitRequested);
    if (msg == MENU_MSG_DRAW)
        Menu_Printf(self, TEXTALIGN_CENTER, Text_GetUiString(UISTR_EXIT));
}

/* "restart level". */
void PauseMenu_ItemRestartLevel(u8 msg, MenuPage *self)
{
    PauseMenu_HandleConfirm(msg, &g_pauseRestartRequested);
    if (msg == MENU_MSG_DRAW)
        Menu_Printf(self, TEXTALIGN_CENTER, Text_GetUiString(UISTR_RESTART_LEVEL));
}

/* *value += delta, saturating at 0 and 0xff. */
void PauseMenu_StepSetting(u8 *value, s8 delta)
{
    switch (delta >= 0 ? 1 : -1) {
        case -1:
            if (*value + delta > 0)
                *value = *value + delta;
            else
                *value = 0;
            break;
        case 1:
            if (*value + delta < 0xff)
                *value = *value + delta;
            else
                *value = 0xff;
            break;
    }
}

/* the speaker-mode row (Progress.optionFlags bits 0-1, cycled % 3). */
void PauseMenu_ItemSpeakerMode(u8 msg, MenuPage *self)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(self, TEXTALIGN_LEFT, Text_GetUiString(UISTR_MODE));
            Menu_Printf(self, TEXTALIGN_RIGHT, g_settingNames[g_pProgress->optionBits.setting]);
            break;
        case MENU_MSG_LEFT:
            Progress_Options()->setting = (RECORD_OPTIONS(Progress_Record())->setting + 2) % 3;
            break;
        case MENU_MSG_RIGHT:
            Progress_Options()->setting = (RECORD_OPTIONS(Progress_Record())->setting + 1) % 3;
            break;
        case MENU_MSG_VALUE_CHANGED:
            Stub_Ret(g_pProgress->optionBits.setting);
            Ui_PlayMoveSound();
            break;
    }
}

/* the body of the three volume rows: label, slider cursor cursorIdx, LEFT / RIGHT step by 8. */
void PauseMenu_VolumeItem(u8 msg, MenuPage *item, u8 cursorIdx, u8 *pValue, u8 uiStringId)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuCursors[cursorIdx].Animate();
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, Text_GetUiString(uiStringId));
            if (item->selected || g_pauseSlidersShown) {
                g_menuCursors[cursorIdx].SetPos(0x15c, (s16)g_textCursorY + g_textWinY);
                g_menuCursors[cursorIdx].SetSlide(*pValue);
                g_menuCursors[cursorIdx].Draw(g_screenLayerBase + 2);
            }
            break;
        case MENU_MSG_LEFT:
            PauseMenu_StepSetting(pValue, -8);
            break;
        case MENU_MSG_RIGHT:
            PauseMenu_StepSetting(pValue, 8);
            break;
        case MENU_MSG_VALUE_CHANGED:
            g_pStreamPlayer->ApplyVolume();
            Ui_PlayMoveSound();
            break;
    }
}

/* "sfx" volume. */
void PauseMenu_ItemSfxVolume(u8 msg, MenuPage *item)
{
    PauseMenu_VolumeItem(msg, item, 0, &g_pProgress->sfxVolume, UISTR_SFX);
    if (msg == MENU_MSG_VALUE_CHANGED)
        Sound_SetSfxVolume(g_pProgress->sfxVolume);
}

/* "voice" volume. */
void PauseMenu_ItemVoiceVolume(u8 msg, MenuPage *item)
{
    PauseMenu_VolumeItem(msg, item, 1, &g_pProgress->streamVolumeB, UISTR_VOICE);
}

/* "music" volume. */
void PauseMenu_ItemMusicVolume(u8 msg, MenuPage *item)
{
    PauseMenu_VolumeItem(msg, item, 2, &g_pProgress->streamVolumeA, UISTR_MUSIC);
}

/* "sub-titles" on/off (Progress.optionFlags bit 2). */
void PauseMenu_ItemSubTitles(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, Text_GetUiString(UISTR_SUB_TITLES));
            Menu_Printf(item, TEXTALIGN_RIGHT, g_onOffNames[g_pProgress->optionBits.gate]);
            break;
        case MENU_MSG_VALUE_CHANGED:
            Progress_Options()->gate ^= 1;
            Ui_PlayMoveSound();
            break;
    }
}

/* "done" on the sound page: the new volumes become the snapshot's. */
void PauseMenu_ItemSoundDone(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuFooterStyle = MENU_FOOTER_VALIDATE_CANCEL;
            Menu_BuildValidateCancelFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, Text_GetUiString(UISTR_DONE));
            break;
        case MENU_MSG_CONFIRM:
            *(u32 *)&g_progressPauseCopy[0x11] = *(u32 *)&g_pProgress->streamVolumeA;
            Menu_GoBack();
            g_pauseSlidersShown = 1;
            break;
    }
}

inline s32 Progress_InLevel()
{
    s8 level = g_pProgress->currentLevel;
    return level >= SCENE_LVL_00 && level < SCENE_LEVEL_COUNT;
}
inline s8 Progress_Level()
{
    return g_pProgress->currentLevel;
}

void PauseMenu_Build()
{
    MenuPage *root;
    MenuPage *optMenu;
    MenuPage *sounds;
    MenuPage *padPage;
    MenuPage *dispPage;
    MenuPage *rewooledPage;
    MenuPage *bindPage;

    Menu_SetCurrent(&g_pauseMenu);
    root = Menu_AddPage(0, (void *)"PAUSE", 0);
    /* a menu lists its entries in the order they are added: Resume first, then Options and Rewooled Settings, then
     * the rest */
    g_pauseResumeIndex = Menu_AddItems(root, PauseMenu_ItemResume, MENU_END);
    optMenu = Menu_AddPage(root, (void *)PauseMenu_PageOptions, 1);
    sounds = Menu_AddPage(optMenu, (void *)PauseMenu_PageSoundOptions, 1);
    padPage = Menu_AddPage(optMenu, (void *)PauseMenu_PageControllerSetting, 1);
    dispPage = Menu_AddPage(optMenu, (void *)PauseMenu_PageDisplaySettings, 1);
    rewooledPage = Menu_AddPage(root, (void *)PauseMenu_PageRewooledSettings, 1); /* on the pause menu, below Options */
    if (Progress_InLevel())
        Menu_AddItems(root, PauseMenu_ItemRestartLevel, PauseMenu_ItemExit, PauseMenu_ItemQuit, MENU_END);
    else if (Progress_Level() == -1)
        Menu_AddItems(root, PauseMenu_ItemQuit, MENU_END);
    else
        Menu_AddItems(root, PauseMenu_ItemExit, PauseMenu_ItemQuit, MENU_END);
    Menu_AddItems(optMenu, PauseMenu_ItemAutoSave, MENU_END);
    Menu_AddItems(sounds, PauseMenu_ItemSpeakerMode, PauseMenu_ItemSoundsEnabled, PauseMenu_ItemSfxVolume,
                  PauseMenu_ItemVoiceVolume, PauseMenu_ItemMusicVolume, PauseMenu_ItemSubTitles,
                  PauseMenu_ItemSoundDone, MENU_END);
    Menu_AddItems(dispPage, PauseMenu_ItemFog, PauseMenu_ItemDisplayDone, MENU_END);
    Menu_AddItems(padPage, PauseMenu_ItemControlDevice, MENU_END);
    bindPage = Menu_AddPage(padPage, (void *)PauseMenu_PageEditConfig, 1);
    Menu_AddItems(rewooledPage, PauseMenu_ItemCameraSpeed, PauseMenu_ItemCameraAcceleration,
                  PauseMenu_ItemInvertCameraJoystick, PauseMenu_ItemMeshOutlines, PauseMenu_ItemTextLanguage,
                  PauseMenu_ItemVoiceLanguage, PauseMenu_ItemTextureFiltering, PauseMenu_ItemScreenPercent,
                  PauseMenu_ItemAntiAliasing, PauseMenu_ItemFpsLimit,
                  PauseMenu_ItemVSync, PauseMenu_ItemRewooledDone, MENU_END);

    Menu_AddItems(bindPage, PauseMenu_ItemBindUp, PauseMenu_ItemBindRight, PauseMenu_ItemBindDown,
                  PauseMenu_ItemBindLeft, PauseMenu_ItemBindAction, PauseMenu_ItemBindJump,
                  PauseMenu_ItemBindWolfEyeView, PauseMenu_ItemBindRun, PauseMenu_ItemBindQuickInventory,
                  PauseMenu_ItemBindWalkStealthily, PauseMenu_ItemBindCamLeft, PauseMenu_ItemBindCamRight,
                  PauseMenu_ItemBindViewMap, PauseMenu_ItemSaveConfig, MENU_END);
}

/* the "controller setting" page. */
void PauseMenu_PageControllerSetting(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildValidateBackFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_CENTER, Text_GetUiString(UISTR_CONTROLLER_SETTING));
            break;
    }
}

/* the "Display settings" page; while it has the cursor the view distance is the one saved on entry. */
void PauseMenu_PageDisplaySettings(u8 msg, MenuPage *item)
{
    float t;

    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildValidateBackFooter();
            g_pProgress->viewDistanceSetting = g_pauseViewDistance;
            t = (float)(0xff - g_pProgress->viewDistanceSetting) / 255.0f;
            g_pViewFrustum->SetViewDistance((1.0f - t) * g_viewDistNear + g_viewDistFar * t, 1);
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_CENTER, g_textCatalog.LoadString(1, g_uiLangMask, 0x800));
            break;
    }
}

static void PauseMenu_PageRewooledSettings(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildPauseFooter();
            g_meshOutlinesEnabled = g_pauseMeshOutlines;
            g_pauseTextLanguage = LangPack_ChosenText();
            g_pauseVoiceLanguage = LangPack_ChosenVoices();
            g_textureFilterMode = g_pauseTextureFilterMode;
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_CENTER, "%s", PauseMenu_RewooledText(RWMENU_REWOOLED_SETTINGS));
            break;
    }
}

/* Rewooled's own menu text, from the menu catalogue as the game's own is (rewooled_menu_strings.h: a language pack's
 * Menus.txt, else the text Rewooled carries for the game's languages); kept until the next call for the same string */
static const char *PauseMenu_RewooledText(u16 index)
{
    static char text[RWMENU_COUNT + 1][0x80]; /* 0x80: the message box's own buffer, g_msgBoxText */
    if (index > RWMENU_COUNT)
        return "";
    char *s = g_textCatalog.LoadString(REWOOLED_MENU_GROUP, g_uiLangMask, index);
    snprintf(text[index], sizeof(text[index]), "%s", s ? s : "");
    delete[] s;
    return text[index];
}

/* the "Edit config." page: entering it reloads the saved bindings. */
void PauseMenu_PageEditConfig(u8 msg, MenuPage *item)
{
    u8 device;

    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildValidateBackFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, g_textCatalog.LoadString(0x10, g_uiLangMask, 8));
            break;
        case MENU_MSG_FOCUS:
            device = g_inputMgr.GetCurrentDeviceIdx();
            g_inputMgr.LoadConfig(g_dirReference);
            g_inputMgr.SelectDevice(device);
            break;
    }
}

/* "Quit...". */
void PauseMenu_ItemQuit(u8 msg, MenuPage *item)
{
    PauseMenu_QuitConfirm(msg, item);
    if (msg == MENU_MSG_DRAW)
        Menu_Printf(item, TEXTALIGN_CENTER, g_textCatalog.LoadString(2, g_uiLangMask, 0x10));
}

/* the quit yes/no box: "yes" writes "CfgGame" and posts WM_QUIT. */
void PauseMenu_QuitConfirm(u8 msg, MenuPage *item)
{
    HKEY key;
    DWORD len;

    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildQuitFooter();
            break;
        case MENU_MSG_CONFIRM:
            if (g_quitPending == 0) {
                g_pauseSlidersShown = 1;
                Menu_SetCapture(MENU_CAPTURE_HIDDEN, 0);
            } else {
                GAME_STATE->Game_SetFlags(GF_PAUSED, g_quitChoice);
                g_quitChoice = 1;
            }
            g_quitPending = 0;
            break;
        case MENU_MSG_CAPTURE_TICK:
            Text_CenterVertically(4);
            Text_Printf(TEXTALIGN_CENTER, Text_GetUiString(UISTR_ARE_YOU_SURE));
            Text_NewLine(2);
            Menu_PrintfSelected(g_quitChoice, TEXTALIGN_CENTER, Text_GetUiString(UISTR_NO));
            Text_NewLine(1);
            Menu_PrintfSelected(g_quitChoice == 0, TEXTALIGN_CENTER, Text_GetUiString(UISTR_YES));
            if (Pad_MenuPressed((u16)~PAD_CROSS)) {
                if (g_quitChoice == 0) {
                    len = 7;
                    g_quitCfg[0] = g_pProgress->streamVolumeA;
                    g_quitCfg[1] = g_pProgress->streamVolumeB;
                    g_quitCfg[2] = g_pProgress->sfxVolume;
                    g_quitCfg[3] = g_pProgress->optionBits.soundMode;
                    g_quitCfg[4] = g_pProgress->optionBits.setting;
                    g_quitCfg[5] = g_pProgress->optionBits.gate;
                    g_quitCfg[6] = g_pProgress->viewDistanceSetting;
                    if (Reg_CreateSubKey(&key, "CfgGame") == 0) {
                        if (Reg_WriteBinary(key, 0, g_quitCfg, len) == 1)
                            Reg_CloseKey(&key);
                    }
                    Platform_RequestQuit(); /* the main loop reads SDL's events, not the thread's messages */
                } else {
                    g_quitPending = 0;
                    Menu_SetCapture(MENU_CAPTURE_NONE, 1);
                    Ui_PlayConfirmSound();
                    GAME_STATE->Game_SetFlags(GF_PAUSE_TOGGLE, g_quitChoice);
                }
            }
            if (Pad_MenuPressed((u16)~PAD_TRIANGLE)) {
                g_quitPending = 0;
                Menu_SetCapture(MENU_CAPTURE_NONE, 1);
                Menu_SetDirty(1);
                Ui_PlayCancelSound();
            }
            if (Pad_MenuRepeat((u16)~PAD_UP) || Pad_MenuRepeat((u16)~PAD_DOWN)) {
                g_quitChoice ^= 1;
                Ui_PlayMoveSound();
            }
            break;
        case MENU_MSG_CAPTURE_RELEASE:
            g_pauseSlidersShown = 1;
            g_quitChoice = 1;
            break;
    }
}

/* "Sounds Enabled" (Progress.optionFlags bits 3-4, cycled % 3). */
void PauseMenu_ItemSoundsEnabled(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, g_textCatalog.LoadString(1, g_uiLangMask, 0x2000));
            Menu_Printf(item, TEXTALIGN_RIGHT, g_soundModeNames[g_pProgress->optionBits.soundMode]);
            break;
        case MENU_MSG_LEFT:
            Progress_Options()->soundMode = (RECORD_OPTIONS(Progress_Record())->soundMode + 2) % 3;
            break;
        case MENU_MSG_RIGHT:
            Progress_Options()->soundMode = (RECORD_OPTIONS(Progress_Record())->soundMode + 1) % 3;
            break;
        case MENU_MSG_VALUE_CHANGED:
            Ui_PlayMoveSound();
            break;
    }
}

/* "Fog": the view-distance slider (cursor 3), applied to the frustum on every change. */
void PauseMenu_ItemFog(u8 msg, MenuPage *item)
{
    float t;

    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuCursors[3].Animate();
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, g_textCatalog.LoadString(1, g_uiLangMask, 0x1000));
            if (item->selected || g_pauseSlidersShown) {
                g_menuCursors[3].SetPos(0xdd, (s16)g_textCursorY + g_textWinY);
                g_menuCursors[3].SetSlide(g_pProgress->viewDistanceSetting);
                g_menuCursors[3].Draw(g_screenLayerBase + 2);
            }
            break;
        case MENU_MSG_LEFT:
            PauseMenu_StepSetting(&g_pProgress->viewDistanceSetting, -8);
            break;
        case MENU_MSG_RIGHT:
            PauseMenu_StepSetting(&g_pProgress->viewDistanceSetting, 8);
            break;
        case MENU_MSG_VALUE_CHANGED:
            t = (float)(0xff - g_pProgress->viewDistanceSetting) / 255.0f;
            g_pViewFrustum->SetViewDistance((1.0f - t) * g_viewDistNear + g_viewDistFar * t, 1);
            break;
    }
}

/* Preview mesh outlines using the same Yes/No controls as the other toggles. */
static void PauseMenu_ItemMeshOutlines(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "%s", PauseMenu_RewooledText(RWMENU_OBJECT_OUTLINES));
            Menu_Printf(item, TEXTALIGN_RIGHT, g_onOffNames[g_meshOutlinesEnabled]);
            break;
        case MENU_MSG_VALUE_CHANGED:
            g_meshOutlinesEnabled ^= 1;
            Ui_PlayMoveSound();
            break;
    }
}

/* "Text Language": every language, built in or a pack (langpack.h); Done saves it, for the next level load */
static void PauseMenu_ItemTextLanguage(u8 msg, MenuPage *item)
{
    s32 count = LangPack_Count();
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "%s", PauseMenu_RewooledText(RWMENU_TEXT_LANGUAGE));
            Menu_Printf(item, TEXTALIGN_RIGHT, LangPack_Name(g_pauseTextLanguage));
            break;
        case MENU_MSG_LEFT:
            if (count > 0)
                g_pauseTextLanguage = (g_pauseTextLanguage + count - 1) % count;
            break;
        case MENU_MSG_RIGHT:
            if (count > 0)
                g_pauseTextLanguage = (g_pauseTextLanguage + 1) % count;
            break;
        case MENU_MSG_VALUE_CHANGED:
            Ui_PlayMoveSound();
            break;
    }
}

/* Preview the texture filter: Nearest (sharp, pixelated textures), Bilinear (the game's own), or Sharp Bilinear (pixelated
 * textures whose texel edges are blended over a narrow band, so they do not stair-step). */
static void PauseMenu_ItemTextureFiltering(u8 msg, MenuPage *item)
{
    static const char *const names[3] = {"Nearest", "Bilinear", "Sharp Bilinear"};
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "Texture Filtering");
            Menu_Printf(item, TEXTALIGN_RIGHT, "%s", names[g_textureFilterMode % 3]);
            break;
        case MENU_MSG_LEFT:
            g_textureFilterMode = (u8)((g_textureFilterMode + 2) % 3);
            break;
        case MENU_MSG_RIGHT:
            g_textureFilterMode = (u8)((g_textureFilterMode + 1) % 3);
            break;
        case MENU_MSG_VALUE_CHANGED:
            Ui_PlayMoveSound();
            break;
    }
}

/* Screen percent: the share of the window's size the game renders at. The picture is scaled up to the window afterwards,
 * with nearest-neighbour below 100, so the pixels stay sharp. It applies as soon as it is changed, and is saved. */
static const int g_screenPercentSteps[] = {25, 33, 50, 67, 75, 100, 125, 150, 200};

static void PauseMenu_ItemScreenPercent(u8 msg, MenuPage *item)
{
    const int count = (int)(sizeof(g_screenPercentSteps) / sizeof(g_screenPercentSteps[0]));
    int index = 0, best = 100000, i;
    char text[8];

    /* the step nearest the current value: the launcher takes any value from 25 to 200 */
    for (i = 0; i < count; i++) {
        int d = g_options.renderScale - g_screenPercentSteps[i];
        if (d < 0)
            d = -d;
        if (d < best) {
            best = d;
            index = i;
        }
    }
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            snprintf(text, sizeof(text), "%d", g_options.renderScale); /* no "%": the game font draws it as another symbol */
            Menu_Printf(item, TEXTALIGN_LEFT, "Screen Percent");
            Menu_Printf(item, TEXTALIGN_RIGHT, "%s", text);
            break;
        case MENU_MSG_LEFT:
            g_options.renderScale = g_screenPercentSteps[(index + count - 1) % count];
            break;
        case MENU_MSG_RIGHT:
            g_options.renderScale = g_screenPercentSteps[(index + 1) % count];
            break;
        case MENU_MSG_VALUE_CHANGED:
            PauseMenu_SaveRenderSettings();
            Ui_PlayMoveSound();
            break;
    }
}

/* the voice language after `from` in direction dir: -1 (the text language's own voices), then every language that
 * has voices */
static s32 PauseMenu_StepVoiceLanguage(s32 from, s32 dir)
{
    s32 count = LangPack_Count(), lang = from, i;
    for (i = 0; i <= count; i++) {
        lang += dir;
        if (lang >= count)
            lang = -1;
        else if (lang < -1)
            lang = count - 1;
        if (lang < 0 || LangPack_HasVoices(lang))
            return lang;
    }
    return -1;
}

/* "Voice Language": the voices of any language that has them, or the text language's own; Done saves it, at once */
static void PauseMenu_ItemVoiceLanguage(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "%s", PauseMenu_RewooledText(RWMENU_VOICE_LANGUAGE));
            Menu_Printf(item, TEXTALIGN_RIGHT,
                        g_pauseVoiceLanguage < 0 ? PauseMenu_RewooledText(RWMENU_SAME_AS_TEXT)
                                                 : LangPack_Name(g_pauseVoiceLanguage));
            break;
        case MENU_MSG_LEFT:
            g_pauseVoiceLanguage = PauseMenu_StepVoiceLanguage(g_pauseVoiceLanguage, -1);
            break;
        case MENU_MSG_RIGHT:
            g_pauseVoiceLanguage = PauseMenu_StepVoiceLanguage(g_pauseVoiceLanguage, 1);
            break;
        case MENU_MSG_VALUE_CHANGED:
            Ui_PlayMoveSound();
            break;
    }
}

/* Anti-aliasing: multisampling. It applies as soon as it is changed, and is saved. A count the graphics card does not
 * support is replaced by the best one below it, and the row then shows the count that runs. */
static void PauseMenu_ItemAntiAliasing(u8 msg, MenuPage *item)
{
    static const int values[] = {0, 2, 4, 8};
    static const char *const names[] = {"Off", "MSAA 2x", "MSAA 4x", "MSAA 8x"};
    int index = 0, i;

    for (i = 0; i < 4; i++)
        if (g_options.msaa >= values[i])
            index = i;
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "Anti-Aliasing");
            if (PauseMenu_MsaaSupported(values[index])) {
                Menu_Printf(item, TEXTALIGN_RIGHT, "%s", names[index]);
            } else {
                /* this graphics API cannot do it: greyed out, as the menu's disabled rows are, with "Unsupported" before
                 * it. It can still be selected; Done turns it into the best supported choice. */
                u32 savedColor = g_pCurFont->color;
                g_pCurFont->color = 0xa0a0a0;
                Menu_Printf(item, TEXTALIGN_RIGHT, "Unsupported %s", names[index]);
                g_pCurFont->color = savedColor;
            }
            break;
        case MENU_MSG_LEFT:
            g_options.msaa = values[(index + 3) % 4];
            break;
        case MENU_MSG_RIGHT:
            g_options.msaa = values[(index + 1) % 4];
            break;
        case MENU_MSG_VALUE_CHANGED:
            PauseMenu_SaveRenderSettings();
            Ui_PlayMoveSound();
            break;
    }
}

/* FPS Limit: the frame rate the game is held to, 20 to 300 in steps of 5. It applies as soon as it is changed, and is saved.
 * With V-Sync on, the display's refresh rate is a limit too: the lower of the two applies. */
static void PauseMenu_ItemFpsLimit(u8 msg, MenuPage *item)
{
    char text[8];
    int fps = g_options.fpsLimit;

    if (fps < 20)
        fps = fps <= 0 ? 300 : 20;
    if (fps > 300)
        fps = 300;
    fps = (fps + 2) / 5 * 5; /* a value saved by hand rounds to a step */
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            snprintf(text, sizeof(text), "%d", fps);
            Menu_Printf(item, TEXTALIGN_LEFT, "FPS Limit");
            Menu_Printf(item, TEXTALIGN_RIGHT, "%s", text);
            break;
        case MENU_MSG_LEFT:
            g_options.fpsLimit = fps > 20 ? fps - 5 : 20;
            break;
        case MENU_MSG_RIGHT:
            g_options.fpsLimit = fps < 300 ? fps + 5 : 300;
            break;
        case MENU_MSG_VALUE_CHANGED:
            Options_SaveFrameSettings(g_options.fpsLimit, g_options.vsync);
            Ui_PlayMoveSound();
            break;
    }
}

/* V-Sync: Yes, a frame waits for the display's refresh (no tearing, but never more frames than the display shows); No, it
 * does not, so the FPS Limit can be above the refresh rate. It applies as soon as it is changed, and is saved. */
static void PauseMenu_ItemVSync(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "V-Sync");
            Menu_Printf(item, TEXTALIGN_RIGHT, g_onOffNames[g_options.vsync != 0]);
            break;
        case MENU_MSG_VALUE_CHANGED:
            g_options.vsync = g_options.vsync ? 0 : 1;
            Options_SaveFrameSettings(g_options.fpsLimit, g_options.vsync);
            Ui_PlayMoveSound();
            break;
    }
}

/* "Done" keeps the display settings. */
void PauseMenu_ItemDisplayDone(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuFooterStyle = MENU_FOOTER_VALIDATE_CANCEL;
            Menu_BuildValidateCancelFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, Text_GetUiString(UISTR_DONE));
            break;
        case MENU_MSG_CONFIRM: {
            g_pauseViewDistance = g_pProgress->viewDistanceSetting;
            Menu_GoBack();
            g_pauseSlidersShown = 1;
            break;
        }
    }
}

/* Camera settings save on change; "Done" accepts and saves the outline preview. */
static void PauseMenu_ItemRewooledDone(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuFooterStyle = MENU_FOOTER_VALIDATE_CANCEL;
            Menu_BuildValidateCancelFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, Text_GetUiString(UISTR_DONE));
            break;
        case MENU_MSG_CONFIRM: {
            s32 textChanged = g_pauseTextLanguage != LangPack_ChosenText();
            g_pauseMeshOutlines = g_meshOutlinesEnabled;
            LangPack_ChooseText(g_pauseTextLanguage);
            LangPack_ChooseVoices(g_pauseVoiceLanguage);
            if (textChanged && g_pauseTextLanguage != LangPack_ActiveText())
                Menu_ShowMessageBox(PauseMenu_RewooledText(RWMENU_TEXT_LANGUAGE_SAVED), TEXTALIGN_CENTER, 2500);
            g_pauseTextureFilterMode = g_textureFilterMode;
            SaveKey *key = Save_OpenKey("CfgGame");
            if (key) {
                Save_SetValue(key, "CelShadingOutlines", 3 /* REG_BINARY */,
                              &g_pauseMeshOutlines, sizeof(g_pauseMeshOutlines));
                Save_SetValue(key, "TextureFiltering", 3 /* REG_BINARY */,
                              &g_pauseTextureFilterMode, sizeof(g_pauseTextureFilterMode));
                Save_CloseKey(key);
            }
            /* an unsupported anti-aliasing choice (greyed out in its row) becomes the best supported one */
            g_options.msaa = PauseMenu_SupportedMsaa(g_options.msaa);
            PauseMenu_SaveRenderSettings();
            Menu_GoBack();
            g_pauseSlidersShown = 1;
            break;
        }
    }
}

/* "Control device": keyboard or joystick; CROSS switches, re-creating the joystick if it was lost. */
void PauseMenu_ItemControlDevice(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildControlsFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, g_textCatalog.LoadString(1, g_uiLangMask, 0x400));
            switch (g_inputMgr.GetCurrentDeviceIdx()) {
                case INPUTDEV_MASK_JOYSTICK:
                    Menu_Printf(item, TEXTALIGN_RIGHT, g_textCatalog.LoadString(0x10, g_uiLangMask, 2));
                    break;
                default:
                    Menu_Printf(item, TEXTALIGN_RIGHT, g_textCatalog.LoadString(0x10, g_uiLangMask, 4));
                    break;
            }
            break;
        case MENU_MSG_CONFIRM:
            switch (g_inputMgr.GetCurrentDeviceIdx()) {
                case INPUTDEV_MASK_KEYBOARD:
                    if (g_inputMgr.SetAcquired(INPUTDEV_MASK_JOYSTICK, 1) == E_FAIL) {
                        if (g_inputMgr.RecreateDevice(INPUTDEV_MASK_JOYSTICK) >= 0)
                            g_inputMgr.SelectDevice(INPUTDEV_MASK_JOYSTICK);
                    } else {
                        g_inputMgr.SelectDevice(INPUTDEV_MASK_JOYSTICK);
                    }
                    break;
                default:
                    g_inputMgr.SelectDevice(INPUTDEV_MASK_KEYBOARD);
                    break;
            }
            break;
    }
}

/* Camera sliders use 33 positions so the 1x centre is reachable exactly from either endpoint. */
static void PauseMenu_CameraControlItem(u8 msg, MenuPage *item, u8 cursorIdx, u8 *value, const char *label)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuCursors[cursorIdx].Animate();
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "%s", label);
            if (item->selected || g_pauseSlidersShown) {
                g_menuCursors[cursorIdx].SetPos(0x15c, (s16)g_textCursorY + g_textWinY);
                g_menuCursors[cursorIdx].SetSlide(*value == CAMERA_CONTROL_MAX ? 255 : *value * 8);
                g_menuCursors[cursorIdx].Draw(g_screenLayerBase + 2);
            }
            break;
        case MENU_MSG_LEFT:
            if (*value > 0)
                --*value;
            break;
        case MENU_MSG_RIGHT:
            if (*value < CAMERA_CONTROL_MAX)
                ++*value;
            break;
        case MENU_MSG_VALUE_CHANGED:
            Camera_SaveControlSettings();
            Ui_PlayMoveSound();
            break;
    }
}

void PauseMenu_ItemCameraSpeed(u8 msg, MenuPage *item)
{
    PauseMenu_CameraControlItem(msg, item, 4, &g_camSpeedSetting, PauseMenu_RewooledText(RWMENU_CAMERA_SPEED));
}

void PauseMenu_ItemCameraAcceleration(u8 msg, MenuPage *item)
{
    PauseMenu_CameraControlItem(msg, item, 5, &g_camAccelerationSetting,
                                PauseMenu_RewooledText(RWMENU_CAMERA_ACCELERATION));
}

void PauseMenu_ItemInvertCameraJoystick(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildNavFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, "%s", PauseMenu_RewooledText(RWMENU_INVERT_CAMERA_JOYSTICK));
            Menu_Printf(item, TEXTALIGN_RIGHT, g_onOffNames[g_camInvertJoystick]);
            break;
        case MENU_MSG_VALUE_CHANGED:
            g_camInvertJoystick ^= 1;
            Camera_SaveControlSettings();
            Ui_PlayMoveSound();
            break;
    }
}

inline u8 Input_CurrentDevice()
{
    return g_inputMgr.GetCurrentDeviceIdx();
}

/* The four direction rows of "Edit config.": keyboard-only menu keys (slots 12..15), greyed out on the joystick. */
#define PAUSEMENU_BIND_ARROW(NAME, LABEL_INDEX, SLOT, SHOWN, JOY)                           \
    void NAME(u8 msg, MenuPage *item)                                                       \
    {                                                                                       \
        char caption[256];                                                                  \
        char joyText[128];                                                                  \
        switch (msg) {                                                                      \
            case MENU_MSG_DRAW_SELECTED:                                                    \
                Menu_BuildControlsFooter();                                                 \
                break;                                                                      \
            case MENU_MSG_PREDRAW:                                                          \
                item->disabled = Input_CurrentDevice() != INPUTDEV_MASK_KEYBOARD;           \
                break;                                                                      \
            case MENU_MSG_DRAW:                                                             \
                strcpy(caption, g_textCatalog.LoadString(0x10, g_uiLangMask, LABEL_INDEX)); \
                Menu_Printf(item, TEXTALIGN_LEFT, caption);                                 \
                if (SHOWN) {                                                                \
                    if (g_inputMgr.GetCurrentDeviceIdx() == INPUTDEV_MASK_KEYBOARD) {       \
                        Menu_DrawBindingText(SLOT, item);                                   \
                    } else {                                                                \
                        strcpy(joyText, JOY);                                               \
                        strcat(joyText, caption);                                           \
                        Menu_Printf(item, TEXTALIGN_RIGHT, joyText);                        \
                    }                                                                       \
                }                                                                           \
                break;                                                                      \
            case MENU_MSG_CONFIRM:                                                          \
                SHOWN = 0;                                                                  \
                if (!g_pauseRebinding)                                                      \
                    Menu_SetCapture(MENU_CAPTURE_DRAWN, 0);                                 \
                break;                                                                      \
            case MENU_MSG_CAPTURE_TICK:                                                     \
                g_pauseRebinding = 1;                                                       \
                g_menuFooterStyle = MENU_FOOTER_BIND;                                       \
                g_uiFooterText = g_strUiString1E;                                           \
                Menu_RebindControl(SLOT);                                                   \
                break;                                                                      \
            case MENU_MSG_CAPTURE_RELEASE:                                                  \
                SHOWN = 1;                                                                  \
                g_pauseSlidersShown = 1;                                                    \
                g_pauseRebinding = 0;                                                       \
                break;                                                                      \
        }                                                                                   \
    }

/* "Up" slot 0xc, "Right" slot 0xd, "Down" slot 0xe, "Left" slot 0xf. */
PAUSEMENU_BIND_ARROW(PauseMenu_ItemBindUp, 0x10, INPUT_BIND_MENU_UP, g_bindUpShown, "(JOY)")
PAUSEMENU_BIND_ARROW(PauseMenu_ItemBindRight, 0x20, INPUT_BIND_MENU_RIGHT, g_bindRightShown, "(JOY)")
PAUSEMENU_BIND_ARROW(PauseMenu_ItemBindDown, 0x40, INPUT_BIND_MENU_DOWN, g_bindDownShown, "(JOY)")
PAUSEMENU_BIND_ARROW(PauseMenu_ItemBindLeft, 0x80, INPUT_BIND_MENU_LEFT, g_bindLeftShown, "(JOY)")

/* The action rows: "$B_<button>$ (<name>)", the name a UI string (GET_UI) or a BSM string (GET_BSM). */
#define PAUSEMENU_GET_UI(INDEX) Text_GetUiString(INDEX)
#define PAUSEMENU_GET_BSM(INDEX) g_textCatalog.LoadString(0x10, g_uiLangMask, INDEX)
#define PAUSEMENU_BIND_ACTION(NAME, BUTTON, NAME_STRING, SLOT, SHOWN) \
    void NAME(u8 msg, MenuPage *item)                                 \
    {                                                                 \
        char label[64];                                               \
        switch (msg) {                                                \
            case MENU_MSG_DRAW_SELECTED:                              \
                Menu_BuildControlsFooter();                           \
                break;                                                \
            case MENU_MSG_DRAW:                                       \
                strcpy(label, BUTTON);                                \
                strcat(label, " (");                                  \
                strcat(label, NAME_STRING);                           \
                strcat(label, ")");                                   \
                Menu_Printf(item, TEXTALIGN_LEFT, label);             \
                if (SHOWN)                                            \
                    Menu_DrawBindingText(SLOT, item);                 \
                break;                                                \
            case MENU_MSG_CONFIRM:                                    \
                SHOWN = 0;                                            \
                if (!g_pauseRebinding)                                \
                    Menu_SetCapture(MENU_CAPTURE_DRAWN, 0);           \
                break;                                                \
            case MENU_MSG_CAPTURE_TICK:                               \
                g_pauseRebinding = 1;                                 \
                g_menuFooterStyle = MENU_FOOTER_BIND;                 \
                g_uiFooterText = g_strUiString1E;                     \
                Menu_RebindControl(SLOT);                             \
                break;                                                \
            case MENU_MSG_CAPTURE_RELEASE:                            \
                SHOWN = 1;                                            \
                g_pauseSlidersShown = 1;                              \
                g_pauseRebinding = 0;                                 \
                break;                                                \
        }                                                             \
    }

PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindAction, "$B_CROSS$", PAUSEMENU_GET_UI(UISTR_ACTION), INPUT_BIND_CROSS,
                      g_bindActionShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindJump, "$B_SQUARE$", PAUSEMENU_GET_UI(UISTR_JUMP), INPUT_BIND_SQUARE,
                      g_bindJumpShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindWolfEyeView, "$B_TRIANGLE$", PAUSEMENU_GET_UI(UISTR_WOLFS_EYE_VIEW),
                      INPUT_BIND_TRIANGLE, g_bindWolfEyeShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindRun, "$B_CIRCLE$", PAUSEMENU_GET_UI(UISTR_RUN), INPUT_BIND_CIRCLE,
                      g_bindRunShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindQuickInventory, "$B_L1$", PAUSEMENU_GET_UI(UISTR_QUICK_INVENTORY),
                      INPUT_BIND_L1, g_bindQuickInvShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindWalkStealthily, "$B_R1$", PAUSEMENU_GET_UI(UISTR_WALK_STEALTHILY),
                      INPUT_BIND_R1, g_bindSneakShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindViewMap, "$B_SELECT$", PAUSEMENU_GET_BSM(0x400), INPUT_BIND_SELECT,
                      g_bindViewMapShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindCamLeft, "$B_L2$", PAUSEMENU_GET_BSM(0x200), INPUT_BIND_L2, g_bindCamLeftShown)
PAUSEMENU_BIND_ACTION(PauseMenu_ItemBindCamRight, "$B_R2$", PAUSEMENU_GET_BSM(0x100), INPUT_BIND_R2,
                      g_bindCamRightShown)

/* "Save config": writes the bindings to the registry and says so for two seconds. */
void PauseMenu_ItemSaveConfig(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            g_menuFooterStyle = MENU_FOOTER_VALIDATE_CANCEL;
            Menu_BuildValidateCancelFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_LEFT, g_textCatalog.LoadString(0x10, g_uiLangMask, 0x800));
            break;
        case MENU_MSG_CONFIRM:
            g_inputMgr.SaveConfig(0);
            Menu_ShowMessageBox(g_textCatalog.LoadString(8, g_uiLangMask, 0x400), TEXTALIGN_CENTER, 2000);
            Menu_GoBack();
            g_pauseSlidersShown = 1;
            break;
    }
}

/* 1 if every character of the key name is a letter, a digit or one of " !()+,-./:=?" (the font has them). */
int Menu_IsKeyNameDisplayable(const char *keyName)
{
    char allowed[] = " !()+,-./:=?";
    u32 i;
    u32 j;
    int valid;
    int found;

    if (strlen(keyName) == 0)
        return 0;
    for (i = 0; i < strlen(keyName); i++) {
        valid = keyName[i] >= 'a' && keyName[i] <= 'z';
        valid |= keyName[i] >= 'A' && keyName[i] <= 'Z';
        valid |= keyName[i] >= '0' && keyName[i] <= '9';
        if (!valid) {
            found = 0;
            for (j = 0; j < strlen(allowed); j++) {
                if (keyName[i] == allowed[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found)
                return 0;
        }
    }
    return 1;
}

/* "press a key": binds the first control pressed to actionId, swapping it with the action that had it.
 * Slots 12..15 (the menu arrows) take keyboard keys only. */
void Menu_RebindControl(u8 actionId)
{
    long result;
    u8 changed;
    InputBindingIdx newKey;
    char keyText[64];
    char unused[12];
    int usable;
    u8 clash;
    InputBindingIdx oldBind;

    changed = 0;
    if (g_rebindCooldown <= 0) {
        g_inputMgr.ResetEdges();
        result = g_inputMgr.GetAnyPressed(&newKey);
        if (result == (long)E_FAIL) {
            Menu_SetCapture(MENU_CAPTURE_NONE, 0);
        } else if (result == 0) {
            g_inputMgr.Input_GetKeyName(newKey.code, keyText, 0x40);
            if (!g_inputMgr.IsReservedKey(&newKey)) {
                usable = 1;
                if (newKey.devIdx != INPUTDEV_MASK_JOYSTICK)
                    usable = Menu_IsKeyNameDisplayable(keyText);
                if (usable) {
                    clash = g_inputMgr.FindBindingByIndex(&newKey);
                    if (clash < 0x10) {
                        g_inputMgr.GetBinding(actionId, &oldBind);
                        if ((clash > INPUT_BIND_R3) == 1) {
                            if ((actionId > INPUT_BIND_R3) == 1) {
                                if (oldBind.devIdx == INPUTDEV_MASK_KEYBOARD &&
                                    newKey.devIdx == INPUTDEV_MASK_KEYBOARD) {
                                    g_inputMgr.BindMenuKey(clash, oldBind.code);
                                    g_inputMgr.BindMenuKey(actionId, newKey.code);
                                    changed = 1;
                                }
                            } else if (oldBind.devIdx == INPUTDEV_MASK_KEYBOARD) {
                                g_inputMgr.BindMenuKey(clash, oldBind.code);
                                g_inputMgr.BindAction(actionId, newKey.devIdx, newKey.code);
                                changed = 1;
                            }
                        } else if ((actionId > INPUT_BIND_R3) == 1) {
                            if (newKey.devIdx == INPUTDEV_MASK_KEYBOARD) {
                                g_inputMgr.BindAction(clash, oldBind.devIdx, oldBind.code);
                                g_inputMgr.BindMenuKey(actionId, newKey.code);
                                changed = 1;
                            }
                        } else {
                            g_inputMgr.BindAction(clash, oldBind.devIdx, oldBind.code);
                            g_inputMgr.BindAction(actionId, newKey.devIdx, newKey.code);
                            changed = 1;
                        }
                    } else if ((actionId > INPUT_BIND_R3) == 1) {
                        if (newKey.devIdx == INPUTDEV_MASK_KEYBOARD) {
                            g_inputMgr.BindMenuKey(actionId, newKey.code);
                            changed = 1;
                        }
                    } else {
                        g_inputMgr.BindAction(actionId, newKey.devIdx, newKey.code);
                        changed = 1;
                    }
                } else {
                    Menu_ShowMessageBox(g_textCatalog.LoadString(8, g_uiLangMask, 0x200), TEXTALIGN_CENTER, 2000);
                }
            } else {
                Menu_ShowMessageBox(g_textCatalog.LoadString(8, g_uiLangMask, 0x200), TEXTALIGN_CENTER, 2000);
            }
            if (changed == 1)
                Menu_SetCapture(MENU_CAPTURE_NONE, 0);
        }
    }
}

/* ======================================================================================================================
 * the tail of the menu module: the pause menu's
 * per-frame update and its TV-static backdrop, the binding text of the controls page, the footer-hint builders, the
 * timed message box, the Menu_Printf wrappers, the one-item "PAUSED" menu (PausedMenu_*).
 */

/* Their constants reach the code unfolded. */
#define SDW_INLINE_FREE_SCREENWIDTHS16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENWIDTHS16
#define SDW_INLINE_FREE_SCREENHEIGHTS16 1
#include "screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENHEIGHTS16

/* the noise texture's surface description gets its dwSize before every DirectDraw call on it: a macro, since the global
 * is re-read at every test */
#define NOISE_TEX_FIX_DESC()                                                           \
\
    if (g_pMenuNoiseTexture && !((DDSURFACEDESC2 *)&g_pMenuNoiseTexture->desc)->dwSize) \
    ((DDSURFACEDESC2 *)&g_pMenuNoiseTexture->desc)->dwSize = sizeof(DDSURFACEDESC2)

/* one frame of the open pause menu: leave it when the pad reports a connection (and the game is not in
 * flag 0x20), else the static backdrop, the "pause" title, the menu itself in the pause box, the footer hint two rows
 * below it and the message box on top. */
void PauseMenu_Update()
{
    u32 result;

    if (g_pad.IsConnected() && !(g_gameFlags & GF_PAUSE_TOGGLE)) {
        PauseMenu_Exit();
    } else {
        g_pPolyBin->Flush();
        PauseMenu_DrawNoiseOverlay();
        Text_SetFont(FONT_GAME_SMALL);
        g_pauseTextRect[0] = 0x80;
        g_pauseTextRect[1] = g_pauseBoxRect[1] - 0x18;
        g_pauseTextRect[2] = ScreenWidthS16() - 0x100;
        g_pauseTextRect[3] = g_pCurFont->lineHeight;
        Text_SetWindowRect(g_screenLayerBase, g_pauseTextRect, 1);
        Text_Printf(TEXTALIGN_CENTER, Text_GetUiString(UISTR_PAUSE));
        Hud_EndBox_stub();
        g_pauseFlag89 = 0;
        g_menuFooterStylePrev = g_menuFooterStyle;
        g_menuFooterStyle = MENU_FOOTER_NONE;
        Text_SetWindowRect(g_screenLayerBase + 2, g_pauseBoxRect, 1);
        result = Menu_Update(MENU_LAYOUT_CENTERED, TEXTALIGN_CENTER);
        Hud_EndBox_stub();
        g_pauseTextRect[0] = 0x10;
        g_pauseTextRect[1] = g_pauseBoxRect[1] + 0xa8 - g_pCurFont->lineHeight / 2;
        g_pauseTextRect[2] = ScreenWidthS16() - 0x20;
        g_pauseTextRect[3] = g_pCurFont->lineHeight * 2;
        Text_SetColor(0x4bccff);
        Text_SetWindowRect(g_screenLayerBase + 2, g_pauseTextRect, 1);
        Text_Printf(TEXTALIGN_CENTER, "%s", g_uiFooterText);
        Hud_EndBox_stub();
        Menu_DrawMessageBox();
    }
}

/* makes (or, after a lost surface, restores) the 256 x 256 noise texture behind the pause menu and fills it
 * with random levels 6..9 in the top nibble (alpha) over 0x00f. Returns 0 in every reachable case: E_FAIL needs
 * "create" and "lost" both set, and "lost" is only computed when the texture already exists. A failed creation
 * (status != 0) keeps the new object, clears "create", skips the fill and still returns 0. */
long Menu_CreateNoiseTexture()
{
    s32 status;
    s32 lost;
    u16 *dst;
    s32 create;

    create = 0;
    lost = 0;
    create = g_pMenuNoiseTexture == 0;
    lost = !create && g_pMenuNoiseTexture->surface && g_renderDevice->IsTextureLost(g_pMenuNoiseTexture->surface);
    if (create) {
        NOISE_TEX_FIX_DESC();
        g_pMenuNoiseTexture = new Texture(g_pD3DAppMain, 0x100, 0x100, TEXFMT_ARGB4444, &status);
        if (status)
            create = 0;
    }
    if (lost) {
        NOISE_TEX_FIX_DESC();
        if (g_renderDevice->RestoreTexture(g_pMenuNoiseTexture->surface)) /* as the original: a FAILED restore clears it */
            lost = 0;
    }
    if (lost ^ create) {
        if (g_pMenuNoiseTexture->Surface_LockForWrite((DDSURFACEDESC2 *)&g_pMenuNoiseTexture->desc) >= 0) {
            srand(time(0));
            dst = (u16 *)((DDSURFACEDESC2 *)&g_pMenuNoiseTexture->desc)->lpSurface;
            if (dst) {
                u16 row;
                u16 column;
                for (row = 0; row < 0x100; row++) {
                    for (column = 0; column < 0x100; column++) {
                        s32 grey = Crt_Rand15() * 4 / 0x7fff + 6;
                        dst[(row << 8) + column] = ((u16)grey << 12) + 0xf;
                    }
                }
            }
            g_pMenuNoiseTexture->Surface_Unlock();
            NOISE_TEX_FIX_DESC();
            return 0;
        }
    }
    if (!create || !lost)
        return 0;
    return E_FAIL;
}

/* the pause menu's backdrop: the noise texture stretched over the whole viewport (texture coordinates up to
 * viewport / 256, so it tiles), grey 0xc0c0c0. */
void PauseMenu_DrawNoiseOverlay()
{
    float uMax;
    float v;

    if (Menu_CreateNoiseTexture() == 0) {
        uMax = g_pViewFrustum->viewportWidth / 256.0f;
        v = g_pViewFrustum->viewportHeight / 256.0f;
        Draw2D_TexRect_Immediate(
            g_screen.Draw2D_LayerToZ(g_screenLayerBase + 3), 0, 0, g_pViewFrustum->viewportWidth,
            g_pViewFrustum->viewportHeight, RSF_BLEND_ALPHA | RSF_ZWRITE_OFF | RSF_TEXTURED | RSF_FILTER_LINEAR,
            g_pMenuNoiseTexture, 0, 0, 0xc0c0c0, 0, v, 0xc0c0c0, uMax, 0, 0xc0c0c0, uMax, v, 0xc0c0c0);
    }
}

/* the control currently bound to an action, printed on its row of the controls page: "(MOUSE)n" or "(JOY)n"
 * in green, or the key's name in yellow, cut at its first space when it would run past x 0x1c0 of the window. */
void Menu_DrawBindingText(u8 actionId, MenuPage *item)
{
    char *firstSpace;
    s32 n;
    s32 room;
    char name[64];
    char text[64];
    InputBindingIdx b;

    g_inputMgr.GetBinding(actionId, &b);
    switch (b.devIdx) {
        case INPUTDEV_MASK_MOUSE:
            sprintf(text, "(MOUSE)%d", b.code);
            Text_SetColor(0x50ff50);
            break;
        case INPUTDEV_MASK_JOYSTICK:
            sprintf(text, "(JOY)%d", b.code);
            Text_SetColor(0x50ff50);
            break;
        default:
            g_inputMgr.Input_GetKeyName(b.code, name, 0x40);
            room = (g_textClipOffX + 0x1c0 - g_textCursorX) / g_pCurFont->glyphWidth - 1;
            if (room < strlen(name)) {
                firstSpace = strchr(name, ' ');
                if (firstSpace)
                    n = firstSpace - name;
                else
                    n = room;
                strncpy(text, name, n);
                text[n] = 0;
            } else {
                strcpy(text, name);
            }
            Text_SetColor(0xffff50);
            break;
    }
    Menu_Printf(item, TEXTALIGN_RIGHT, text);
    Text_SetColor(0x4bccff);
}

/* footer "VALID <Return> validate / CANCEL <Escape> <UI string 0x21>" (style 8). */
void Menu_BuildValidateCancelFooter()
{
    char format[256];
    char keyValid[64];
    char keyCancel[64];

    g_inputMgr.Input_GetKeyName(DIK_RETURN, keyValid, 0x40);
    g_inputMgr.Input_GetKeyName(DIK_ESCAPE, keyCancel, 0x40);
    g_menuFooterStyle = MENU_FOOTER_VALIDATE_CANCEL;
    strcpy(format, "$B_VALID$/$C_CYAN$");
    strcat(format, keyValid);
    strcat(format, "$C_DEFAULT$ %s\n");
    strcat(format, "$B_CANCEL$/$C_CYAN$");
    strcat(format, keyCancel);
    strcat(format, "$C_DEFAULT$ %s");
    g_uiFooterText = Text_Sprintf(g_menuFooterText, format, g_strValidate, g_strUiString21);
}

/* footer "VALID <Return> validate / CANCEL <Escape> cancel" (style 1). */
void Menu_BuildValidateBackFooter()
{
    char format[256];
    char keyValid[64];
    char keyCancel[64];

    g_inputMgr.Input_GetKeyName(DIK_RETURN, keyValid, 0x40);
    g_inputMgr.Input_GetKeyName(DIK_ESCAPE, keyCancel, 0x40);
    g_menuFooterStyle = MENU_FOOTER_VALIDATE_BACK;
    strcpy(format, "$B_VALID$/$C_CYAN$");
    strcat(format, keyValid);
    strcat(format, "$C_DEFAULT$ %s\n");
    strcat(format, "$B_CANCEL$/$C_CYAN$");
    strcat(format, keyCancel);
    strcat(format, "$C_DEFAULT$ %s");
    g_uiFooterText = Text_Sprintf(g_menuFooterText, format, g_strValidate, g_strCancel);
}

void Menu_BuildQuitFooter()
{
    Menu_BuildValidateBackFooter();
}

void Menu_BuildPauseFooter()
{
    Menu_BuildValidateBackFooter();
}

/* footer "CANCEL <Escape> cancel" on the second line (style 3). */
void Menu_BuildBackFooter()
{
    char format[256];
    char keyCancel[64];

    g_inputMgr.Input_GetKeyName(DIK_ESCAPE, keyCancel, 0x40);
    g_menuFooterStyle = MENU_FOOTER_BACK;
    strcpy(format, "\n$B_CANCEL$/$C_CYAN$");
    strcat(format, keyCancel);
    strcat(format, "$C_DEFAULT$ %s");
    g_uiFooterText = Text_Sprintf(g_menuFooterText, format, g_strCancel);
}

/* footer "LEFT/RIGHT <UI string 0x1c> / CANCEL <Escape> cancel" (style 5). */
void Menu_BuildNavFooter()
{
    char format[256];
    char keyCancel[64];

    g_inputMgr.Input_GetKeyName(DIK_ESCAPE, keyCancel, 0x40);
    g_menuFooterStyle = MENU_FOOTER_NAV;
    strcpy(format, "$B_LEFT$$B_RIGHT$ %s\n");
    strcat(format, "$B_CANCEL$/$C_CYAN$");
    strcat(format, keyCancel);
    strcat(format, "$C_DEFAULT$ %s");
    g_uiFooterText = Text_Sprintf(g_menuFooterText, format, g_strUiString1C, g_strCancel);
}

void Menu_BuildAutoSaveFooter()
{
    Menu_BuildNavFooter();
}

/* footer "VALID <Return> <UI string 0x1c> / CANCEL <Escape> cancel" (style 6). */
void Menu_BuildControlsFooter()
{
    char format[256];
    char keyValid[64];
    char keyCancel[64];

    g_inputMgr.Input_GetKeyName(DIK_RETURN, keyValid, 0x40);
    g_inputMgr.Input_GetKeyName(DIK_ESCAPE, keyCancel, 0x40);
    g_menuFooterStyle = MENU_FOOTER_CONTROLS;
    strcpy(format, "$B_VALID$/$C_CYAN$");
    strcat(format, keyValid);
    strcat(format, "$C_DEFAULT$ %s\n");
    strcat(format, "$B_CANCEL$/$C_CYAN$");
    strcat(format, keyCancel);
    strcat(format, "$C_DEFAULT$ %s");
    g_uiFooterText = Text_Sprintf(g_menuFooterText, format, g_strUiString1C, g_strCancel);
}

/* opens the timed message box: sized to the text plus a one-cell margin, centred on the screen, framed with
 * frame bitmap 0x15. The text is strcpy'd into a 0x80-byte buffer with no bound. */
void Menu_ShowMessageBox(const char *text, u8 align, s32 durationMs)
{
    g_msgBoxRect[2] = (Text_MaxLineLength(text) + 2) * g_pCurFont->glyphWidth;
    g_msgBoxRect[3] = (Text_CountWrappedLines(text) + 2) * g_pCurFont->lineHeight;
    g_msgBoxRect[0] = ScreenWidthS16() / 2 - g_msgBoxRect[2] / 2;
    g_msgBoxRect[1] = ScreenHeightS16() / 2 - g_msgBoxRect[3] / 2;
    Ui_BuildFrameQuads(&g_msgBoxFrame, g_msgBoxRect, DAV_IDI_IGLCADP_, 8);
    strcpy(g_msgBoxText, text);
    g_msgBoxAlign = align;
    g_rebindCooldown = durationMs;
}

/* draws the message box while its time lasts (holding the menu's input meanwhile) and counts the time down
 * in real milliseconds; releases the menu when it runs out. */
void Menu_DrawMessageBox()
{
    HudElement hud(HudElement::Centre, HudElement::Centre, HudElement::CentreRect, g_msgBoxRect);
    u32 color;
    u16 strip;
    struct {
        float y0, x0, x1, y1;
    } scr;

    if (g_rebindCooldown > 0) {
        Menu_SetCapture(MENU_CAPTURE_DRAWN, 0);
        Draw2D_FlatRect_Immediate(g_screen.Draw2D_LayerToZ(g_screenLayerBase + 1), g_screen.ScaleX(g_msgBoxRect[0]),
                                  g_screen.ScaleY(g_msgBoxRect[1]), g_screen.ScaleX(g_msgBoxRect[0] + g_msgBoxRect[2]),
                                  g_screen.ScaleY(g_msgBoxRect[1] + g_msgBoxRect[3]), RSF_BLEND_ALPHA, 0x60404040);
        Text_SetWindow(g_screenLayerBase + 1, g_msgBoxRect[0], g_msgBoxRect[1], g_msgBoxRect[2], g_msgBoxRect[3], 1);
        Text_CenterVertically(Text_CountWrappedLines(g_msgBoxText));
        Text_PrintfStyled(g_msgBoxAlign, 1, "%s", g_msgBoxText);
        color = 0xaa8832;
        for (strip = 0; strip < 4; strip++) {
            scr.x0 = g_screen.ScaleX(g_msgBoxFrame.quads[strip][0]);
            scr.y0 = g_screen.ScaleY(g_msgBoxFrame.quads[strip][1]);
            scr.x1 = g_screen.ScaleX(g_msgBoxFrame.quads[strip][6]);
            scr.y1 = g_screen.ScaleY(g_msgBoxFrame.quads[strip][7]);
            Draw2D_TexRect(g_screen.Draw2D_LayerToZ(g_screenLayerBase + 1), scr.x0, scr.y0, scr.x1, scr.y1,
                           g_msgBoxFrame.texPage, g_msgBoxFrame.uvs[strip][0], g_msgBoxFrame.uvs[strip][1], color,
                           g_msgBoxFrame.uvs[strip][4], g_msgBoxFrame.uvs[strip][5], color, g_msgBoxFrame.uvs[strip][2],
                           g_msgBoxFrame.uvs[strip][3], color, g_msgBoxFrame.uvs[strip][6], g_msgBoxFrame.uvs[strip][7],
                           color);
        }
        g_rebindCooldown -= g_dtRawMs;
        if (g_rebindCooldown <= 0)
            Menu_SetCapture(MENU_CAPTURE_NONE, 0);
    }
}

/* Text_PrintFmtStyled with an explicit blink flag. */
void Menu_PrintfSelected(int blink, u8 align, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    Text_PrintFmtStyled(align, blink, fmt, ap);
}

/* one menu row's text; the row blinks while it is the selected one. */
void Menu_Printf(MenuPage *item, u8 align, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    Text_PrintFmtStyled(align, item->selected, fmt, ap);
}

/* opens the one-item "PAUSED" menu (the sibling of PauseMenu_Open): snapshot of the progress
 * record, game paused (0x40), flags 0x4001 cleared. */
void PausedMenu_Open()
{
    Stub_Ret();
    g_pad.SetActuator(0);
    g_pProgress->CopyRecord(g_progressPauseCopy);
    g_gameFlags |= GF_PAUSED;
    Menu_SetCurrent(&g_pausedMenu);
    Game_ClearFlags(GF_BIT0 | GF_UPDATE_OBJECTS);
    Menu_ResetToRoot(&g_pausedMenu, -1);
    PauseMenu_OnOpen_stub();
    g_menuFooterStyle = MENU_FOOTER_NONE;
    g_pauseFlag89 = 1;
    g_pauseSlidersShown = 1;
    g_pauseRebinding = 0;
    g_pauseFlag8b = 0;
    g_pauseExitRequested = 0;
    g_pauseRestartRequested = 0;
    g_inputMgr.SetActiveFlags(0);
}

void PausedMenu_Build()
{
    MenuPage *page;

    Menu_SetCurrent(&g_pausedMenu);
    page = Menu_AddPage(0, (void *)"PAUSED", 0);
    Menu_AddItems(page, PausedMenu_ItemPaused, MENU_END);
}

/* empties g_pausedMenu's node array. */
void PausedMenu_ClearItems()
{
    memset(g_pausedMenu.items, 0, g_pausedMenu.capacity * sizeof(MenuPage));
    g_pausedMenu.count = 0;
}

/* the "PAUSED" menu's only item: BSM string group 0x10, index 1. */
void PausedMenu_ItemPaused(u8 msg, MenuPage *item)
{
    switch (msg) {
        case MENU_MSG_DRAW_SELECTED:
            Menu_BuildBackFooter();
            break;
        case MENU_MSG_DRAW:
            Menu_Printf(item, TEXTALIGN_CENTER, g_textCatalog.LoadString(0x10, g_uiLangMask, 1));
            break;
    }
}

void PausedMenu_Stub() {}

void PauseMenu_OnOpen_stub() {}
