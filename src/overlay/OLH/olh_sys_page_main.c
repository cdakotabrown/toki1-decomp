/* @SymbolStart: 0x80132420 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x80137628 */
/* olh_sys_page_main - OLH overlay, Address: 0x80132428, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_page10_init(void);
extern void olh_sys_menu_select(void);
extern void olh_sys_icon_type_help(void);
extern void olh_sys_mouselike_help(void);
extern void olh_sys_pad_a_help(void);
extern void olh_sys_pad_b_help(void);
extern void olh_sys_mouselike_pad_a_help(void);
extern void olh_sys_select_button_help(void);
extern void olh_sys_mascot_help(void);

void olh_sys_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_page10_init();
        break;
    case 1:
        olh_sys_menu_select();
        break;
    case 16:
        olh_sys_icon_type_help();
        break;
    case 32:
        olh_sys_mouselike_help();
        break;
    case 48:
        olh_sys_pad_a_help();
        break;
    case 64:
        olh_sys_pad_b_help();
        break;
    case 80:
        olh_sys_mouselike_pad_a_help();
        break;
    case 96:
        olh_sys_select_button_help();
        break;
    case 112:
        olh_sys_mascot_help();
        break;
    }
}
