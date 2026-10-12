/* The launcher's Video Settings popup (launcher_state.h): the renderer, the resolution and full screen */
#include "launcher_state.h"
#include "imgui.h"

#include <stdio.h>

static const char *const s_id = "Video Settings";

void PopupVideo_Open() {
    ImGui::OpenPopup(s_id);
}

void PopupVideo_Draw(LauncherState &s) {
    if (!Popup_Begin(s_id))
        return;

    Popup_Label("Renderer");
    if (s.renderers.empty()) {
        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "none can run here");
    } else if (ImGui::BeginCombo("##renderer", s.renderers[s.rendererIndex]->label)) {
        for (size_t i = 0; i < s.renderers.size(); i++)
            if (ImGui::Selectable(s.renderers[i]->label, (int) i == s.rendererIndex))
                s.rendererIndex = (int) i;
        ImGui::EndCombo();
    }

    char label[32];
    snprintf(label, sizeof(label), "%d x %d", s.resolutions[s.resolutionIndex].w, s.resolutions[s.resolutionIndex].h);
    Popup_Label("Resolution");
    if (ImGui::BeginCombo("##resolution", label)) {
        for (size_t i = 0; i < s.resolutions.size(); i++) {
            snprintf(label, sizeof(label), "%d x %d", s.resolutions[i].w, s.resolutions[i].h);
            if (ImGui::Selectable(label, (int) i == s.resolutionIndex))
                s.resolutionIndex = (int) i;
        }
        ImGui::EndCombo();
    }

    Popup_Label("Full screen");
    ImGui::Checkbox("##fullscreen", &s.fullscreen);

    Popup_End();
}
