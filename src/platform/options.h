/* The game's start-up options: the renderer, the window, the controller and the scene to start in.
 *
 * Each comes from, in order of precedence: the command line, the saved settings (the app key of the save store,
 * save_store.h), a default. The launcher (src/launcher/) saves its choices and starts the game with the same values as
 * flags; started on its own, the game uses the saved ones.
 *
 * Flags (a leading "-" or "--"; a value after "="):
 *   --renderer=vulkan|d3d12|metal   the SDL3 GPU driver
 *   --width=N --height=N            the window size
 *   --scale=N                       the render size as a percentage of the window's (25..200, default 100)
 *   --msaa=0|2|4|8                  multisample anti-aliasing (0: off; the GPU's best supported count up to N)
 *   --fullscreen / --windowed
 *   --controller=keyboard|N         the keyboard, or SDL's N-th controller (0-based)
 *   --level=N                       the scene to start in: -1 the title, 0..17 a disc level Lvl-NN, -2 the intro,
 *                                   -3 the hub (never saved)
 *   --game-dir=PATH                 the game's data folder (a disc drive or a copy); "" : next to the executable */
#ifndef SDW_PLATFORM_OPTIONS_H
#define SDW_PLATFORM_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#define OPTIONS_CONTROLLER_KEYBOARD (-1)

typedef struct GameOptions {
    char renderer[16]; /* "" : the first available */
    int width, height;
    int renderScale; /* percent of width x height the game renders at: 25..200, 100 = native */
    int msaa;        /* 0 = off, else the wanted sample count (2, 4 or 8) */
    int fullscreen;
    int controller; /* OPTIONS_CONTROLLER_KEYBOARD or a controller index */
    int startLevel;
    char exeDir[512]; /* the game's data folder (--game-dir, saved as GameDir); "" : next to the executable */
    int checkUpdates; /* the launcher asks GitHub for a newer release (saved as CheckUpdates); not a flag */
} GameOptions;

extern GameOptions g_options;

void Options_SetDefaults(GameOptions *o);
void Options_LoadSaved(GameOptions *o);            /* over what is in o */
void Options_ParseArgs(GameOptions *o, int argc, char **argv); /* over what is in o; argv[0] is skipped */
void Options_Save(const GameOptions *o);           /* everything but startLevel */
void Options_SaveRenderSettings(int renderScale, int msaa); /* only RenderScale and Msaa: the in-game rows, saved when changed */
void Options_SaveCheckUpdates(int on);             /* only CheckUpdates: the launcher's checkbox, saved when changed */
/* the command line that gives these options: at most max arguments into buf[i], each at most OPTIONS_ARG_SIZE bytes
 * (room for --game-dir= and a full path); OPTIONS_MAX_ARGS is enough for all of them */
#define OPTIONS_ARG_SIZE 600
#define OPTIONS_MAX_ARGS 10
int Options_ToArgs(const GameOptions *o, char buf[][OPTIONS_ARG_SIZE], int max);

#ifdef __cplusplus
}
#endif

#endif
