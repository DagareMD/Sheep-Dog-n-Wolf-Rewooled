/* The launcher's Language Export: the text a game folder does not have in its files (langexport_tables.cpp). */
#ifndef SDW_LAUNCHER_LANGEXPORT_TABLES_H
#define SDW_LAUNCHER_LANGEXPORT_TABLES_H

/* the save-game messages the PC release's program carries, by config language (English, Spanish, Italian,
 * Portuguese), by MCardString id; the game's encoding */
extern const char *const kExportSaveMessages[4][51];

/* the menu items Rewooled adds, in the menu catalogue's group of their own (Menus.txt: @0x40.0x1 to @0x40.0x9) */
#define EXPORT_REWOOLED_MENU_GROUP 0x40
#define EXPORT_REWOOLED_MENU_COUNT 9

/* item `index` (1..EXPORT_REWOOLED_MENU_COUNT) in the game's language with GREET_txt.BSM language bit `langMask`, in the
 * game's encoding; 0 for an unknown language or index */
const char *ExportTables_RewooledMenuString(unsigned langMask, unsigned index);

#endif
