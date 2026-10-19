/* Rewooled's own menu text in the game's seven languages (rewooled_menu_strings.h).
 *
 * TO TRANSLATE: only English is written; the other six languages have the English text until they are translated. */
#include "rewooled_menu_strings.h"

/* by language bit: English 0x01, French 0x02, German 0x04, Dutch 0x08, Spanish 0x10, Italian 0x20, Portuguese 0x40 */
#define REWOOLED_MENU_ENGLISH                                                                                          \
    {"Rewooled Settings", "Object Outlines", "Camera Speed", "Camera Acceleration", "Invert Camera Joystick",          \
     "Text Language", "Voice Language", "Same as text",                                                                \
     "Configuration saved!\nThe new text language\nstarts at the next level."}

static const char *const s_strings[7][RWMENU_COUNT] = {
    REWOOLED_MENU_ENGLISH, /* English */
    REWOOLED_MENU_ENGLISH, /* French: to translate */
    REWOOLED_MENU_ENGLISH, /* German: to translate */
    REWOOLED_MENU_ENGLISH, /* Dutch: to translate */
    REWOOLED_MENU_ENGLISH, /* Spanish: to translate */
    REWOOLED_MENU_ENGLISH, /* Italian: to translate */
    REWOOLED_MENU_ENGLISH, /* Portuguese (Brazil): to translate */
};

const char *RewooledMenu_String(unsigned langMask, unsigned index)
{
    for (unsigned bit = 0; bit < 7; bit++)
        if (langMask == (1u << bit))
            return index >= 1 && index <= RWMENU_COUNT ? s_strings[bit][index - 1] : 0;
    return 0;
}
