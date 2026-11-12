/* Debug Symbol: holiday_tel_call (from o.bin, high match) */
/* Address: 0x800780E4 - 0x800781D4 (240 bytes, 60 instructions) */
/* Region: GCC (OR encoding) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern s8  now_icon;
extern s8  now_girl;

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void k_disp_inc(void);
extern void goto_sub_step(s32 a0);
extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

void holiday_tel_call(void) {
    s32 input;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_bar_show(0);
    menu_cursor_move(0);
    k_disp_inc();

    input = pad_trig;

    if (input & 0x40) {
        goto_sub_step(0);
        return;
    }

    if (!(input & 0x20)) {
        return;
    }

    if (now_icon == -1) {
        return;
    }

    if (now_icon == 13) {
        goto_sub_step(0);
        return;
    }

    device_cmd_send(1290);

    if (now_icon == 0) {
        now_girl = 13;
    } else if (now_icon == 12) {
        now_girl = 11;
    } else {
        now_girl = now_icon - 1;
    }

    sub_sub_step_up();
}
