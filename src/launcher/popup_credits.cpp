/* The launcher's Credits popup (launcher_state.h): res/credits.txt, embedded at build time */
#include "launcher_state.h"
#include "imgui.h"
#include "sdw_credits_txt.h" /* g_sdwCreditsTxt[]: res/credits.txt (generated in the build folder; not 0-terminated) */

static const char *const s_id = "Credits";

void PopupCredits_Open() {
    ImGui::OpenPopup(s_id);
}

void PopupCredits_Draw() {
    if (!Popup_Begin(s_id))
        return;
    /* a fixed height, scrolling when the text is longer */
    ImGui::BeginChild("credits", ImVec2(0, 260));
    ImGui::PushTextWrapPos(0);
    const char *text = (const char *) g_sdwCreditsTxt;
    ImGui::TextUnformatted(text, text + sizeof(g_sdwCreditsTxt));
    ImGui::PopTextWrapPos();
    ImGui::EndChild();
    Popup_End();
}
