/* The launcher's Video Settings popup (launcher_state.h): the renderer, the resolution, screen percent, anti-aliasing and full screen */
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

    /* Screen percent: the game renders at this share of the resolution above, then the picture is scaled up to the
     * window. Below 100 the scale-up is nearest-neighbour, so the pixels stay sharp. */
    Popup_Label("Screen percent");
    if (s.options.renderScale < 25 || s.options.renderScale > 200)
        s.options.renderScale = 100;
    ImGui::SliderInt("##renderscale", &s.options.renderScale, 25, 200, "%d%%");
    {
        const Resolution &r = s.resolutions[s.resolutionIndex];
        Popup_Label("");
        ImGui::TextDisabled("Renders at %d x %d", r.w * s.options.renderScale / 100, r.h * s.options.renderScale / 100);
    }

    /* Anti-aliasing: hardware multisampling; the game uses the GPU's best supported count up to the one chosen */
    static const int s_msaaValues[] = {0, 2, 4, 8};
    static const char *const s_msaaLabels[] = {"Off", "MSAA 2x", "MSAA 4x", "MSAA 8x"};
    int msaaIndex = 0;
    for (int i = 0; i < 4; i++)
        if (s.options.msaa == s_msaaValues[i])
            msaaIndex = i;
    Popup_Label("Anti-aliasing");
    if (ImGui::BeginCombo("##msaa", s_msaaLabels[msaaIndex])) {
        for (int i = 0; i < 4; i++)
            if (ImGui::Selectable(s_msaaLabels[i], i == msaaIndex))
                s.options.msaa = s_msaaValues[i];
        ImGui::EndCombo();
    }

    Popup_Label("Full screen");
    ImGui::Checkbox("##fullscreen", &s.fullscreen);

    Popup_End();
}
