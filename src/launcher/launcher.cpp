/* The launcher: a small window (Dear ImGui over SDL3's 2D renderer) to pick the renderer, the resolution, full screen
 * and the controller. Play saves the choices (src/platform/options.h) and starts the game next to this executable with
 * the same values as flags; the game needs neither, it reads the saved ones when started on its own. */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "../platform/app_icon.h"
#include "../platform/options.h"
#include "sdw_banner_png.h" /* g_sdwBannerPng[]: res/banner.png (generated in the build folder) */
#include "sdw_launcher_font.h" /* g_sdwLauncherFont[]: Dear ImGui's Roboto Medium (generated in the build folder) */
#include "../platform/save_store.h"
#include "update_check.h"
#include "launcher_state.h"

#include <stdio.h>

#ifndef SDW_VERSION
#define SDW_VERSION "dev" /* set by CMakeLists.txt: the release's tag in the release builds */
#endif
#ifndef SDW_REPOSITORY
#define SDW_REPOSITORY "" /* set by CMakeLists.txt: the GitHub repository the update check asks; "" does not check */
#endif

#include <string.h>
#include <algorithm>
#include <string>
#include <vector>

#ifdef _WIN32
#define LAUNCHER_GAME_EXE "SheepD3D.exe"
#else
#define LAUNCHER_GAME_EXE "SheepD3D"
#endif

/* in the order the game prefers them (render_select.cpp: Metal first on macOS) */
static const RendererChoice s_renderers[] = {
#ifdef SDL_PLATFORM_APPLE
        {"metal", "Metal", "metal"},
#endif
        {"vulkan", "Vulkan", "vulkan"},
#ifdef SDL_PLATFORM_WINDOWS
        {"d3d12", "Direct3D 12", "direct3d12"},
#endif
};

/* the scenes the game can start in (Progress::GotoScene's ids): the title, then the levels in players' numbering
 * (docs/00-conventions.md) */
static const SceneChoice s_scenes[] = {
        {-1, "Title screen"},
        {-2, "Intro"},
        {-3, "Hub (level select)"},
        {0,  "Level 0 (Lvl-00)"},
        {1,  "Level 1 (Lvl-01)"},
        {2,  "Level 2 (Lvl-02)"},
        {3,  "Level 3 (Lvl-03)"},
        {4,  "Level 4 (Lvl-04)"},
        {5,  "Bonus 1 (Lvl-05)"},
        {6,  "Level 5 (Lvl-06)"},
        {7,  "Level 6 (Lvl-07)"},
        {8,  "Level 7 (Lvl-08)"},
        {9,  "Level 8 (Lvl-09)"},
        {10, "Bonus 2 (Lvl-10)"},
        {11, "Level 9 (Lvl-11)"},
        {12, "Level 10 (Lvl-12)"},
        {13, "Level 11 (Lvl-13)"},
        {14, "Level 12 (Lvl-14)"},
        {15, "Level 13 (Lvl-15)"},
        {16, "Level 14 (Lvl-16)"},
        {17, "Planet X (Lvl-17)"},
};

static std::vector<const RendererChoice *> Launcher_AvailableRenderers() {
    std::vector<const RendererChoice *> out;
    const SDL_GPUShaderFormat formats =
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXBC | SDL_GPU_SHADERFORMAT_MSL;
    for (const RendererChoice &r: s_renderers)
        if (SDL_GPUSupportsShaderFormats(formats, r.driver))
            out.push_back(&r);
    return out;
}

/* the primary display's full-screen sizes, largest first, and the saved one */
static std::vector<Resolution> Launcher_Resolutions(const GameOptions &o) {
    std::vector<Resolution> out;
    int count = 0;
    SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes(SDL_GetPrimaryDisplay(), &count);
    for (int i = 0; modes && i < count; i++) {
        Resolution r = {modes[i]->w, modes[i]->h};
        if (std::find(out.begin(), out.end(), r) == out.end())
            out.push_back(r);
    }
    SDL_free(modes);
    Resolution saved = {o.width, o.height};
    if (std::find(out.begin(), out.end(), saved) == out.end())
        out.push_back(saved);
    std::sort(out.begin(), out.end(), [](const Resolution &a, const Resolution &b) {
        return a.w != b.w ? a.w > b.w : a.h > b.h;
    });
    return out;
}

/* Keyboard, then every gamepad SDL sees, in SDL's order (the game opens them by that index: src/platform/platform_sdl.cpp) */
static std::vector<std::string> Launcher_Controllers() {
    std::vector<std::string> out;
    out.push_back("Keyboard");
    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    for (int i = 0; ids && i < count; i++) {
        const char *name = SDL_GetGamepadNameForID(ids[i]);
        out.push_back(name ? name : "Controller");
    }
    SDL_free(ids);
    return out;
}

/* removes the saved games and the game's own settings (not the launcher's) */
void Launcher_ResetSaves() {
    Save_DeleteKey("Progress");
    Save_DeleteKey("Config");
    Save_DeleteKey("Setup");
    Save_DeleteKey("CfgGame");
}

/* starts the game with o as flags; 1 when it started */
static int Launcher_StartGame(const GameOptions &o) {
    char path[4096];
    char flags[OPTIONS_MAX_ARGS][OPTIONS_ARG_SIZE];
    const char *args[OPTIONS_MAX_ARGS + 2]; /* the exe, the flags, the terminating 0 */
    int n = 0, count;
    const char *base = SDL_GetBasePath();
    const char *bundle = base ? SDL_strstr(base, ".app/Contents/") : 0;
    if (bundle) /* in the macOS app bundle the game is next to the launcher, in Contents/MacOS (CMakeLists.txt) */
        snprintf(path, sizeof(path), "%.*sMacOS/%s", (int) (bundle - base + strlen(".app/Contents/")), base,
                 LAUNCHER_GAME_EXE);
    else
        snprintf(path, sizeof(path), "%s%s", base ? base : "", LAUNCHER_GAME_EXE);
    args[n++] = path;
    count = Options_ToArgs(&o, flags, OPTIONS_MAX_ARGS);
    for (int i = 0; i < count; i++)
        args[n++] = flags[i];
    args[n] = 0;
    /* detached (its own session, no terminal): started from a terminal or an IDE, the game would otherwise get the
     * terminal's hang-up when the launcher exits a moment later, and die with it */
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, (void *) args);
    SDL_SetBooleanProperty(props, SDL_PROP_PROCESS_CREATE_BACKGROUND_BOOLEAN, true);
    SDL_Process *process = SDL_CreateProcessWithProperties(props);
    SDL_DestroyProperties(props);
    if (!process) {
        char message[4200];
        snprintf(message, sizeof(message), "The game could not be started:\n%s\n\n%s", path, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sheep, Dog 'n' Wolf: Rewooled", message, 0);
        return 0;
    }
    SDL_DestroyProcess(process); /* the handle only: the game keeps running */
    return 1;
}

/* ---- the game's data: a folder (--game-dir) or a disc image, picked with the system's dialogs ---- */

/* the dialogs answer on any thread, at any time: the answer comes back to the main loop as this event, data1 the
 * chosen path (SDL_strdup'd) or 0 when the dialog was cancelled or failed, code the PICKED_ kind of dialog */
static Uint32 s_pathPickedEvent;
static Uint32 s_updateEvent; /* update_check.h: a newer release, data1 an UpdateCheckResult */

enum { PICKED_FOLDER, PICKED_FILE, PICKED_OUTPUT_DIR };

/* userdata: the PICKED_ kind, passed on as the event's code */
static void SDLCALL Launcher_OnPathPicked(void *userdata, const char *const *filelist, int filter) {
    (void) filter;
    SDL_Event e;
    SDL_zero(e);
    e.type = s_pathPickedEvent;
    e.user.code = (Sint32) (intptr_t) userdata;
    e.user.data1 = filelist && filelist[0] ? SDL_strdup(filelist[0]) : 0;
    if (!filelist)
        SDL_Log("SheepLauncher: file dialog: %s", SDL_GetError());
    SDL_PushEvent(&e);
}

/* where the game looks for its data without --game-dir: its own folder, or the folder holding the macOS app bundle
 * (as Platform_GetBasePath, src/platform/platform_sdl.cpp) */
std::string Launcher_DefaultGameDir() {
    const char *base = SDL_GetBasePath();
    std::string dir = base ? base : "";
    size_t bundle = dir.find(".app/Contents/");
    if (bundle != std::string::npos)
        dir.erase(dir.rfind('/', bundle) + 1);
    return dir;
}

/* whether dir holds the game's data: a Levels folder, in any case (a disc may have LEVELS) */
bool Launcher_HasGameData(const char *dir) {
    int count = 0;
    char **found = SDL_GlobDirectory(dir, "levels", SDL_GLOB_CASEINSENSITIVE, &count);
    SDL_free(found);
    return count > 0;
}

/* text cut from the left with "..." so that it fits width */
std::string Launcher_FitLeft(const char *text, float width) {
    std::string s = text;
    if (ImGui::CalcTextSize(s.c_str()).x <= width)
        return s;
    while (s.size() > 1 && ImGui::CalcTextSize(("..." + s).c_str()).x > width)
        s.erase(0, 1);
    return "..." + s;
}

/* opens the system's folder dialog for the game folder; its answer comes back to main as s_pathPickedEvent */
void Launcher_PickFolder(LauncherState &s) {
    s.picking = true;
    SDL_ShowOpenFolderDialog(Launcher_OnPathPicked, (void *) (intptr_t) PICKED_FOLDER, s.window,
                             s.options.exeDir[0] ? s.options.exeDir : 0, false);
}

/* opens the system's folder dialog for where a disc image is extracted to; its answer comes back as s_pathPickedEvent */
void Launcher_PickOutputDir(LauncherState &s) {
    s.picking = true;
    SDL_ShowOpenFolderDialog(Launcher_OnPathPicked, (void *) (intptr_t) PICKED_OUTPUT_DIR, s.window,
                             s.outputDir.empty() ? 0 : s.outputDir.c_str(), false);
}

/* opens the system's file dialog for a disc image; its answer comes back to main as s_pathPickedEvent */
void Launcher_PickDiscImage(LauncherState &s) {
    /* read by the dialog until it answers, so not on the stack */
    static const SDL_DialogFileFilter filters[] = {
            {"Disc image (.cue, .bin, .iso)", "cue;bin;iso"},
    };
    s.picking = true;
    SDL_ShowOpenFileDialog(Launcher_OnPathPicked, (void *) (intptr_t) PICKED_FILE, s.window, filters,
                           (int) (sizeof(filters) / sizeof(filters[0])), 0, false);
}

/* a disc image's kind, from its extension (the file dialog only offers .cue, .bin and .iso): a .bin is a CUE/BIN's */
static GameSourceKind Launcher_DiscImageKind(const char *path) {
    const char *dot = SDL_strrchr(path, '.');
    return dot && SDL_strcasecmp(dot, ".iso") == 0 ? GAME_SOURCE_ISO : GAME_SOURCE_CUE;
}

/* the raw image a .cue names on its first FILE line (FILE "name.bin" BINARY), beside the .cue; a .bin itself */
std::string Launcher_DiscBinPath(const std::string &path) {
    size_t dot = path.rfind('.');
    if (dot == std::string::npos || SDL_strcasecmp(path.c_str() + dot, ".cue") != 0)
        return path;
    size_t size = 0;
    char *text = (char *) SDL_LoadFile(path.c_str(), &size);
    if (!text)
        return "";
    std::string name;
    for (char *line = text; line && *line && name.empty();) {
        char *end = SDL_strchr(line, '\n');
        if (end)
            *end = 0;
        while (*line == ' ' || *line == '\t')
            line++;
        if (SDL_strncasecmp(line, "FILE ", 5) == 0) {
            const char *p = line + 5, *q;
            if (*p == '"' && (q = SDL_strchr(p + 1, '"')))
                name.assign(p + 1, q);
            else if ((q = SDL_strchr(p, ' ')))
                name.assign(p, q);
        }
        line = end ? end + 1 : 0;
    }
    SDL_free(text);
    if (name.empty())
        return "";
    size_t slash = path.find_last_of("/\\");
    return slash == std::string::npos ? name : path.substr(0, slash + 1) + name;
}

/* whether path looks usable as kind: a quick look, on the UI thread; the extraction does the real reading */
bool Launcher_CheckSource(GameSourceKind kind, const char *path) {
    if (kind == GAME_SOURCE_FOLDER)
        return Launcher_HasGameData(path);
    SDL_PathInfo info;
    if (!SDL_GetPathInfo(path, &info) || info.type != SDL_PATHTYPE_FILE)
        return false;
    switch (kind) {
        case GAME_SOURCE_CUE: { /* the raw image is there: the .bin itself, or the one the .cue names */
            std::string bin = Launcher_DiscBinPath(path);
            return !bin.empty() && SDL_GetPathInfo(bin.c_str(), &info) && info.type == SDL_PATHTYPE_FILE;
        }
        case GAME_SOURCE_ISO: { /* an ISO9660 volume: "CD001" in sector 16 of 2048 bytes */
            char id[5] = {0};
            SDL_IOStream *io = SDL_IOFromFile(path, "rb");
            bool found = io && SDL_SeekIO(io, 16 * 2048 + 1, SDL_IO_SEEK_SET) >= 0 && SDL_ReadIO(io, id, 5) == 5 &&
                         SDL_memcmp(id, "CD001", 5) == 0;
            if (io)
                SDL_CloseIO(io);
            return found;
        }
        default:
            return false;
    }
}

/* the banner's height on a window w wide: it fills the width at 16:9 */
float Launcher_BannerHeight(float w) {
    return w * 9.0f / 16.0f;
}

/* the large font, loaded with the default one in main */
static ImFont *s_bigFont;

/* text to be noticed: large, bold (drawn twice, a pixel apart: the font has no bold) and amber, wrapped to the width */
void Launcher_Warning(const char *text) {
    ImGui::PushFont(s_bigFont);
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const float wrap = ImGui::GetContentRegionAvail().x - 1;
    const ImU32 colour = ImGui::GetColorU32(ImVec4(1, 0.72f, 0.25f, 1));
    ImDrawList *draw = ImGui::GetWindowDrawList();
    draw->AddText(0, 0, pos, colour, text, 0, wrap);
    draw->AddText(0, 0, ImVec2(pos.x + 1, pos.y), colour, text, 0, wrap);
    ImGui::Dummy(ImVec2(wrap + 1, ImGui::CalcTextSize(text, 0, false, wrap).y));
    ImGui::PopFont();
}

/* the banner, edge to edge at the top of the window (the part that moves it: Launcher_HitTest) */
float Launcher_DrawBanner(const LauncherState &s) {
    const ImVec2 size = ImGui::GetIO().DisplaySize;
    const float height = Launcher_BannerHeight(size.x);
    if (s.banner) {
        ImDrawList *draw = ImGui::GetWindowDrawList();
        draw->PushClipRect(ImVec2(0, 0), size); /* past the window's padding, which clips by default */
        draw->AddImage((ImTextureID) s.banner, ImVec2(0, 0), ImVec2(size.x, height));
        draw->PopClipRect();
    }
    return height;
}

/* whether the first launch's setup is needed: the files the launcher cannot work without are missing */
static bool Launcher_NeedsSetup(const LauncherState &s) {
    /* TODO: the other files to check */
    //return true;
    return !s.hasGameData;
}

/* the window has no title bar, so the banner moves it */
static SDL_HitTestResult Launcher_HitTest(SDL_Window *window, const SDL_Point *point, void *) {
    int w = 0, h = 0;
    SDL_GetWindowSize(window, &w, &h);
    return point->y < Launcher_BannerHeight((float) w) ? SDL_HITTEST_DRAGGABLE : SDL_HITTEST_NORMAL;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sheep, Dog 'n' Wolf: Rewooled", SDL_GetError(), 0);
        return 1;
    }
    SDL_Window *window;
    SDL_Renderer *renderer;
    s_pathPickedEvent = SDL_RegisterEvents(1);
    s_updateEvent = SDL_RegisterEvents(1);
    if (!SDL_CreateWindowAndRenderer("Sheep, Dog 'n' Wolf: Rewooled " SDW_VERSION, 460, 631,
                                     SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_BORDERLESS, &window, &renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sheep, Dog 'n' Wolf: Rewooled", SDL_GetError(), 0);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *bannerTexture = nullptr;
    if (SDL_Surface *banner = SDL_LoadPNG_IO(SDL_IOFromConstMem(g_sdwBannerPng, sizeof(g_sdwBannerPng)), true)) {
        bannerTexture = SDL_CreateTextureFromSurface(renderer, banner);
        SDL_DestroySurface(banner);
    }
    if (bannerTexture)
        SDL_SetTextureScaleMode(bannerTexture, SDL_SCALEMODE_LINEAR);
    else
        SDL_Log("banner: %s", SDL_GetError());
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetWindowHitTest(window, Launcher_HitTest, 0);
    App_SetWindowIcon(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = 0; /* nothing to remember: the options are saved on Play */
    ImGui::StyleColorsDark();
    {
        /* the font rasterised at the screen's pixel density, drawn at its logical size */
        float density = SDL_GetWindowPixelDensity(window);
        ImFontConfig font;
        font.FontDataOwnedByAtlas = false; /* the embedded array, never freed */
        io.Fonts->AddFontFromMemoryTTF((void *) g_sdwLauncherFont, (int) sizeof(g_sdwLauncherFont),
                                       16.0f * (density > 0 ? density : 1.0f), &font);
        /* the same, larger, for warnings (Launcher_Warning) */
        s_bigFont = io.Fonts->AddFontFromMemoryTTF((void *) g_sdwLauncherFont, (int) sizeof(g_sdwLauncherFont),
                                                   20.0f * (density > 0 ? density : 1.0f), &font);
        io.FontGlobalScale = 1.0f / (density > 0 ? density : 1.0f);
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(16, 14);
        style.FramePadding = ImVec2(8, 5);
        style.ItemSpacing = ImVec2(8, 8);
        style.WindowRounding = 0;
        style.WindowBorderSize = 0;
        /* soft buttons: a faint fill and border that light up under the mouse */
        style.FrameBorderSize = 1;
        style.FrameRounding = 0;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.078f, 0.078f, 0.094f, 1);
        style.Colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.94f, 1);
        style.Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 0.10f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.16f, 1);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.23f, 1);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.24f, 0.29f, 1);
        /* progress bars (the setup's extraction): the Play button's pink on the buttons' fill */
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.80f, 0.22f, 0.42f, 1);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.16f, 1);
        /* the popups and their widgets in the same greys and pink, not Dear ImGui's blue */
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.23f, 1);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.29f, 1);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.12f, 1);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.13f, 0.16f, 1);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.13f, 0.16f, 1);
        style.Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.29f, 1);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.19f, 0.19f, 0.23f, 1);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.29f, 1);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.88f, 0.30f, 0.50f, 1);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.6f);
    }
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    LauncherState state = {};
    state.window = window;
    state.banner = bannerTexture;
    Options_SetDefaults(&state.options);
    Options_LoadSaved(&state.options);

    /* the update check runs while the launcher is open; a newer release shows under the buttons */
    if (state.options.checkUpdates)
        UpdateCheck_Start(SDW_REPOSITORY, SDW_VERSION, s_updateEvent);

    state.renderers = Launcher_AvailableRenderers();
    for (size_t i = 0; i < state.renderers.size(); i++)
        if (strcmp(state.renderers[i]->name, state.options.renderer) == 0)
            state.rendererIndex = (int) i;
    state.resolutions = Launcher_Resolutions(state.options);
    for (size_t i = 0; i < state.resolutions.size(); i++)
        if (state.resolutions[i].w == state.options.width && state.resolutions[i].h == state.options.height)
            state.resolutionIndex = (int) i;
    state.scenes = s_scenes;
    state.sceneCount = (int) (sizeof(s_scenes) / sizeof(s_scenes[0]));
    for (int i = 0; i < state.sceneCount; i++)
        if (s_scenes[i].id == state.options.startLevel)
            state.sceneIndex = i;
    state.fullscreen = state.options.fullscreen != 0;
    state.hasGameData = Launcher_HasGameData(state.options.exeDir[0] ? state.options.exeDir
                                                                      : Launcher_DefaultGameDir().c_str());
    state.sourceKind = GAME_SOURCE_FOLDER;
    state.sourcePath = state.options.exeDir[0] ? state.options.exeDir : Launcher_DefaultGameDir();
    state.sourceValid = state.sourceReady = state.hasGameData;
    state.outputDir = Save_Dir(); /* the extracted game beside the settings and saves */
    state.screen = Launcher_NeedsSetup(state) ? LAUNCHER_SCREEN_SETUP : LAUNCHER_SCREEN_MAIN;

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                running = false;
            if (e.type == s_updateEvent) {
                state.update = *(UpdateCheckResult *) e.user.data1;
                UpdateCheck_Free(e.user.data1);
                state.updateShown = true;
            }
            if (e.type == s_pathPickedEvent) {
                state.picking = false;
                if (e.user.data1) {
                    const char *path = (const char *) e.user.data1;
                    if (e.user.code == PICKED_OUTPUT_DIR) { /* where a disc image goes: not the source */
                        state.outputDir = path;
                    } else if (e.user.code == PICKED_FOLDER) {
                        state.sourcePath = path; /* the game folder itself */
                        SDL_strlcpy(state.options.exeDir, path, sizeof(state.options.exeDir));
                        state.hasGameData = Launcher_HasGameData(state.options.exeDir);
                        state.sourceKind = GAME_SOURCE_FOLDER;
                        state.sourceValid = state.sourceReady = state.hasGameData;
                    } else { /* a disc image, extracted into a game folder by the setup */
                        state.sourcePath = path;
                        state.sourceKind = Launcher_DiscImageKind(path);
                        state.sourceValid = Launcher_CheckSource(state.sourceKind, path);
                        state.sourceReady = false;
                    }
                    SDL_free(e.user.data1);
                }
            }
        }
        state.controllers = Launcher_Controllers();

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("launcher", 0,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        if (state.screen == LAUNCHER_SCREEN_SETUP)
            ScreenSetup_Draw(state);
        else
            ScreenMain_Draw(state);
        if (state.play || state.quit)
            running = false;

        ImGui::End();
        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, 20, 20, 24, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    UpdateCheck_Stop(); /* a check still waiting for GitHub ends here, before the game starts and SDL quits */
    Extract_Shutdown();

    if (state.play && !state.renderers.empty()) {
        strncpy(state.options.renderer, state.renderers[state.rendererIndex]->name, sizeof(state.options.renderer) - 1);
        state.options.width = state.resolutions[state.resolutionIndex].w;
        state.options.height = state.resolutions[state.resolutionIndex].h;
        state.options.fullscreen = state.fullscreen;
        state.options.startLevel = s_scenes[state.sceneIndex].id;
        Options_Save(&state.options);
        Launcher_StartGame(state.options);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    if (bannerTexture) {

        SDL_DestroyTexture(bannerTexture);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
