/* The first launch's setup: a wizard, one page at a time under the banner, shown instead of the main screen while
 * Launcher_NeedsSetup (launcher.cpp) finds files missing. Finish saves the choices and goes to the main screen. */
#include "launcher_state.h"
#include "imgui.h"

#include <string>

/* the extract page starts an extraction the first time it shows after the install directory page */
static bool s_extractStarted;

enum SetupStep {
    SETUP_WELCOME,
    SETUP_GAME_FOLDER,
    SETUP_OUTPUT_DIR,
    SETUP_EXTRACT,
    SETUP_DONE,
    SETUP_STEP_COUNT
};

static const char *const s_titles[SETUP_STEP_COUNT] = {
        "Welcome",
        "Game data",
        "Install directory",
        "Preparing the game",
        "All set",
};

/* whether the page is shown for the choices so far: the install directory and the extraction only for a disc image */
static bool Setup_PageApplies(const LauncherState &s, int step) {
    switch (step) {
        case SETUP_OUTPUT_DIR:
        case SETUP_EXTRACT:
            return s.sourceKind != GAME_SOURCE_FOLDER;
        default:
            return true;
    }
}

/* the next (dir 1) or previous (dir -1) page that applies; Back and Next skip the others */
static int Setup_Step(const LauncherState &s, int dir) {
    int step = s.setupStep + dir;
    while (step > 0 && step < SETUP_STEP_COUNT - 1 && !Setup_PageApplies(s, step))
        step += dir;
    return step;
}

/* whether Next can be pressed on this page: what the page asks for is done */
static bool Setup_CanContinue(const LauncherState &s) {
    switch (s.setupStep) {
        case SETUP_GAME_FOLDER:
            return s.sourceValid;
        case SETUP_OUTPUT_DIR:
            return !s.outputDir.empty() && !s.picking;
        case SETUP_EXTRACT:
            return s.sourceReady;
        default:
            return true;
    }
}

static void Setup_Welcome(LauncherState &s) {
    (void) s;
    ImGui::TextWrapped("Welcome to Sheep, Dog 'n' Wolf: Rewooled\nTo play Rewooled, you will need to provide your own files, either bin/cue, CD drive, or game folder");
}

static void Setup_GameFolder(LauncherState &s) {
    Launcher_Warning("PSX disc support is highly experimental and not fully tested.");
    ImGui::Spacing();
    ImGui::TextWrapped("Choose the folder/or file with the game's data.");
    ImGui::Spacing();

    /* what is picked: its kind, then its path */
    static const char *const kinds[] = {"Folder", "CUE/BIN image", "ISO image"};
    ImGui::TextDisabled("%s", kinds[s.sourceKind]);
    ImGui::TextUnformatted(Launcher_FitLeft(s.sourcePath.c_str(), ImGui::GetContentRegionAvail().x).c_str());
    if (!s.sourceValid) {
        static const char *const problems[] = {
                "No Levels folder here: not the game's data?",
                "Not a usable CUE/BIN image",
                "Not a usable ISO image",
        };
        ImGui::TextColored(ImVec4(1, 0.75f, 0.3f, 1), "%s", problems[s.sourceKind]);
    }
    ImGui::Spacing();

    /* a folder or a disc image: two buttons sharing the width */
    const float width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
    ImGui::BeginDisabled(s.picking);
    if (ImGui::Button("Folder...", ImVec2(width, 0)))
        Launcher_PickFolder(s);
    ImGui::SameLine();
    if (ImGui::Button("Disc image...", ImVec2(width, 0)))
        Launcher_PickDiscImage(s);
    ImGui::EndDisabled();
}

/* a disc image's files into a game folder: the PS1 version's renamed and converted. When done: options.exeDir the
 * folder, hasGameData and sourceReady set (Next waits for sourceReady) */
static void Setup_Extract(LauncherState &s) {
    if (!s_extractStarted) {
        s_extractStarted = true;
        Extract_Start(s);
    }
    Extract_Draw(s);
}

/* where the disc image's files go (Music/, Voices/ and Levels/ inside): Save_Dir until another is picked */
static void Setup_SelectOutput(LauncherState &s) {
    ImGui::TextWrapped("Choose where to install the game's files. By default they go beside the settings and saves.");
    ImGui::Spacing();
    ImGui::TextUnformatted(Launcher_FitLeft(s.outputDir.c_str(), ImGui::GetContentRegionAvail().x).c_str());
    ImGui::Spacing();
    ImGui::BeginDisabled(s.picking);
    if (ImGui::Button("Change...", ImVec2(-1, 0)))
        Launcher_PickOutputDir(s);
    ImGui::EndDisabled();
}

/* on leaving the install directory page for the extraction: the extract page starts a new one, with the choices */
static void Setup_ApplyOutput() {
    s_extractStarted = false;
}

static void Setup_Done(LauncherState &s) {
    (void) s;
    ImGui::TextWrapped("TODO: what was set up; the settings can be changed later from the main screen");
}

void ScreenSetup_Draw(LauncherState &s) {
    const ImGuiIO &io = ImGui::GetIO();
    const float bannerHeight = Launcher_DrawBanner(s);
    const float margin = 30;
    const ImVec2 navButton(120, 40);

    /* the page: its title and step, then its contents, in a region between the margins (its own width is what the
     * pages' widgets fill and wrap to) */
    const float top = bannerHeight + 24, bottom = io.DisplaySize.y - navButton.y - margin - 16;
    ImGui::SetCursorPos(ImVec2(margin, top));
    ImGui::BeginChild("page", ImVec2(io.DisplaySize.x - 2 * margin, bottom - top));
    ImGui::PushTextWrapPos(0); /* the region's right edge */
    ImGui::TextDisabled("Setup  %d / %d", s.setupStep + 1, (int) SETUP_STEP_COUNT);
    ImGui::TextUnformatted(s_titles[s.setupStep]);
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(-1);
    switch (s.setupStep) {
        case SETUP_WELCOME:
            Setup_Welcome(s);
            break;
        case SETUP_GAME_FOLDER:
            Setup_GameFolder(s);
            break;
        case SETUP_OUTPUT_DIR:
            Setup_SelectOutput(s);
            break;
        case SETUP_EXTRACT:
            Setup_Extract(s);
            break;
        case SETUP_DONE:
            Setup_Done(s);
            break;
    }
    ImGui::PopItemWidth();
    ImGui::PopTextWrapPos();
    ImGui::EndChild();

    /* Back (Quit on the first page) at the bottom left, Next (Finish on the last) at the bottom right */
    const float y = io.DisplaySize.y - navButton.y - margin;
    ImGui::SetCursorPos(ImVec2(margin, y));
    if (s.setupStep == 0) {
        if (ImGui::Button("Quit", navButton))
            s.quit = true;
    } else {
        ImGui::BeginDisabled(Extract_Running());
        if (ImGui::Button("Back", navButton))
            s.setupStep = Setup_Step(s, -1);
        ImGui::EndDisabled();
    }
    ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - margin - navButton.x, y));
    const bool last = s.setupStep == SETUP_STEP_COUNT - 1;
    ImGui::BeginDisabled(!Setup_CanContinue(s));
    if (ImGui::Button(last ? "Finish" : "Next", navButton)) {
        if (last) {
            Options_Save(&s.options); /* the game folder, so the next launch goes straight to the main screen */
            s.screen = LAUNCHER_SCREEN_MAIN;
        } else {
            if (s.setupStep == SETUP_OUTPUT_DIR)
                Setup_ApplyOutput();
            s.setupStep = Setup_Step(s, 1);
        }
    }
    ImGui::EndDisabled();
}
