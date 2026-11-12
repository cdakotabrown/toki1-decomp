/* @SymbolStart: 0x801344A0 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x801384CC */
/* olh_icon_page_main - OLH overlay, Address: 0x801344A8, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_icon_page_init(void);
extern void olh_main50_menu1(void);
extern void olh_icon_active_help(void);
extern void olh_icon_forced_help(void);
extern void olh_icon_free_move_help(void);
extern void olh_icon_cursor_speed_help(void);
extern void olh_icon_mouse_help(void);
extern void olh_icon_type_set_help(void);
extern void olh_icon_overlap_help(void);

void olh_icon_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_icon_page_init();
        break;
    case 1:
        olh_main50_menu1();
        break;
    case 16:
        olh_icon_active_help();
        break;
    case 32:
        olh_icon_forced_help();
        break;
    case 48:
        olh_icon_free_move_help();
        break;
    case 64:
        olh_icon_cursor_speed_help();
        break;
    case 80:
        olh_icon_mouse_help();
        break;
    case 96:
        olh_icon_type_set_help();
        break;
    case 112:
        olh_icon_overlap_help();
        break;
    }
}
