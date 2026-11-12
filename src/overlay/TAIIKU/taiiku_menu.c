/* @SymbolSize: 240 */
/* @CompileFlags: -Wo,-notail */
/* Address: 0x80141BC8 */
/* Size: 240 bytes (60 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void k_reset(s32);
extern s32 check_end_k(void);
extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern void step_directional_scroll_once(void);
extern void set_flag_b3_high_bit(s32);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 cursor_cw_disp;
extern s32 pad_trig;
extern u8 menu0_hit0;
extern u8 menu0_hit1;
extern u8 menu0_hit2;
extern s32 serifu_no;

s32 taiiku_menu(void)
{
    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);
    set_flag_b3_high_bit(1);
    step_directional_scroll_once();

    if ((pad_trig & 0x20) == 0) return;
    if (check_end_k() == 0) return;

    if (menu0_hit0 != 0) {
        serifu_no = 0;
        goto common;
    }
    if (menu0_hit1 != 0) {
        serifu_no = 1;
        goto common;
    }
    if (menu0_hit2 != 0) {
        serifu_no = 2;
        goto common;
    }
    return 0;
common:
    cursor_cw_disp = cursor_cw_disp & ~0x80;
    k_reset(1);
    sub_sub_step_up();
    return;
}
