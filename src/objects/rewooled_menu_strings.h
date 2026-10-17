/* Rewooled's own menu text: the menu items Rewooled adds, in the game's menu catalogue as the game's own are
 * (TextResBank::LoadString), in a group of their own that GREET_txt.BSM does not use. The game's languages' text is
 * carried here (the game's files have none); a language pack's is in its Menus.txt, under @0x40.0xN as any other. */
#ifndef SDW_OBJECTS_REWOOLED_MENU_STRINGS_H
#define SDW_OBJECTS_REWOOLED_MENU_STRINGS_H

#define REWOOLED_MENU_GROUP 0x40

enum RewooledMenuString {
    RWMENU_REWOOLED_SETTINGS = 1,
    RWMENU_OBJECT_OUTLINES,
    RWMENU_CAMERA_SPEED,
    RWMENU_CAMERA_ACCELERATION,
    RWMENU_INVERT_CAMERA_JOYSTICK,
    RWMENU_TEXT_LANGUAGE,
    RWMENU_VOICE_LANGUAGE,
    RWMENU_SAME_AS_TEXT,
    RWMENU_TEXT_LANGUAGE_SAVED,
    RWMENU_COUNT = RWMENU_TEXT_LANGUAGE_SAVED /* the last index */
};

/* string `index` (1..RWMENU_COUNT) of the game's language with GREET_txt.BSM language bit `langMask` (LANGMASK_*), in
 * the game's encoding; 0 for another group's index or an unknown language */
const char *RewooledMenu_String(unsigned langMask, unsigned index);

#endif
