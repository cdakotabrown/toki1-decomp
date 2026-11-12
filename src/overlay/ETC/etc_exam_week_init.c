/* Address: 0x801467B0 */
/* Size: 312 bytes (78 instructions) */
/* @SymbolSize: 312 */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void tpage_buf_clear(void);
extern void class_clear_f(void);
extern void text_sys_init(void);
extern void parameter_show_init(void);
extern void hizuke_init(void);
extern void icon_disp_switch(s32 a0);
extern void icon_can_use_set(s32 a0, s32 a1);
extern void icon_init(void);
extern void icon_menu_set(s32 a0);
extern void basyo_init(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void icon_schedule_set(void);
extern u8 now_youbi;
extern u8 bukatu_pt10[];
extern u8 schedule_cursor;
extern u8 icon_num;
extern u8 etc_exam_week_basyo_str[];

void etc_exam_week_init(void)
{
    s32 s0;

    text_sys_init();
    basyo_init((s32)etc_exam_week_basyo_str);
    prim_init();
    class_clear_f();
    tpage_buf_clear();
    icon_init();
    icon_disp_switch(1);
    icon_schedule_set();

    for (s0 = 0; s0 != 14; s0++) {
        icon_can_use_set(s0, 0);
    }

    icon_menu_set(icon_num);
    set_flag_b3_high_bit(1);
    hizuke_init();
    parameter_show_init();
    parameter_show_init();

    if (now_youbi == 0) {
        device_load_start(1, 0xBFA6, 0xBF87, 0xCB56, 0xCB0B, 0xCB05);
    } else if (bukatu_pt10[schedule_cursor * 4] != 64) {
        device_load_start(1, 0xBFA6, 0xBF87, 0xCB56, 0xCB0B, 0xCB05);
    }

    sub_sub_step_up();
}
