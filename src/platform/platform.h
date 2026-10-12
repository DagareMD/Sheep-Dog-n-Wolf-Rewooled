/* The window and its events, through SDL3 (platform_sdl.cpp): what the original does with Win32's
 * RegisterClass / CreateWindowEx, its window procedure and its PeekMessage loop (src/app/app_main.cpp).
 *
 * The renderers draw into this window (src/render/render_device.h): SDL's own GPU API through Platform_Window(), and
 * the native handle Platform_CreateWindow returns (an HWND on Windows). */
#ifndef SDW_PLATFORM_H
#define SDW_PLATFORM_H

struct SDL_Window;

/* Platform_CreateWindow's flags (a renderer's RenderBackend::WindowFlags) */
enum {
    PLATFORM_WINDOW_DECORATED = 1, /* a title bar, resizable, Alt+Enter / F11 for full screen; else a bare popup (the
                                      original's, for Direct3D 7's exclusive full screen) */
    PLATFORM_WINDOW_VULKAN = 2,    /* a Vulkan surface will be made for it */
    PLATFORM_WINDOW_METAL = 4,     /* a Metal view will be made for it */
    PLATFORM_WINDOW_FULLSCREEN = 8, /* opens full screen (on the desktop's display mode) */
};

/* Platform_PollEvents' result bits */
enum {
    PLATFORM_EVENT_QUIT = 1, /* the window was closed, or the game asked to quit (Platform_RequestQuit) */
};

/* SDL's video and controllers; on Windows also the window class name and the icon resource the window uses */
int Platform_Init(const char *className, int iconResource);
/* The game's window, hidden until Platform_ShowWindow, centred; returns its native handle (HWND on Windows), 0 when
 * it cannot be made. */
void *Platform_CreateWindow(const char *title, int width, int height, unsigned flags);
struct SDL_Window *Platform_Window();
void Platform_ShowWindow();
/* Handles the pending events; PLATFORM_EVENT_* */
unsigned Platform_PollEvents();
/* Handles the pending events for a loop that runs for a while outside the main loop (the blocking screen transition),
 * so that the system does not take the window for hung ("Not Responding" on Windows after about 5 s); what they
 * report is kept for the next Platform_PollEvents, so a quit is not lost. */
void Platform_PumpEvents();
int Platform_IsActive(); /* the window has the keyboard focus */
void Platform_RequestQuit(); /* the original's PostQuitMessage: the next Platform_PollEvents reports QUIT */
void Platform_Shutdown();
/* Platform services used by startup without exposing SDL headers to legacy CRT users. */
int Platform_GetBasePath(char *out, int capacity);
/* Makes the executable's folder the working directory (1 on success), so the game's paths can be short relative ones:
 * its fixed-size path buffers were sized for a short Windows install folder, not an absolute path. */
int Platform_EnterBaseDir();
/* The path as this system can open it: the game's data spelled in any case ("LEVELS\\LVL-01\\Lvl-01.WAR") is found
 * as it is on disc, on the case-sensitive file systems of macOS and Linux too (the lookup fopen and the level loader
 * already use: src/platform/crt_posix_compat.cpp); a path that does not exist keeps its spelling. For code that opens
 * files without the game's fopen, such as SDL's loaders: SDL_IOFromFile(resolved, "rb"). 0 when out was too small. */
int Platform_ResolvePath(const char *path, char *out, int capacity);
/* A PNG file decoded to 32-bit RGBA (bytes R, G, B, A, rows packed, top row first), the path found as
 * Platform_ResolvePath finds it; 0 when there is no such file or it is not a PNG. Free it with Platform_FreeImage. */
unsigned char *Platform_LoadImageRGBA(const char *path, int *width, int *height);
void Platform_FreeImage(unsigned char *pixels);
void Platform_ShowMessage(const char *title, const char *message, int isError);
/* A line in the log (SDL_Log: the terminal or CLion's Run window, the Console app on macOS), printf-style. For game
 * code: it cannot include SDL's headers, whose C runtime clashes with the game's own declarations (src/sdk/crt.h). */
void Platform_Log(const char *fmt, ...)
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((format(printf, 1, 2)))
#endif
    ;
unsigned Platform_LanguageId();

/* ---- input (the game's InputDevice classes read these: src/app/keyboard.cpp, joystick.cpp, src/engine/mouse.cpp) ----
 * State as of the last Platform_PollEvents. */

/* The keyboard, indexed by DirectInput scan code (DIK_*): the game's bindings, its saved configurations and its .BSC
 * files are in those codes. down[code] = 1 while the key is held (all 0 while the window has no focus). */
void Platform_ReadKeyboard(unsigned char down[256]);
/* The key's name in the current layout, in the game's 8-bit (Latin-1) text; 0 when the code has no key. */
int Platform_KeyName(unsigned dikCode, char *out, int outSize);

/* The mouse: its motion and wheel since the previous call, and its buttons (left, right, middle, X1). */
void Platform_ReadMouse(int *dx, int *dy, int *wheel, unsigned char buttons[4]);

/* Controllers: SDL's gamepads (its standard layout), in SDL's order. Platform_ReadJoystick: the left stick as the X and
 * Y axes in -65535..65535 (DirectInput's range as the game set it; Z is 0), the right stick as axes 3 and 4 (same
 * range), and the twelve PlayStation buttons in the
 * game's slot order, mapped by the fixed table in platform_sdl.cpp (s_padMapping); 0 when the controller is gone. */
/* ---- audio (platform_audio_sdl.cpp): voices mixed by SDL3 with DirectSound's semantics, which src/platform/
 * dsound_sdl.cpp puts behind the DirectSound interfaces the game's sound code calls ----
 * A voice is a buffer of PCM (8-bit unsigned or 16-bit signed, mono or stereo) played at a frequency, looping or once,
 * at a volume and pan in hundredths of a decibel (DirectSound's units). Ids are > 0; 0 is "none". */
int Platform_AudioOpen();  /* the default output; 0 when there is none */
void Platform_AudioClose(); /* destroys every voice */
/* a silent voice of `bytes` bytes; *data is its memory, written by the caller (as a locked DirectSound buffer) */
int Platform_VoiceCreate(unsigned bytes, int channels, int bits, int rate, void **data);
void Platform_VoiceDestroy(int voice);
void Platform_VoicePlay(int voice, int looping); /* from the current position */
void Platform_VoiceStop(int voice);              /* keeps the position */
int Platform_VoiceIsPlaying(int voice);
unsigned Platform_VoiceGetPosition(int voice); /* the play cursor, in bytes */
void Platform_VoiceSetPosition(int voice, unsigned byte);
void Platform_VoiceSetVolume(int voice, int centibels); /* 0 full .. -10000 silent */
void Platform_VoiceSetPan(int voice, int centibels);    /* -10000 left .. 10000 right */
void Platform_VoiceSetFrequency(int voice, unsigned hz);
/* when the play cursor passes one of the byte offsets, tag's bit (0..31) is set in the notified mask; tag < 0: none */
void Platform_VoiceSetNotify(int voice, const unsigned *offsets, int count, int tag);
unsigned Platform_AudioTakeNotified(); /* the notified tags since the last call, as a bit mask */

/* ---- .wav files (wav_sdl.cpp): read whole by SDL_LoadWAV_IO, for src/engine/wave_file.cpp (WaveFile), on every
 * platform - no WINMM mmio. The samples are in a format a voice plays: 8-bit unsigned or 16-bit signed little-endian,
 * mono or stereo (other encodings SDL reads are converted to 16-bit). ---- */
struct PlatformWav {
    void *data;         /* the 'data' chunk's samples; Platform_WavFree */
    unsigned bytes;     /* their size, whole sample frames */
    int channels, bits, rate;
    unsigned riffBytes; /* the RIFF chunk's size field (what mmioDescend put in MMCKINFO.cksize) */
};
/* the file at `path` (memory 0), or the RIFF image of memoryBytes bytes at `memory`; 1 on success, else 0 (logged) */
int Platform_WavLoad(const char *path, const void *memory, unsigned memoryBytes, struct PlatformWav *out);
void Platform_WavFree(void *data);

int Platform_JoystickCount();
/* Waits up to timeoutMs, handling events, until SDL lists controller `index`: controllers are reported asynchronously
 * (on macOS only once events are handled), so right after start-up the list can still be empty. 1 when it is there. */
int Platform_WaitForJoystick(int index, int timeoutMs);
const char *Platform_JoystickName(int index);
int Platform_OpenJoystick(int index);
void Platform_CloseJoystick();
int Platform_ReadJoystick(int axes[5], unsigned char *buttons, int buttonCount);
enum {
    PLATFORM_DPAD_UP = 1,
    PLATFORM_DPAD_RIGHT = 2,
    PLATFORM_DPAD_DOWN = 4,
    PLATFORM_DPAD_LEFT = 8,
};
/* Held D-pad directions, kept separate from the gameplay buttons and axes. Returns 0 without focus or a pad. */
unsigned Platform_ReadJoystickDpad();

#endif
