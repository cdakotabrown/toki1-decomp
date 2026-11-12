/* @SymbolSize: 472 */
/* option_list_main4 - OPTION overlay, Address: 0x80138FE0, Size: 472 bytes (118 insns) */
#include "types.h"

extern void option_menu_set();
extern void k_disp_switch();
extern void option_sub_vab_step();
extern void goto_sub_sub_step();
extern void _debug_number_show();
extern void debug_string_show();
extern void menu_check();
extern void menu_bar_show();
extern void menu_cursor_move();
extern void option_box_show();
extern void option_boxes_show();

extern u8  step_mode;
extern s32 pad_trig;
extern s8  menu_csr;
extern u8  option_sub_vab_no;
extern u8  option_sub_vab_name_tbl[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void option_list_main4(void)
{
    s32 i;
    s32 y;

    if (step_mode == 0) {
        option_menu_set(1);
        for (i = 0; i < 7; i++) {
            k_disp_switch(i + 1, 0);
        }
        step_mode = step_mode + 1;
    } else {
        if (pad_trig & 0x20) {
            if (menu_csr != -1) {
                option_sub_vab_no = (u8)(menu_csr + option_sub_vab_no);
            }
            option_sub_vab_step();
            for (i = 0; i < 7; i++) {
                k_disp_switch(i + 1, 1);
            }
            goto_sub_sub_step(2);
        }
    }

    for (i = 0, y = -100; i < 16; i++, y += 10) {
        _debug_number_show((u32)(option_sub_vab_no + i) % 63U, -10, y, 6);
    }

    for (i = 0, y = -100; i != 16; i++, y += 10) {
        debug_string_show(option_sub_vab_name_tbl + ((u32)(option_sub_vab_no + i) % 63U) * 16, 10, y);
    }

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    menu_bar_show(0);
    option_box_show();
    option_boxes_show();
}
