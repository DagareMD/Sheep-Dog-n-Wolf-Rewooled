/* The launcher's Language Export popup (launcher_state.h): one of the game's languages written out as a language pack
 * (docs/language-packs.md), from the game's files: a game folder, or a disc image read in place (langexport.h). The
 * export runs on a thread of its own: the page waits for it, and so does the launcher before it exits. */
#include "launcher_state.h"
#include "langexport.h"
#include "imgui.h"

#include <atomic>
#include <filesystem>
#include <string>
#include <thread>
#include <vector>

static const char *const s_id = "Language Export";

namespace {

struct ExportPage {
    std::string source;              /* the game's files read: a folder or a disc image ("" none yet) */
    bool sourceOk = false;
    std::string sourceError;
    std::vector<std::string> names;  /* its languages */
    int language = 0;
    char packName[128] = "";
    bool includeVoices = true;
    bool includeTextures = true;
    std::string status;
    bool statusIsError = false;

    std::thread worker;
    std::atomic<bool> running{false};
    bool wrote = false;              /* the worker's result, read once it has stopped */
    char error[1024] = "";
    std::string writtenTo;
};

ExportPage s_page;

/* the game's files at path read, and their languages listed */
void Page_Open(const std::string &path)
{
    s_page.source = path;
    s_page.names.clear();
    s_page.language = 0;
    s_page.status.clear();
    char error[1024];
    s_page.sourceOk = Export_OpenSource(path.c_str(), error, sizeof(error));
    s_page.sourceError = s_page.sourceOk ? "" : error;
    if (!s_page.sourceOk)
        return;
    for (s32 i = 0; i < Export_Count(); i++)
        s_page.names.push_back(Export_NameUtf8(i));
    SDL_snprintf(s_page.packName, sizeof(s_page.packName), "%s", s_page.names.empty() ? "" : s_page.names[0].c_str());
}

/* the export, on the worker thread */
void Page_Run(int language, std::string to, std::string name, bool voices, bool textures)
{
    s_page.wrote =
        Export_Language(language, to.c_str(), name.c_str(), voices, textures, s_page.error, sizeof(s_page.error));
    s_page.writtenTo = to;
    s_page.running = false;
}

/* the worker's result, once it has stopped */
void Page_Collect()
{
    if (s_page.running || !s_page.worker.joinable())
        return;
    s_page.worker.join();
    s_page.status = s_page.wrote ? "Written to " + s_page.writtenTo : std::string(s_page.error);
    s_page.statusIsError = !s_page.wrote;
}

} // namespace

void PopupLangExport_Open(LauncherState &s)
{
    /* the game folder the launcher plays, until another source is chosen; packs are written in its Languages */
    std::string game = s.options.exeDir[0] ? s.options.exeDir : Launcher_DefaultGameDir();
    while (game.size() > 1 && (game.back() == '/' || game.back() == '\\'))
        game.pop_back();
    if (s.exportOutput.empty())
        s.exportOutput = game + "/Languages";
    if (s_page.source.empty() && !s_page.running)
        Page_Open(game);
    ImGui::OpenPopup(s_id);
}

void PopupLangExport_Draw(LauncherState &s)
{
    Page_Collect();
    if (s.exportSourceChanged && !s_page.running) {
        s.exportSourceChanged = false;
        Page_Open(s.exportSource);
    }
    const bool running = s_page.running;
    if (!Popup_Begin(s_id, !running))
        return;

    ImGui::TextDisabled("Game's files");
    ImGui::TextUnformatted(
        Launcher_FitLeft(s_page.source.empty() ? "(none chosen)" : s_page.source.c_str(), ImGui::GetContentRegionAvail().x)
            .c_str());
    if (!s_page.sourceOk && !s_page.sourceError.empty())
        ImGui::TextColored(ImVec4(1, 0.75f, 0.3f, 1), "%s", s_page.sourceError.c_str());
    const float half = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
    ImGui::BeginDisabled(s.picking || running);
    if (ImGui::Button("Select Game Folder", ImVec2(half, 0)))
        Launcher_PickExportFolder(s);
    ImGui::SameLine();
    if (ImGui::Button("Select Game ISO", ImVec2(half, 0)))
        Launcher_PickExportIso(s);
    ImGui::EndDisabled();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginDisabled(!s_page.sourceOk || running);
    /* the labels' column: as wide as the longest of them */
    const float labels = ImGui::GetCursorPosX() + ImGui::CalcTextSize("Include textures").x + ImGui::GetStyle().ItemSpacing.x * 2;
    Popup_Label("Language", labels);
    if (ImGui::BeginCombo("##language", s_page.names.empty() ? "" : s_page.names[s_page.language].c_str())) {
        for (int i = 0; i < (int)s_page.names.size(); i++)
            if (ImGui::Selectable(s_page.names[i].c_str(), i == s_page.language)) {
                s_page.language = i;
                SDL_snprintf(s_page.packName, sizeof(s_page.packName), "%s", s_page.names[i].c_str());
            }
        ImGui::EndCombo();
    }
    Popup_Label("Pack name", labels);
    ImGui::InputText("##packname", s_page.packName, sizeof(s_page.packName));
    Popup_Label("Include voices", labels);
    ImGui::Checkbox("##voices", &s_page.includeVoices);
    Popup_Label("Include textures", labels);
    ImGui::Checkbox("##textures", &s_page.includeTextures);
    ImGui::Spacing();
    ImGui::TextDisabled("Export to...");
    ImGui::TextUnformatted(Launcher_FitLeft(s.exportOutput.c_str(), ImGui::GetContentRegionAvail().x).c_str());
    ImGui::BeginDisabled(s.picking);
    if (ImGui::Button("Choose...", ImVec2(-1, 0)))
        Launcher_PickExportOutput(s);
    ImGui::EndDisabled();
    ImGui::Spacing();
    if (ImGui::Button(running ? "Exporting..." : "Export", ImVec2(-1, 0))) {
        if (!s_page.packName[0]) {
            s_page.status = "Give the pack a name.";
            s_page.statusIsError = true;
        } else {
            s_page.status.clear();
            s_page.running = true;
            s_page.worker = std::thread(Page_Run, s_page.language, s.exportOutput + "/" + s_page.packName,
                                        std::string(s_page.packName), s_page.includeVoices, s_page.includeTextures);
        }
    }
    ImGui::EndDisabled();

    if (!s_page.status.empty()) {
        ImGui::Spacing();
        if (s_page.statusIsError)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.45f, 1));
        ImGui::TextWrapped("%s", s_page.status.c_str());
        if (s_page.statusIsError)
            ImGui::PopStyleColor();
    }
    Popup_End(!running);
}

void PopupLangExport_Shutdown()
{
    if (s_page.worker.joinable())
        s_page.worker.join();
}
