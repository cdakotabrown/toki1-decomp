/* @SymbolSize: 212 */
/* Address: 0x80149314 */
/* Size: 212 bytes (53 instructions) */
#include "types.h"

extern u8 etc_siken_init_str0;
extern s32 etc_siken_qno;
extern s32 etc_siken_sel;
extern s32 etc_siken_seikai;
extern s32 etc_siken_base;

extern void prim_init();
extern void sub_sub_step_up();
extern void draw2d3d();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void class_switch();
extern void class_clear_f();
extern void text_sys_init();
extern void k_sub_reset_point_set();
extern void k_sub_disp_start();
extern void dec_bg_show();
extern void parameter_disp_switch();
extern void parameter_show_init();
extern void message_window_init();
extern void icon_disp_switch();
extern void mes_window_switch();
extern void basyo_init();
extern s32 rand();

void etc_siken_init(void)
{
    s32 result;

    draw2d3d(1, 0);
    class_switch(1);
    class_clear_f();
    back_clear_switch(1);
    tpage_buf_clear();
    prim_init();
    icon_disp_switch(0);
    parameter_show_init();
    parameter_disp_switch(1);
    message_window_init();
    text_sys_init();
    basyo_init(&etc_siken_init_str0);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    mes_window_switch(0);
    dec_bg_show(0);
    etc_siken_qno = 0;
    result = rand();
    etc_siken_base = result % 12;
    etc_siken_sel = 0;
    etc_siken_seikai = 0;
    sub_sub_step_up();
}
