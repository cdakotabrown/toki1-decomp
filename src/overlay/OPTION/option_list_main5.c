#include "types.h"

extern void option_menu_set(s32);
extern void k_disp_switch(s32, s32);
extern void goto_sub_sub_step(s32);
extern void _debug_number_show(s32, s32, s32, s32);
extern void debug_string_show(u8 *, s32, s32);
extern void menu_check(s32, s32, s32);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void option_box_show(void);
extern void option_boxes_show(void);

extern u8 step_mode;
extern s32 pad_trig;
extern s8 menu_csr;
extern u8 option_se_bank_no;
extern u8 option_se_name_tbl[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void option_list_main5(void)
{
    s32 i;

    if (step_mode == 0) {
        option_menu_set(0);
        for (i = 0; i < 7; i++) {
            k_disp_switch(i + 1, 0);
        }
        step_mode++;
    } else if (pad_trig & 0x20) {
        if (menu_csr != -1) {
            option_se_bank_no = menu_csr + option_se_bank_no;
        }
        for (i = 0; i < 7; i++) {
            k_disp_switch(i + 1, 1);
        }
        goto_sub_sub_step(2);
    }
    for (i = 0; i < 16; i++) {
        _debug_number_show((u32)(option_se_bank_no + i) % 58, -10, i * 10 - 100, 6);
    }
    for (i = 0; i < 16; i++) {
        debug_string_show(&option_se_name_tbl[((u32)(option_se_bank_no + i) % 58) * 16], 10, i * 10 - 100);
    }
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    menu_bar_show(0);
    option_box_show();
    option_boxes_show();
}
