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
#include "../platform/save_store.h"
#include "update_check.h"

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

struct RendererChoice {
    const char *name;    /* options.h's spelling */
    const char *label;   /* shown */
    const char *driver;  /* SDL_GPU's name */
};

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
struct SceneChoice {
    int id;
    const char *label;
};
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

struct Resolution {
    int w, h;

    bool operator==(const Resolution &o) const { return w == o.w && h == o.h; }
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
static void Launcher_ResetSaves() {
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

/* ---- the game folder (--game-dir): a folder picked with the system's dialog ---- */

/* the dialog answers on any thread, at any time: the answer comes back to the main loop as this event, data1 the
 * chosen path (SDL_strdup'd) or 0 when the dialog was cancelled or failed */
static Uint32 s_folderPickedEvent;
static Uint32 s_updateEvent; /* update_check.h: a newer release, data1 an UpdateCheckResult */

static void SDLCALL Launcher_OnFolderPicked(void *userdata, const char *const *filelist, int filter) {
    (void) userdata;
    (void) filter;
    SDL_Event e;
    SDL_zero(e);
    e.type = s_folderPickedEvent;
    e.user.data1 = filelist && filelist[0] ? SDL_strdup(filelist[0]) : 0;
    if (!filelist)
        SDL_Log("SheepLauncher: folder dialog: %s", SDL_GetError());
    SDL_PushEvent(&e);
}

/* where the game looks for its data without --game-dir: its own folder, or the folder holding the macOS app bundle
 * (as Platform_GetBasePath, src/platform/platform_sdl.cpp) */
static std::string Launcher_DefaultGameDir() {
    const char *base = SDL_GetBasePath();
    std::string dir = base ? base : "";
    size_t bundle = dir.find(".app/Contents/");
    if (bundle != std::string::npos)
        dir.erase(dir.rfind('/', bundle) + 1);
    return dir;
}

/* whether dir holds the game's data: a Levels folder, in any case (a disc may have LEVELS) */
static bool Launcher_HasGameData(const char *dir) {
    int count = 0;
    char **found = SDL_GlobDirectory(dir, "levels", SDL_GLOB_CASEINSENSITIVE, &count);
    SDL_free(found);
    return count > 0;
}

/* text cut from the left with "..." so that it fits width */
static std::string Launcher_FitLeft(const char *text, float width) {
    std::string s = text;
    if (ImGui::CalcTextSize(s.c_str()).x <= width)
        return s;
    while (s.size() > 1 && ImGui::CalcTextSize(("..." + s).c_str()).x > width)
        s.erase(0, 1);
    return "..." + s;
}

/* the banner's height on a window w wide: it fills the width at 16:9 */
static float Launcher_BannerHeight(float w) {
    return w * 9.0f / 16.0f;
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
    s_folderPickedEvent = SDL_RegisterEvents(1);
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
        font.SizePixels = 15.0f * (density > 0 ? density : 1.0f);
        io.Fonts->AddFontDefault(&font);
        io.FontGlobalScale = 1.0f / (density > 0 ? density : 1.0f);
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(16, 14);
        style.FramePadding = ImVec2(8, 5);
        style.ItemSpacing = ImVec2(8, 8);
        style.FrameRounding = 4;
        style.WindowRounding = 0;
    }
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    GameOptions options;
    Options_SetDefaults(&options);
    Options_LoadSaved(&options);

    /* the update check runs while the launcher is open; a newer release shows next to the title */
    UpdateCheckResult update = {};
    bool updateShown = false, checkUpdates = options.checkUpdates != 0, updateStarted = false;
    if (checkUpdates)
        updateStarted = UpdateCheck_Start(SDW_REPOSITORY, SDW_VERSION, s_updateEvent) != 0;

    std::vector<const RendererChoice *> renderers = Launcher_AvailableRenderers();
    int rendererIndex = 0;
    for (size_t i = 0; i < renderers.size(); i++)
        if (strcmp(renderers[i]->name, options.renderer) == 0)
            rendererIndex = (int) i;
    std::vector<Resolution> resolutions = Launcher_Resolutions(options);
    int resolutionIndex = 0;
    for (size_t i = 0; i < resolutions.size(); i++)
        if (resolutions[i].w == options.width && resolutions[i].h == options.height)
            resolutionIndex = (int) i;
    int sceneIndex = 0;
    for (int i = 0; i < (int) (sizeof(s_scenes) / sizeof(s_scenes[0])); i++)
        if (s_scenes[i].id == options.startLevel)
            sceneIndex = i;
    bool fullscreen = options.fullscreen != 0;
    bool pickingFolder = false;
    bool hasGameData = Launcher_HasGameData(options.exeDir[0] ? options.exeDir : Launcher_DefaultGameDir().c_str());

    bool running = true, play = false;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                running = false;
            if (e.type == s_updateEvent) {
                update = *(UpdateCheckResult *) e.user.data1;
                UpdateCheck_Free(e.user.data1);
                updateShown = true;
            }
            if (e.type == s_folderPickedEvent) {
                pickingFolder = false;
                if (e.user.data1) {
                    SDL_strlcpy(options.exeDir, (const char *) e.user.data1, sizeof(options.exeDir));
                    SDL_free(e.user.data1);
                    hasGameData = Launcher_HasGameData(options.exeDir);
                }
            }
        }
        std::vector<std::string> controllers = Launcher_Controllers();
        int controllerIndex = options.controller + 1; /* Keyboard is -1 */
        if (controllerIndex < 0 || controllerIndex >= (int) controllers.size())
            controllerIndex = 0;

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("launcher", 0,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        /* the banner, edge to edge above the padded options */
        const float bannerHeight = Launcher_BannerHeight(io.DisplaySize.x);
        if (bannerTexture) {
            ImDrawList *draw = ImGui::GetWindowDrawList();
            draw->PushClipRect(ImVec2(0, 0), io.DisplaySize); /* past the window's padding, which clips by default */
            draw->AddImage((ImTextureID) bannerTexture, ImVec2(0, 0), ImVec2(io.DisplaySize.x, bannerHeight));
            draw->PopClipRect();
        }
        ImGui::SetCursorPosY(bannerHeight + ImGui::GetStyle().WindowPadding.y);
        ImGui::TextUnformatted("Launch Options");
        if (updateShown) { /* "v1.0.8 available  [Download] [x]", on the title's line, at the right */
            char text[64];
            snprintf(text, sizeof(text), "%s available", update.version);
            const ImGuiStyle &style = ImGui::GetStyle();
            float width = ImGui::CalcTextSize(text).x + ImGui::CalcTextSize("Download").x +
                          ImGui::CalcTextSize("x").x + 4 * style.FramePadding.x + 2 * style.ItemSpacing.x;
            ImGui::SameLine(ImGui::GetWindowWidth() - style.WindowPadding.x - width);
            ImGui::TextColored(ImVec4(0.45f, 0.85f, 0.45f, 1), "%s", text);
            ImGui::SameLine();
            if (ImGui::SmallButton("Download"))
                SDL_OpenURL(update.url);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", update.url);
            ImGui::SameLine();
            if (ImGui::SmallButton("x"))
                updateShown = false;
        }
        ImGui::Separator();
        const float labelWidth = 120.0f;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Renderer");
        ImGui::SameLine(labelWidth);
        ImGui::SetNextItemWidth(-1);
        if (renderers.empty()) {
            ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "none can run here");
        } else if (ImGui::BeginCombo("##renderer", renderers[rendererIndex]->label)) {
            for (size_t i = 0; i < renderers.size(); i++)
                if (ImGui::Selectable(renderers[i]->label, (int) i == rendererIndex))
                    rendererIndex = (int) i;
            ImGui::EndCombo();
        }

        char label[32];
        snprintf(label, sizeof(label), "%d x %d", resolutions[resolutionIndex].w, resolutions[resolutionIndex].h);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Resolution");
        ImGui::SameLine(labelWidth);
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##resolution", label)) {
            for (size_t i = 0; i < resolutions.size(); i++) {
                snprintf(label, sizeof(label), "%d x %d", resolutions[i].w, resolutions[i].h);
                if (ImGui::Selectable(label, (int) i == resolutionIndex))
                    resolutionIndex = (int) i;
            }
            ImGui::EndCombo();
        }

        ImGui::SetCursorPosX(labelWidth);
        ImGui::Checkbox("Full screen", &fullscreen);
        ImGui::SameLine();
        if (ImGui::Checkbox("Check for updates", &checkUpdates)) {
            options.checkUpdates = checkUpdates;
            Options_SaveCheckUpdates(checkUpdates);
            if (checkUpdates && !updateStarted)
                updateStarted = UpdateCheck_Start(SDW_REPOSITORY, SDW_VERSION, s_updateEvent) != 0;
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("When the launcher opens, ask GitHub whether there is a newer release");

        /* the game folder: the game's data, a disc drive or a copy; empty = next to the launcher */
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Game folder");
        ImGui::SameLine(labelWidth);
        {
            const float browseWidth = 80.0f, resetWidth = 60.0f, spacing = ImGui::GetStyle().ItemSpacing.x;
            float pathWidth = ImGui::GetContentRegionAvail().x - browseWidth - spacing -
                              (options.exeDir[0] ? resetWidth + spacing : 0.0f);
            std::string shown =
                    options.exeDir[0] ? Launcher_FitLeft(options.exeDir, pathWidth) : std::string(
                            "next to the launcher");
            ImGui::AlignTextToFramePadding();
            if (options.exeDir[0])
                ImGui::TextUnformatted(shown.c_str());
            else
                ImGui::TextDisabled("%s", shown.c_str());
            if (options.exeDir[0] && ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", options.exeDir);
            ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - browseWidth -
                            (options.exeDir[0] ? resetWidth + spacing : 0.0f));
            ImGui::BeginDisabled(pickingFolder);
            if (ImGui::Button("Browse...", ImVec2(browseWidth, 0))) {
                pickingFolder = true;
                SDL_ShowOpenFolderDialog(Launcher_OnFolderPicked, 0, window, options.exeDir[0] ? options.exeDir : 0,
                                         false);
            }
            ImGui::EndDisabled();
            if (options.exeDir[0]) {
                ImGui::SameLine();
                if (ImGui::Button("Reset", ImVec2(resetWidth, 0))) {
                    options.exeDir[0] = 0;
                    hasGameData = Launcher_HasGameData(Launcher_DefaultGameDir().c_str());
                }
            }
        }
        if (!hasGameData) {
            ImGui::SetCursorPosX(labelWidth);
            ImGui::TextColored(ImVec4(1, 0.75f, 0.3f, 1), "no Levels folder here: not the game's data?");
        }

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Controller");
        ImGui::SameLine(labelWidth);
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##controller", controllers[controllerIndex].c_str())) {
            for (size_t i = 0; i < controllers.size(); i++)
                if (ImGui::Selectable(controllers[i].c_str(), (int) i == controllerIndex))
                    controllerIndex = (int) i;
            ImGui::EndCombo();
        }
        options.controller = controllerIndex - 1;

        //if (ImGui::CollapsingHeader("Developer")) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Skip to");
        ImGui::SameLine(labelWidth);
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##scene", s_scenes[sceneIndex].label)) {
            for (int i = 0; i < (int) (sizeof(s_scenes) / sizeof(s_scenes[0])); i++)
                if (ImGui::Selectable(s_scenes[i].label, i == sceneIndex))
                    sceneIndex = i;
            ImGui::EndCombo();
        }
        // }

        /* the buttons, on the window's bottom line */
        const float buttonHeight = ImGui::GetFrameHeight();
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - buttonHeight - ImGui::GetStyle().WindowPadding.y);
        if (ImGui::Button("Reset saves..."))
            ImGui::OpenPopup("Reset saves");
        ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 2 * 90 -
                        ImGui::GetStyle().ItemSpacing.x);
        if (ImGui::Button("Quit", ImVec2(90, 0)) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
            running = false;
        ImGui::SameLine();
        ImGui::BeginDisabled(renderers.empty());
        if (ImGui::Button("Play", ImVec2(90, 0)) ||
            (!renderers.empty() && ImGui::IsKeyPressed(ImGuiKey_Enter, false) && !ImGui::IsPopupOpen("Reset saves"))) {
            play = true;
            running = false;
        }
        ImGui::EndDisabled();

        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always,
                                ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Reset saves", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted("Delete the saved games and the game's settings\n(controls, volumes)?");
            if (ImGui::Button("Delete", ImVec2(90, 0))) {
                Launcher_ResetSaves();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(90, 0)))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::End();
        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, 20, 20, 24, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    UpdateCheck_Stop(); /* a check still waiting for GitHub ends here, before the game starts and SDL quits */

    if (play && !renderers.empty()) {
        strncpy(options.renderer, renderers[rendererIndex]->name, sizeof(options.renderer) - 1);
        options.width = resolutions[resolutionIndex].w;
        options.height = resolutions[resolutionIndex].h;
        options.fullscreen = fullscreen;
        options.startLevel = s_scenes[sceneIndex].id;
        Options_Save(&options);
        play = Launcher_StartGame(options) != 0;
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
