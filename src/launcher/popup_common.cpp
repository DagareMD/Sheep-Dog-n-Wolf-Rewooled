/* The frame every launcher popup shares (launcher_state.h) */
#include "launcher_state.h"
#include "imgui.h"

/* true while the popup is open: draw its contents, then call Popup_End */
bool Popup_Begin(const char *id, bool canClose) {
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always,
                            ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 60, 0)); /* the launcher's width less a margin; as tall as needed */
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18, 16));
    bool open = ImGui::BeginPopupModal(id, 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::PopStyleVar(2);
    if (open && canClose && ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        ImGui::CloseCurrentPopup();
    return open;
}

/* a setting's row: its label, then the next widget filling the rest of the line, all rows' widgets lined up */
void Popup_Label(const char *label) {
    const float labelWidth = 110;
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(-1);
}

void Popup_End(bool canClose) {
    ImGui::Spacing();
    ImGui::BeginDisabled(!canClose);
    if (ImGui::Button("Close", ImVec2(-1, 0)))
        ImGui::CloseCurrentPopup();
    ImGui::EndDisabled();
    ImGui::EndPopup();
}
