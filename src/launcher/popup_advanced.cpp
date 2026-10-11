/* The launcher's Advanced popup (launcher_state.h): the scene the game starts in */
#include "launcher_state.h"
#include "imgui.h"

static const char *const s_id = "Advanced";

void PopupAdvanced_Open() {
    ImGui::OpenPopup(s_id);
}

void PopupAdvanced_Draw(LauncherState &s) {
    if (!Popup_Begin(s_id))
        return;

    Popup_Label("Skip to");
    if (ImGui::BeginCombo("##scene", s.scenes[s.sceneIndex].label)) {
        for (int i = 0; i < s.sceneCount; i++)
            if (ImGui::Selectable(s.scenes[i].label, i == s.sceneIndex))
                s.sceneIndex = i;
        ImGui::EndCombo();
    }

    Popup_End();
}
