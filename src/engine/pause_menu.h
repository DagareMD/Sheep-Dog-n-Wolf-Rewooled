#ifndef SDW_ENGINE_PAUSE_MENU_H
#define SDW_ENGINE_PAUSE_MENU_H

/* The functions and globals pause_menu.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

struct MenuPage;
class Texture;

extern Texture *g_pMenuNoiseTexture; /* built by Menu_CreateNoiseTexture */
u32 Game_IsPaused();                 /* returns g_gameFlags & 0x40 (paused) */
void Menu_BuildAutoSaveFooter();
void Menu_BuildBackFooter();
void Menu_BuildControlsFooter();
void Menu_BuildNavFooter();
void Menu_BuildPauseFooter();
void Menu_BuildQuitFooter();
void Menu_BuildValidateBackFooter();
void Menu_BuildValidateCancelFooter();
long Menu_CreateNoiseTexture();
void Menu_DrawBindingText(u8 actionId, MenuPage *item);
void Menu_DrawMessageBox();
void Menu_Printf(MenuPage *item, u8 align, const char *fmt, ...);
void Menu_PrintfSelected(int blink, u8 align, const char *fmt, ...);
void Menu_RebindControl(u8 actionId);
void Menu_ShowMessageBox(const char *text, u8 align, s32 durationMs);
void Menus_LoadLevelUi();    /* pause-menu UI setup (frame skins, UI strings) */
void PauseMenu_Build();
void PauseMenu_ClearItems();
void PauseMenu_DrawNoiseOverlay();
void PauseMenu_Exit();
void PauseMenu_ItemAutoSave(u8 msg, MenuPage *self);
void PauseMenu_ItemControlDevice(u8 msg, MenuPage *item);
void PauseMenu_ItemDisplayDone(u8 msg, MenuPage *item);
void PauseMenu_ItemExit(u8 msg, MenuPage *self);
void PauseMenu_ItemFog(u8 msg, MenuPage *item);
void PauseMenu_ItemMusicVolume(u8 msg, MenuPage *item);
void PauseMenu_ItemQuit(u8 msg, MenuPage *item);
void PauseMenu_ItemRestartLevel(u8 msg, MenuPage *self);
void PauseMenu_ItemResume(u8 msg, MenuPage *self);
void PauseMenu_ItemSfxVolume(u8 msg, MenuPage *item);
void PauseMenu_ItemSoundDone(u8 msg, MenuPage *item);
void PauseMenu_ItemSoundsEnabled(u8 msg, MenuPage *item);
void PauseMenu_ItemSpeakerMode(u8 msg, MenuPage *self);
void PauseMenu_ItemSubTitles(u8 msg, MenuPage *item);
void PauseMenu_ItemVoiceVolume(u8 msg, MenuPage *item);
void PauseMenu_OnOpen_stub();
void PauseMenu_Open();
void PauseMenu_PageControllerSetting(u8 msg, MenuPage *item);
void PauseMenu_PageDisplaySettings(u8 msg, MenuPage *item);
void PauseMenu_PageEditConfig(u8 msg, MenuPage *item);
void PauseMenu_PageOptions(u8 msg, MenuPage *self);
void PauseMenu_PageSoundOptions(u8 msg, MenuPage *self);
void PauseMenu_QuitConfirm(u8 msg, MenuPage *item);
void PauseMenu_Update(); /* pause-menu update (draw, or PauseMenu_Exit) */
void PausedMenu_Build();
void PausedMenu_ClearItems();
void PausedMenu_ItemPaused(u8 msg, MenuPage *item);
void PausedMenu_Open(); /* the other pause-menu opener (sibling of PauseMenu_Open) */

/* The functions and globals pause_menu.cpp defines, declared once for every file that uses them. */

struct Menu;

extern Menu g_pauseMenu;

#endif
