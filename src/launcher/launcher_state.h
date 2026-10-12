/* What the launcher's screens share. launcher.cpp owns the window and the loop, and draws one screen per frame: the
 * first launch's setup (screen_setup.cpp) or the main screen (screen_main.cpp), whose buttons open the popups
 * (popup_*.cpp). They all read and change the choices here; Play saves and uses them. Each popup is a pair: _Open,
 * called when its button is clicked, and _Draw, called every frame (it draws nothing while closed). Both must run
 * inside the launcher's ImGui window. */
#ifndef SDW_LAUNCHER_STATE_H
#define SDW_LAUNCHER_STATE_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "../platform/options.h"
#include "update_check.h"

struct RendererChoice {
    const char *name;    /* options.h's spelling */
    const char *label;   /* shown */
    const char *driver;  /* SDL_GPU's name */
};

/* the scenes the game can start in (Progress::GotoScene's ids) */
struct SceneChoice {
    int id;
    const char *label;
};

struct Resolution {
    int w, h;

    bool operator==(const Resolution &o) const { return w == o.w && h == o.h; }
};

/* where the game's data comes from: a folder already holding it, or a disc image the setup extracts into one */
enum GameSourceKind {
    GAME_SOURCE_FOLDER,
    GAME_SOURCE_CUE, /* a .cue and the .bin it names */
    GAME_SOURCE_ISO,
};

enum LauncherScreen {
    LAUNCHER_SCREEN_SETUP, /* the first launch's wizard: until the files the launcher needs are there */
    LAUNCHER_SCREEN_MAIN,
};

struct LauncherState {
    SDL_Window *window;
    SDL_Texture *banner;                           /* res/banner.png; 0 if it could not be made */
    LauncherScreen screen;                         /* the one drawn: a screen changes it to go to another */
    int setupStep;                                 /* the wizard's page */
    bool play, quit;                               /* set by a screen: the launcher closes (and starts the game) */
    UpdateCheckResult update;                      /* a newer release, when updateShown */
    bool updateShown;
    GameOptions options;                           /* the saved choices; Play writes the ones below into it */
    std::vector<const RendererChoice *> renderers; /* the ones that can run here; empty = Play is disabled */
    std::vector<Resolution> resolutions;
    std::vector<std::string> controllers;          /* Keyboard, then the gamepads: refreshed every frame */
    const SceneChoice *scenes;
    int sceneCount;
    int rendererIndex, resolutionIndex, sceneIndex;
    bool fullscreen;
    bool picking;                                  /* a folder or file dialog is open (its answer is an event in main) */
    bool hasGameData;                              /* the game folder (options.exeDir) has a Levels folder */
    GameSourceKind sourceKind;                     /* the last one picked: the setup's game data */
    std::string sourcePath;
    bool sourceValid;                              /* Launcher_CheckSource found it usable */
    bool sourceReady;                              /* its data is in a game folder (options.exeDir): at once for a folder,
                                                    * after the extraction for a disc image */
    std::string outputDir;                         /* where a disc image is extracted to (Save_Dir by default) */
};

/* launcher.cpp's helpers, for the popups */
void Launcher_ResetSaves();                    /* deletes the saved games and the game's own settings */
void Launcher_PickFolder(LauncherState &s);    /* the folder dialog: the game folder, options.exeDir; picking until it answers */
void Launcher_PickDiscImage(LauncherState &s); /* the file dialog for a .cue or .iso: sourcePath; picking until it answers */
void Launcher_PickOutputDir(LauncherState &s); /* the folder dialog for outputDir; picking until it answers */
bool Launcher_CheckSource(GameSourceKind kind, const char *path); /* a quick look: run on the UI thread */
std::string Launcher_DiscBinPath(const std::string &path); /* a .cue's .bin (its FILE line); a .bin as it is; "" if none */
std::string Launcher_DefaultGameDir();         /* where the game looks when options.exeDir is empty */
bool Launcher_HasGameData(const char *dir);    /* dir has a Levels folder */
std::string Launcher_FitLeft(const char *text, float width); /* text cut from the left with "..." to fit width */
void Launcher_Warning(const char *text);       /* large, bold, amber text, wrapped to the width left */

float Launcher_BannerHeight(float w);         /* the banner's height on a window w wide */
float Launcher_DrawBanner(const LauncherState &s); /* draws it at the top; returns its height */

void ScreenSetup_Draw(LauncherState &s);
/* a disc image's extraction into a game folder (extract_job.cpp), one at a time: sourcePath into outputDir. When it is
 * done, options.exeDir is outputDir (saved), hasGameData and sourceReady are set */
void Extract_Start(LauncherState &s);
bool Extract_Running();
bool Extract_StartedFor(const std::string &sourcePath); /* the last extraction was for this disc image */
void Extract_Draw(LauncherState &s);           /* its progress and Cancel, or its result and Try again */
void Extract_Shutdown();                       /* before exiting: stops and joins it, if running */
void ScreenMain_Draw(LauncherState &s);

/* the frame every popup shares: centred, modal, with a Close button at the bottom (popup_common.cpp) */
bool Popup_Begin(const char *id, bool canClose = true); /* canClose false: Escape does not close it */
void Popup_Label(const char *label);          /* a setting's label; the next widget fills the rest of its line */
void Popup_End(bool canClose = true);         /* canClose false: its Close button is disabled */

void PopupInstall_Open();
void PopupInstall_Draw(LauncherState &s);
void PopupVideo_Open();
void PopupVideo_Draw(LauncherState &s);
void PopupCredits_Open();
void PopupCredits_Draw();
void PopupAdvanced_Open();
void PopupAdvanced_Draw(LauncherState &s);

#endif
