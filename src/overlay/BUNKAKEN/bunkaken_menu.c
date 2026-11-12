/* Address: 0x80144EBC */
/* Size: 216 bytes (54 insns) */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 k_reset();
extern void menu_check();
extern void menu_cursor_move();
extern void menu_bar_show();
extern void step_directional_scroll_once();
extern void set_flag_b3_high_bit();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern u8 screen_w[];
extern u8 cursor_cw_disp;
extern s32 serifu_no;
extern s32 menu_on;

void bunkaken_menu(void) {
    s32 i;
    s32 count;
    s32 mask;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);
    set_flag_b3_high_bit(0);
    step_directional_scroll_once();

    if ((pad_trig & 0x20) == 0) {
        return;
    }

    count = 6;
    mask = -129;
    for (i = 0; i != count; i++) {
        if (screen_w[3990 + i * 16] != 0) {
            serifu_no = i;
            cursor_cw_disp = cursor_cw_disp & mask;
            k_reset(1);
            menu_on = 0;
            sub_sub_step_up();
        }
    }
}
