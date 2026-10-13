/* A disc image's extraction into a game folder (launcher_state.h): extractXaAudio (xa_extract.h) on a worker thread,
 * its progress drawn by the setup's extract page and the Installation Settings popup. One at a time. */
#include "launcher_state.h"
#include "imgui.h"
#include "xa_extract.h"

#include <string>
#include <thread>

ExtractionSettings g_settings;

static std::thread s_worker;    /* joined once the extraction has finished (Extract_Draw) or on exit */
static std::string s_jobSource; /* the disc image the last extraction was started for; "" before the first */

void Extract_Start(LauncherState &s) {
    if (Extract_Running())
        return;
    if (s_worker.joinable())
        s_worker.join();
    g_settings.binPath = Launcher_DiscBinPath(s.sourcePath);
    g_settings.outputDir = s.outputDir;
    g_settings.cancelRequested = false;
    g_settings.status = ExtractionStatus::Idle;
    g_settings.progress = 0.0f;
    g_settings.filesTotal = g_settings.filesWritten = g_settings.filesLeft = 0;
    s_jobSource = s.sourcePath;
    s.sourceReady = false;
    s_worker = std::thread([] { extractXaAudio(&g_settings); });
}

bool Extract_Running() {
    return s_worker.joinable() && !g_settings.finished();
}

bool Extract_StartedFor(const std::string &sourcePath) {
    return !s_jobSource.empty() && s_jobSource == sourcePath;
}

void Extract_Draw(LauncherState &s) {
    /* finished: the thread ends, and a successful extraction becomes the game folder, saved at once (so a launcher
     * closed before the setup's Finish keeps it) */
    if (s_worker.joinable() && g_settings.finished()) {
        s_worker.join();
        if (g_settings.status == ExtractionStatus::Done) {
            SDL_strlcpy(s.options.exeDir, g_settings.outputDir.string().c_str(), sizeof(s.options.exeDir));
            s.hasGameData = Launcher_HasGameData(s.options.exeDir);
            s.sourceReady = true;
            Options_Save(&s.options);
        }
    }

    const ExtractionStatus status = g_settings.status;
    switch (status) {
        case ExtractionStatus::Done:
            ImGui::TextWrapped("Done: %d files written.", g_settings.filesWritten.load());
            ImGui::ProgressBar(1.0f, ImVec2(-1, 0));
            return;
        case ExtractionStatus::Cancelled:
        case ExtractionStatus::Failed:
            if (status == ExtractionStatus::Cancelled)
                ImGui::TextWrapped("Cancelled. The files written so far are kept.");
            else
                ImGui::TextColored(ImVec4(1, 0.45f, 0.4f, 1), "Failed: %s", g_settings.errorMessage().c_str());
            ImGui::Spacing();
            if (ImGui::Button("Try again", ImVec2(-1, 0)))
                Extract_Start(s);
            return;
        default:
            break;
    }

    /* running (Idle only until the thread's first step): the phase, the bar, the counts and the file being written */
    ImGui::TextUnformatted(status == ExtractionStatus::Extracting ? "Extracting the game's files..."
                                                                  : "Reading the disc...");
    ImGui::Spacing();
    char overlay[16];
    SDL_snprintf(overlay, sizeof(overlay), "%.0f%%", g_settings.progress * 100.0f);
    ImGui::ProgressBar(g_settings.progress, ImVec2(-1, 0), overlay);
    if (status == ExtractionStatus::Extracting) {
        ImGui::TextDisabled("%d / %d files, %d left", g_settings.filesWritten.load(), g_settings.filesTotal.load(),
                            g_settings.filesLeft.load());
        std::string file = g_settings.currentFile();
        ImGui::TextDisabled("%s", Launcher_FitLeft(file.c_str(), ImGui::GetContentRegionAvail().x).c_str());
    }
    ImGui::Spacing();
    ImGui::BeginDisabled(g_settings.cancelRequested);
    if (ImGui::Button("Cancel", ImVec2(-1, 0)))
        g_settings.cancelRequested = true;
    ImGui::EndDisabled();
}

/* on quitting with the extraction running: it stops at its next step, and the thread is joined before the launcher's
 * exit (a std::thread still running when destroyed ends the program) */
void Extract_Shutdown() {
    if (!s_worker.joinable())
        return;
    g_settings.cancelRequested = true;
    s_worker.join();
}
