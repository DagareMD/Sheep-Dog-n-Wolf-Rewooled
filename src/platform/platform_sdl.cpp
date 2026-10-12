/* The game's window and events with SDL3 (platform.h). */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> /* main() below is the entry point on every platform (SDL provides WinMain on Windows) */

#include "platform.h"
#include "save_store.h"
#include "app_icon.h"
#include "../render/perf_overlay.h"

#ifdef _WIN32
#include <direct.h>
#define Platform_ChangeDir _chdir
#else
#include <unistd.h>
#define Platform_ChangeDir chdir
#endif

int Game_Main(int argc, char **argv); /* src/app/app_main.cpp */

static SDL_Window *s_window;
static unsigned s_flags;
static int s_wheel; /* the mouse wheel since the last Platform_ReadMouse */
static SDL_Gamepad *s_gamepad; /* the controller the game reads (Platform_ReadJoystick) */

/* SDL's log, also written to SheepD3D.log in the save folder (save_store.h), started afresh every run: on Windows the
 * game has no console, so the file is the only place a player can find SDL's errors. */
static SDL_IOStream *s_logFile;
static SDL_LogOutputFunction s_logDefault;
static void *s_logDefaultData;

static void SDLCALL Platform_LogOutput(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    (void)userdata;
    if (s_logFile) {
        SDL_IOprintf(s_logFile, "%s\n", message);
        SDL_FlushIO(s_logFile);
    }
    s_logDefault(s_logDefaultData, category, priority, message);
}

static void Platform_OpenLog()
{
    char path[1024];
    const char *dir = Save_Dir();
    SDL_CreateDirectory(dir);
    SDL_snprintf(path, sizeof(path), "%s/SheepD3D.log", dir);
    s_logFile = SDL_IOFromFile(path, "w");
    SDL_GetLogOutputFunction(&s_logDefault, &s_logDefaultData);
    SDL_SetLogOutputFunction(Platform_LogOutput, 0);
}

int Platform_Init(const char *className, int iconResource)
{
    char icon[16];
    Platform_OpenLog();
    /* the original's window is not DPI aware: Windows scales it */
    SDL_SetHint("SDL_WINDOWS_DPI_AWARENESS", "unaware");
#ifdef SDL_PLATFORM_WINDOWS
    SDL_snprintf(icon, sizeof(icon), "%d", iconResource);
    SDL_SetHint(SDL_HINT_WINDOWS_INTRESOURCE_ICON, icon);
    SDL_SetHint(SDL_HINT_WINDOWS_INTRESOURCE_ICON_SMALL, icon);
    SDL_RegisterApp(className, 0, 0); /* before SDL_Init, which would register "SDL_app" */
#else
    (void)className;
    (void)iconResource;
    (void)icon;
#endif
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
}

void *Platform_CreateWindow(const char *title, int width, int height, unsigned flags)
{
    SDL_PropertiesID props = SDL_CreateProperties();
    s_flags = flags;
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, !(flags & PLATFORM_WINDOW_DECORATED));
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, (flags & PLATFORM_WINDOW_DECORATED) != 0);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN, (flags & PLATFORM_WINDOW_VULKAN) != 0);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN, (flags & PLATFORM_WINDOW_METAL) != 0);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, (flags & PLATFORM_WINDOW_FULLSCREEN) != 0);
    s_window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);
    if (!s_window) {
        SDL_Log("SheepD3D: SDL_CreateWindow: %s", SDL_GetError());
        return 0;
    }
    App_SetWindowIcon(s_window);
    /* no cursor over the game, as the original's exclusive DirectInput mouse */
    SDL_HideCursor();
#ifdef SDL_PLATFORM_WINDOWS
    return SDL_GetPointerProperty(SDL_GetWindowProperties(s_window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, 0);
#else
    return s_window;
#endif
}

SDL_Window *Platform_Window()
{
    return s_window;
}

void Platform_ShowWindow()
{
    SDL_ShowWindow(s_window);
    SDL_RaiseWindow(s_window);
}

/* what Platform_PumpEvents saw, reported by the next Platform_PollEvents */
static unsigned s_pumpedEvents;

void Platform_PumpEvents()
{
    s_pumpedEvents |= Platform_PollEvents();
}

unsigned Platform_PollEvents()
{
    SDL_Event e;
    unsigned result = s_pumpedEvents;
    s_pumpedEvents = 0;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                result |= PLATFORM_EVENT_QUIT;
                break;
            /* controllers plugged in and out while the game runs: with none open, the new one is; when the open one
             * goes, the first left is */
            case SDL_EVENT_GAMEPAD_ADDED:
                if (!s_gamepad && (s_gamepad = SDL_OpenGamepad(e.gdevice.which)) != 0)
                    SDL_Log("SheepD3D: controller connected: %s", SDL_GetGamepadName(s_gamepad));
                break;
            case SDL_EVENT_GAMEPAD_REMOVED:
                if (s_gamepad && SDL_GetGamepadID(s_gamepad) == e.gdevice.which) {
                    SDL_Log("SheepD3D: controller disconnected");
                    Platform_CloseJoystick();
                    if (Platform_JoystickCount() > 0)
                        Platform_OpenJoystick(0);
                }
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                s_wheel += (int)(e.wheel.y * 120.0f); /* DirectInput's units: 120 per notch */
                break;
            case SDL_EVENT_KEY_DOWN:
                /* full screen, for a window the renderer scales into (Direct3D 7 has its own display modes) */
                if ((s_flags & PLATFORM_WINDOW_DECORATED) && !e.key.repeat &&
                    (e.key.key == SDLK_F11 || (e.key.key == SDLK_RETURN && (e.key.mod & SDL_KMOD_ALT))))
                    SDL_SetWindowFullscreen(s_window, !(SDL_GetWindowFlags(s_window) & SDL_WINDOW_FULLSCREEN));
                /* the performance overlay (src/render/perf_overlay.h) */
                if (e.key.key == SDLK_F3 && !e.key.repeat)
                    PerfOverlay_Toggle();
                if (e.key.key == SDLK_F5 && !e.key.repeat)
                    CamDebug_Toggle();
                break;
        }
    }
    return result;
}

int Platform_IsActive()
{
    return s_window && (SDL_GetWindowFlags(s_window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

void Platform_RequestQuit()
{
    SDL_Event e;
    SDL_zero(e);
    e.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&e);
}

void Platform_Shutdown()
{
    Platform_CloseJoystick();
    if (s_window)
        SDL_DestroyWindow(s_window);
    s_window = 0;
    SDL_Quit();
}

int Platform_GetBasePath(char *out, int capacity)
{
    const char *path = SDL_GetBasePath();
    size_t len;
    if (!path || capacity <= 0) {
        if (capacity > 0)
            out[0] = 0;
        return 0;
    }
    len = SDL_strlcpy(out, path, (size_t)capacity);
    if (len >= (size_t)capacity)
        return 0;
    /* in the macOS app bundle (Rewooled.app, CMakeLists.txt) SDL answers <folder>/Rewooled.app/Contents/Resources/:
     * the game's data is next to the .app, so the base is <folder>/ */
    {
        char *bundle = SDL_strstr(out, ".app/Contents/");
        if (bundle) {
            while (bundle > out && bundle[-1] != '/')
                bundle--;
            *bundle = 0;
        }
    }
    return 1;
}

#ifndef _WIN32
extern "C" void Sdw_ResolvePath(const char *path, char *out, size_t size); /* crt_posix_compat.cpp */
#endif

int Platform_ResolvePath(const char *path, char *out, int capacity)
{
    if (!path || !out || capacity <= 0)
        return 0;
#ifdef _WIN32
    /* Windows' file systems ignore case already */
    return SDL_strlcpy(out, path, (size_t)capacity) < (size_t)capacity;
#else
    Sdw_ResolvePath(path, out, (size_t)capacity);
    return SDL_strlen(out) + 1 < (size_t)capacity;
#endif
}

unsigned char *Platform_LoadImageRGBA(const char *path, int *width, int *height)
{
    char resolved[4096];
    SDL_Surface *loaded, *rgba;
    unsigned char *pixels = 0;
    int row;
    if (!Platform_ResolvePath(path, resolved, sizeof(resolved)))
        return 0;
    if (!(loaded = SDL_LoadPNG(resolved)))
        return 0; /* most often: no such file, the normal case for a page without an override */
    rgba = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loaded);
    if (!rgba) {
        SDL_Log("SheepD3D: %s: %s", resolved, SDL_GetError());
        return 0;
    }
    if ((pixels = (unsigned char *)SDL_malloc((size_t)rgba->w * rgba->h * 4)) != 0) {
        for (row = 0; row < rgba->h; row++)
            SDL_memcpy(pixels + (size_t)row * rgba->w * 4, (const unsigned char *)rgba->pixels + (size_t)row * rgba->pitch,
                       (size_t)rgba->w * 4);
        *width = rgba->w;
        *height = rgba->h;
    }
    SDL_DestroySurface(rgba);
    return pixels;
}

void Platform_FreeImage(unsigned char *pixels)
{
    SDL_free(pixels);
}

int Platform_EnterBaseDir()
{
    char path[4096];
    return Platform_GetBasePath(path, sizeof(path)) && Platform_ChangeDir(path) == 0;
}

void Platform_Log(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);
    va_end(ap);
}

void Platform_ShowMessage(const char *title, const char *message, int isError)
{
    SDL_ShowSimpleMessageBox(isError ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION,
                             title ? title : "SheepD3D", message ? message : "", s_window);
}

unsigned Platform_LanguageId()
{
    int count = 0;
    SDL_Locale **locales = SDL_GetPreferredLocales(&count);
    unsigned result = 9;
    if (locales && count > 0 && locales[0] && locales[0]->language) {
        const char *language = locales[0]->language;
        if (SDL_strcasecmp(language, "fr") == 0)
            result = 0x0c;
        else if (SDL_strcasecmp(language, "de") == 0)
            result = 0x07;
        else if (SDL_strcasecmp(language, "nl") == 0)
            result = 0x13;
        else if (SDL_strcasecmp(language, "es") == 0)
            result = 0x0a;
        else if (SDL_strcasecmp(language, "it") == 0)
            result = 0x10;
        else if (SDL_strcasecmp(language, "pt") == 0)
            result = 0x16;
    }
    SDL_free(locales);
    return result;
}

int main(int argc, char **argv)
{
    return Game_Main(argc, argv);
}

/* ---- the keyboard, in DirectInput scan codes ---- */

/* SDL's scan codes (USB usages) and DirectInput's (the PC's set-1 codes, 0x80 added for the E0-prefixed keys) */
static const struct {
    SDL_Scancode sdl;
    unsigned char dik;
} s_keyMap[] = {
    {SDL_SCANCODE_ESCAPE, 0x01},       {SDL_SCANCODE_1, 0x02},           {SDL_SCANCODE_2, 0x03},
    {SDL_SCANCODE_3, 0x04},            {SDL_SCANCODE_4, 0x05},           {SDL_SCANCODE_5, 0x06},
    {SDL_SCANCODE_6, 0x07},            {SDL_SCANCODE_7, 0x08},           {SDL_SCANCODE_8, 0x09},
    {SDL_SCANCODE_9, 0x0a},            {SDL_SCANCODE_0, 0x0b},           {SDL_SCANCODE_MINUS, 0x0c},
    {SDL_SCANCODE_EQUALS, 0x0d},       {SDL_SCANCODE_BACKSPACE, 0x0e},   {SDL_SCANCODE_TAB, 0x0f},
    {SDL_SCANCODE_Q, 0x10},            {SDL_SCANCODE_W, 0x11},           {SDL_SCANCODE_E, 0x12},
    {SDL_SCANCODE_R, 0x13},            {SDL_SCANCODE_T, 0x14},           {SDL_SCANCODE_Y, 0x15},
    {SDL_SCANCODE_U, 0x16},            {SDL_SCANCODE_I, 0x17},           {SDL_SCANCODE_O, 0x18},
    {SDL_SCANCODE_P, 0x19},            {SDL_SCANCODE_LEFTBRACKET, 0x1a}, {SDL_SCANCODE_RIGHTBRACKET, 0x1b},
    {SDL_SCANCODE_RETURN, 0x1c},       {SDL_SCANCODE_LCTRL, 0x1d},       {SDL_SCANCODE_A, 0x1e},
    {SDL_SCANCODE_S, 0x1f},            {SDL_SCANCODE_D, 0x20},           {SDL_SCANCODE_F, 0x21},
    {SDL_SCANCODE_G, 0x22},            {SDL_SCANCODE_H, 0x23},           {SDL_SCANCODE_J, 0x24},
    {SDL_SCANCODE_K, 0x25},            {SDL_SCANCODE_L, 0x26},           {SDL_SCANCODE_SEMICOLON, 0x27},
    {SDL_SCANCODE_APOSTROPHE, 0x28},   {SDL_SCANCODE_GRAVE, 0x29},       {SDL_SCANCODE_LSHIFT, 0x2a},
    {SDL_SCANCODE_BACKSLASH, 0x2b},    {SDL_SCANCODE_Z, 0x2c},           {SDL_SCANCODE_X, 0x2d},
    {SDL_SCANCODE_C, 0x2e},            {SDL_SCANCODE_V, 0x2f},           {SDL_SCANCODE_B, 0x30},
    {SDL_SCANCODE_N, 0x31},            {SDL_SCANCODE_M, 0x32},           {SDL_SCANCODE_COMMA, 0x33},
    {SDL_SCANCODE_PERIOD, 0x34},       {SDL_SCANCODE_SLASH, 0x35},       {SDL_SCANCODE_RSHIFT, 0x36},
    {SDL_SCANCODE_KP_MULTIPLY, 0x37},  {SDL_SCANCODE_LALT, 0x38},        {SDL_SCANCODE_SPACE, 0x39},
    {SDL_SCANCODE_CAPSLOCK, 0x3a},     {SDL_SCANCODE_F1, 0x3b},          {SDL_SCANCODE_F2, 0x3c},
    {SDL_SCANCODE_F3, 0x3d},           {SDL_SCANCODE_F4, 0x3e},          {SDL_SCANCODE_F5, 0x3f},
    {SDL_SCANCODE_F6, 0x40},           {SDL_SCANCODE_F7, 0x41},          {SDL_SCANCODE_F8, 0x42},
    {SDL_SCANCODE_F9, 0x43},           {SDL_SCANCODE_F10, 0x44},         {SDL_SCANCODE_NUMLOCKCLEAR, 0x45},
    {SDL_SCANCODE_SCROLLLOCK, 0x46},   {SDL_SCANCODE_KP_7, 0x47},        {SDL_SCANCODE_KP_8, 0x48},
    {SDL_SCANCODE_KP_9, 0x49},         {SDL_SCANCODE_KP_MINUS, 0x4a},    {SDL_SCANCODE_KP_4, 0x4b},
    {SDL_SCANCODE_KP_5, 0x4c},         {SDL_SCANCODE_KP_6, 0x4d},        {SDL_SCANCODE_KP_PLUS, 0x4e},
    {SDL_SCANCODE_KP_1, 0x4f},         {SDL_SCANCODE_KP_2, 0x50},        {SDL_SCANCODE_KP_3, 0x51},
    {SDL_SCANCODE_KP_0, 0x52},         {SDL_SCANCODE_KP_PERIOD, 0x53},   {SDL_SCANCODE_NONUSBACKSLASH, 0x56},
    {SDL_SCANCODE_F11, 0x57},          {SDL_SCANCODE_F12, 0x58},         {SDL_SCANCODE_KP_ENTER, 0x9c},
    {SDL_SCANCODE_RCTRL, 0x9d},        {SDL_SCANCODE_KP_DIVIDE, 0xb5},   {SDL_SCANCODE_PRINTSCREEN, 0xb7},
    {SDL_SCANCODE_RALT, 0xb8},         {SDL_SCANCODE_PAUSE, 0xc5},       {SDL_SCANCODE_HOME, 0xc7},
    {SDL_SCANCODE_UP, 0xc8},           {SDL_SCANCODE_PAGEUP, 0xc9},      {SDL_SCANCODE_LEFT, 0xcb},
    {SDL_SCANCODE_RIGHT, 0xcd},        {SDL_SCANCODE_END, 0xcf},         {SDL_SCANCODE_DOWN, 0xd0},
    {SDL_SCANCODE_PAGEDOWN, 0xd1},     {SDL_SCANCODE_INSERT, 0xd2},      {SDL_SCANCODE_DELETE, 0xd3},
    {SDL_SCANCODE_LGUI, 0xdb},         {SDL_SCANCODE_RGUI, 0xdc},        {SDL_SCANCODE_APPLICATION, 0xdd},
};

void Platform_ReadKeyboard(unsigned char down[256])
{
    int count = 0;
    const bool *state = SDL_GetKeyboardState(&count);
    size_t i;
    SDL_memset(down, 0, 256);
    if (!Platform_IsActive())
        return;
    for (i = 0; i < SDL_arraysize(s_keyMap); i++)
        if ((int)s_keyMap[i].sdl < count && state[s_keyMap[i].sdl])
            down[s_keyMap[i].dik] = 1;
}

int Platform_KeyName(unsigned dikCode, char *out, int outSize)
{
    size_t i;
    const unsigned char *s;
    int n = 0;
    *out = 0;
    for (i = 0; i < SDL_arraysize(s_keyMap); i++)
        if (s_keyMap[i].dik == dikCode)
            break;
    if (i == SDL_arraysize(s_keyMap))
        return 0;
    /* the key the current layout puts there ("A" is "Q" on AZERTY); its name is UTF-8, the game's text Latin-1 */
    s = (const unsigned char *)SDL_GetKeyName(SDL_GetKeyFromScancode(s_keyMap[i].sdl, SDL_KMOD_NONE, false));
    if (!*s)
        s = (const unsigned char *)SDL_GetScancodeName(s_keyMap[i].sdl);
    while (*s && n < outSize - 1) {
        unsigned c = *s++;
        if (c >= 0xc0 && c < 0xe0 && (*s & 0xc0) == 0x80) /* two bytes: U+0080..U+07FF */
            c = ((c & 0x1f) << 6) | (*s++ & 0x3f);
        else if (c >= 0x80)
            while ((*s & 0xc0) == 0x80) /* longer, or stray: skip it */
                s++, c = '?';
        out[n++] = (char)(c < 0x100 ? c : '?');
    }
    out[n] = 0;
    return n != 0;
}

/* ---- the mouse ---- */
void Platform_ReadMouse(int *dx, int *dy, int *wheel, unsigned char buttons[4])
{
    float x = 0, y = 0;
    SDL_MouseButtonFlags b = SDL_GetRelativeMouseState(&x, &y);
    int focused = Platform_IsActive();
    *dx = focused ? (int)x : 0;
    *dy = focused ? (int)y : 0;
    *wheel = focused ? s_wheel : 0;
    s_wheel = 0;
    buttons[0] = focused && (b & SDL_BUTTON_LMASK) != 0;
    buttons[1] = focused && (b & SDL_BUTTON_RMASK) != 0;
    buttons[2] = focused && (b & SDL_BUTTON_MMASK) != 0;
    buttons[3] = focused && (b & SDL_BUTTON_X1MASK) != 0;
}

/* ---- controllers: SDL's gamepads, read as a PlayStation pad ---- */

/* THE FIXED CONTROLLER MAPPING. The game reads twelve PlayStation buttons, in this order (its INPUT_BIND_* slots,
 * src/include/sdw_enums.h); each takes either a button or a trigger of SDL's standard gamepad. SDL names the face
 * buttons by POSITION (south = the bottom one: A on an Xbox pad, Cross on a PlayStation pad, B on a Switch pad), so
 * one table fits every controller SDL knows. To change the mapping, change a line here: the input manager binds the
 * game's twelve buttons to these, in this order, and nothing else overrides them (src/engine/input_mgr.cpp,
 * InputMgr::ApplyFixedPadMapping). */
#define PAD_TRIGGER_THRESHOLD 8000 /* how far a trigger (0..32767) must be pulled to count as L2 / R2 pressed */
static const struct {
    SDL_GamepadButton button; /* SDL_GAMEPAD_BUTTON_INVALID: the trigger below instead */
    SDL_GamepadAxis trigger;
} s_padMapping[12] = {
    {SDL_GAMEPAD_BUTTON_NORTH, SDL_GAMEPAD_AXIS_INVALID},            /* Triangle */
    {SDL_GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_AXIS_INVALID},             /* Circle */
    {SDL_GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_AXIS_INVALID},            /* Cross */
    {SDL_GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_AXIS_INVALID},             /* Square */
    {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_AXIS_INVALID},    /* L1 */
    {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, SDL_GAMEPAD_AXIS_INVALID},   /* R1 */
    {SDL_GAMEPAD_BUTTON_INVALID, SDL_GAMEPAD_AXIS_LEFT_TRIGGER},     /* L2 */
    {SDL_GAMEPAD_BUTTON_INVALID, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER},    /* R2 */
    {SDL_GAMEPAD_BUTTON_BACK, SDL_GAMEPAD_AXIS_INVALID},             /* Select */
    {SDL_GAMEPAD_BUTTON_START, SDL_GAMEPAD_AXIS_INVALID},            /* Start */
    {SDL_GAMEPAD_BUTTON_LEFT_STICK, SDL_GAMEPAD_AXIS_INVALID},       /* L3 */
    {SDL_GAMEPAD_BUTTON_RIGHT_STICK, SDL_GAMEPAD_AXIS_INVALID},      /* R3 */
};

int Platform_JoystickCount()
{
    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    SDL_free(ids);
    return count;
}

int Platform_WaitForJoystick(int index, int timeoutMs)
{
    Uint64 end = SDL_GetTicks() + (Uint64)timeoutMs;
    int count;
    while ((count = Platform_JoystickCount()) <= index && SDL_GetTicks() < end) {
        SDL_PumpEvents();
        SDL_UpdateGamepads();
        SDL_Delay(10);
    }
    if (count > index)
        SDL_Log("SheepD3D: controller %d: %s", index, Platform_JoystickName(index));
    else
        SDL_Log("SheepD3D: controller %d not found (%d controller%s after %d ms): using the keyboard", index, count,
                count == 1 ? "" : "s", timeoutMs);
    return count > index;
}

const char *Platform_JoystickName(int index)
{
    int count = 0;
    const char *name = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    if (ids && index >= 0 && index < count)
        name = SDL_GetGamepadNameForID(ids[index]);
    SDL_free(ids);
    return name ? name : "";
}

int Platform_OpenJoystick(int index)
{
    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    Platform_CloseJoystick();
    if (ids && index >= 0 && index < count)
        s_gamepad = SDL_OpenGamepad(ids[index]);
    SDL_free(ids);
    if (!s_gamepad)
        SDL_Log("SheepD3D: controller %d could not be opened: %s", index, SDL_GetError());
    return s_gamepad != 0;
}

void Platform_CloseJoystick()
{
    if (s_gamepad)
        SDL_CloseGamepad(s_gamepad);
    s_gamepad = 0;
}

int Platform_ReadJoystick(int axes[5], unsigned char *buttons, int buttonCount)
{
    int i, focused = Platform_IsActive(); /* as DirectInput's foreground mode: nothing without the focus */
    if (!s_gamepad || !SDL_GamepadConnected(s_gamepad))
        return 0;
    /* the left stick, -32768..32767 to about +-65535 (DirectInput's range as the game set it); no third axis */
    axes[0] = focused ? SDL_GetGamepadAxis(s_gamepad, SDL_GAMEPAD_AXIS_LEFTX) * 2 : 0;
    axes[1] = focused ? SDL_GetGamepadAxis(s_gamepad, SDL_GAMEPAD_AXIS_LEFTY) * 2 : 0;
    axes[2] = 0;
    /* the right stick, same range */
    axes[3] = focused ? SDL_GetGamepadAxis(s_gamepad, SDL_GAMEPAD_AXIS_RIGHTX) * 2 : 0;
    axes[4] = focused ? SDL_GetGamepadAxis(s_gamepad, SDL_GAMEPAD_AXIS_RIGHTY) * 2 : 0;
    for (i = 0; i < buttonCount; i++) {
        int down = 0;
        if (focused && i < 12) {
            if (s_padMapping[i].button != SDL_GAMEPAD_BUTTON_INVALID)
                down = SDL_GetGamepadButton(s_gamepad, s_padMapping[i].button);
            else
                down = SDL_GetGamepadAxis(s_gamepad, s_padMapping[i].trigger) > PAD_TRIGGER_THRESHOLD;
        }
        buttons[i] = (unsigned char)(down != 0);
    }
    return 1;
}
