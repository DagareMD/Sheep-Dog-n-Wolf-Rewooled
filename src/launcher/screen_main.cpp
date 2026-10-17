/* The launcher's main screen: the banner, the settings' buttons (each opens a popup: popup_*.cpp), Play and Quit */
#include "launcher_state.h"
#include "imgui.h"

#include <stdio.h>

void ScreenMain_Draw(LauncherState &s) {
    const ImGuiIO &io = ImGui::GetIO();
    const float bannerHeight = Launcher_DrawBanner(s);

    /* below the banner, centred: the settings in a grid two wide (the last alone, centred), then Play and Quit */
    const ImVec2 gridButton(195, 40), mainButton(200, 42);
    const float gridGap = 30, rowGap = 30, gridToPlay = 44, playToQuit = 20;
    const float blockHeight = 3 * gridButton.y + 2 * rowGap + gridToPlay + 2 * mainButton.y + playToQuit;
    const float centre = io.DisplaySize.x * 0.5f;
    const float left = centre - gridGap * 0.5f - gridButton.x, right = centre + gridGap * 0.5f;
    float y = bannerHeight + (io.DisplaySize.y - bannerHeight - blockHeight) * 0.5f;

    /* the settings, each in its own popup (popup_*.cpp) */
    ImGui::SetCursorPos(ImVec2(left, y));
    if (ImGui::Button("Installation Settings", gridButton))
        PopupInstall_Open();
    ImGui::SetCursorPos(ImVec2(right, y));
    if (ImGui::Button("Video Settings", gridButton))
        PopupVideo_Open();
    y += gridButton.y + rowGap;
    ImGui::SetCursorPos(ImVec2(left, y));
    if (ImGui::Button("Language Export", gridButton))
        PopupLangExport_Open(s);
    ImGui::SetCursorPos(ImVec2(right, y));
    if (ImGui::Button("Advanced", gridButton))
        PopupAdvanced_Open();
    y += gridButton.y + rowGap;
    ImGui::SetCursorPos(ImVec2(centre - gridButton.x * 0.5f, y));
    if (ImGui::Button("Credits", gridButton))
        PopupCredits_Open();
    y += gridButton.y + gridToPlay;

    /* Enter and Escape act on the main screen only: in a popup, Escape closes the popup (popup_common.cpp) */
    const bool popupOpen = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
    ImGui::SetCursorPos(ImVec2(centre - mainButton.x * 0.5f, y));
    ImGui::BeginDisabled(s.renderers.empty());
    /* Play, the main action, in the logo's pink */
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.22f, 0.42f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.88f, 0.30f, 0.50f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.18f, 0.36f, 1));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
    if (ImGui::Button("Play", mainButton) || (!s.renderers.empty() && !popupOpen && ImGui::IsKeyPressed(ImGuiKey_Enter, false))) {
        s.play = true;
    }
    ImGui::PopStyleColor(5);
    ImGui::EndDisabled();
    y += mainButton.y + playToQuit;
    ImGui::SetCursorPos(ImVec2(centre - mainButton.x * 0.5f, y));
    if (ImGui::Button("Quit", mainButton) || (!popupOpen && ImGui::IsKeyPressed(ImGuiKey_Escape, false)))
        s.quit = true;
    ImGui::PopStyleVar();

    if (s.updateShown) { /* "v1.0.8 available  [Download] [x]", centred under Quit */
        char text[64];
        snprintf(text, sizeof(text), "%s available", s.update.version);
        const ImGuiStyle &style = ImGui::GetStyle();
        float width = ImGui::CalcTextSize(text).x + ImGui::CalcTextSize("Download").x +
                      ImGui::CalcTextSize("x").x + 4 * style.FramePadding.x + 2 * style.ItemSpacing.x;
        ImGui::SetCursorPos(ImVec2(centre - width * 0.5f, io.DisplaySize.y - ImGui::GetFrameHeight() - 12));
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(ImVec4(0.45f, 0.85f, 0.45f, 1), "%s", text);
        ImGui::SameLine();
        if (ImGui::SmallButton("Download"))
            SDL_OpenURL(s.update.url);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", s.update.url);
        ImGui::SameLine();
        if (ImGui::SmallButton("x"))
            s.updateShown = false;
    }

    /* the popups, after the main screen: each draws nothing while closed */
    PopupInstall_Draw(s);
    PopupVideo_Draw(s);
    PopupCredits_Draw();
    PopupAdvanced_Draw(s);
    PopupLangExport_Draw(s);
}
