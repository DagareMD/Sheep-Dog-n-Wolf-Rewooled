
#include "sdw_types.h"
#include "../sdk/win32.h"
#define SDW_MEMBERS_TextResBank TextResBank();
#include "sdw_classes.h"
#include "../engine/bs_io.h"
#include "../sdk/crt.h"
#include "../engine/langpack.h"
#include "rewooled_menu_strings.h"
TextResBank::TextResBank()
{
    loaded = 0;
    count = 0;
    entries = 0;
}
TextResBank::~TextResBank()
{
    u16 a;
    for (a = 0; a < count; a++)
        if (entries[a].text)
            delete entries[a].text;
    if (entries)
        delete entries;
}
u16 TextResBank::Load(const char *path)
{
    char *a;
    char *b = 0;
    s32 c;
    s32 unused;
    a = 0;
    a = (char *)Bs_LoadFile(path, (u32 *)&c);
    if (!a) {
        char e[256] = "Could not load file: ";
        strcat(e, path);
        MessageBoxA(0, e, "BSM File Error", MB_ICONHAND);
        loaded = 0;
        count = 0;
        return count;
    }
    count = 0;
    loaded = 0;
    if (c > 0) {
        s32 f = 0;
        u32 g;
        u32 h;
        b = new char[12];
        strncpy(b, a, 12);
        if (strncmp(b, "GREETINGV1.0", 12)) {
            MessageBoxA(0, "Could not load file, check version", "BSM File Error", MB_ICONHAND);
        } else {
            f += 12;
            memcpy(&g, a + f, 4);
            entries = new TextResEntry[g];
            f += 4;
            for (h = 0; h < g; h++) {
                memcpy(&entries[h].key, a + f, 4);
                f += 4;
                memcpy(&entries[h].len, a + f, 4);
                f += 4;
                if (entries[h].len) {
                    entries[h].text = new char[entries[h].len];
                    strncpy(entries[h].text, a + f, entries[h].len);
                    f += entries[h].len;
                } else
                    entries[h].text = 0;
            }
            loaded = 1;
            count = (u16)g;
        }
    }
    if (b)
        delete b;
    if (a)
        delete a;
    return count;
}
char *TextResBank::LoadString(u16 group, u16 langMask, u16 index)
{
    u8 a;
    u32 b;
    char *c = 0;
    u16 d;
    u32 e;
    b = index;
    if (const char *pack = LangPack_MenuString(group, index)) { /* Rewooled: the text language's pack */
        c = new char[strlen(pack) + 1];
        strcpy(c, pack);
        return c;
    }
    if (loaded == 1) {
        e = (group << 24) + (b << 8) + langMask;
        d = 0;
        a = 0;
        while (d < count && !a) {
            if (entries[d].key == e)
                a = 1;
            else
                d++;
        }
        if (a == 1) {
            c = new char[entries[d].len];
            strncpy(c, entries[d].text, entries[d].len);
            if (LangPack_FontActive() && entries[d].len && !c[entries[d].len - 1]) {
                /* Rewooled: the game's own text in a language pack's font (langpack.h) */
                const char *text = LangPack_GameText(c);
                char *recoded = new char[strlen(text) + 1];
                strcpy(recoded, text);
                delete[] c;
                c = recoded;
            }
        }
    }
    if (!c && group == REWOOLED_MENU_GROUP) { /* Rewooled's own menu text, which the game's file has not */
        if (const char *own = RewooledMenu_String(langMask, index)) {
            const char *text = LangPack_GameText(own);
            c = new char[strlen(text) + 1];
            strcpy(c, text);
        }
    }
    return c;
}
