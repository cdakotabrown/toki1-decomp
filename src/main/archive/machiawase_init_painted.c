/* Function: machiawase_init */
/* Address: 0x800766E0 */
/* Size: 336 bytes (84 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_message_disp_switch.py */
#include "types.h"

extern void k_reset(s32);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void gnsx(u8 *);
extern void sndisp(u8 *, s32, s32);
extern s32 func_8007653C(void *);

extern u8 machiawase_basyo2;
extern u8 machiawase_basyo3;
extern u8 machiawase_mode;
extern u8 machiawase_basyo;
extern volatile u8 schedule_cursor;
extern u8 screen_w[];
extern volatile u32 reserved_date;
extern u8 now_girl;
extern u8 girl_no;
extern u8 machiawase_flag;
extern u8 p_sei;
extern u8 machiawase_init_str0;

void machiawase_init(void) {
    s32 local_buf[33];
    s32 pad[4];
    u32 color;

    (void)&pad;

    machiawase_basyo3 = 255;

    if (func_8007653C(local_buf) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(&p_sei);
        sndisp(&machiawase_init_str0, 0, 31);
        k_sub_disp_start(1);
        sub_sub_step_up();
        return;
    }

    color = reserved_date;
    machiawase_mode = color & 0x1F;
    machiawase_basyo = color & 0x1F;
    machiawase_basyo2 = 255;
    machiawase_basyo3 = 255;

    screen_w[schedule_cursor * 4 + 1692] = 128;
    screen_w[schedule_cursor * 4 + 1693] = (reserved_date << 3) >> 28;
    screen_w[schedule_cursor * 4 + 1694] = reserved_date & 0x1F;

    color = reserved_date;
    now_girl = (color << 3) >> 28;
    girl_no = (color << 3) >> 28;
    machiawase_flag = screen_w[1359] & 1;

    goto_sub_sub_step(2);
}
