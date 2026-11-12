/* @SymbolStart: 0x80133FA8 */
/* @SymbolSize: 244 */
/* ending_gfx_reset256 - ENDING overlay, Address: 0x80133FA8, Size: 244 bytes (61 instructions) */
/* @CompileFlags: -cckr */
#include "types.h"

extern u32 step_counter;
extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;
extern void ending_ptr_init();
extern void draw2d3d();
extern void class_switch();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void prim_init();
extern void class_clear_all();
extern void set_flag_b3_high_bit();
extern void text_sys_init();
extern void shared_system_state_reset();
extern s32 SetDispMask();
extern void ending_gfx_init256();
extern void sub_sub_step_up();

s32 ending_gfx_reset256()
{
    step_counter++;
    if (step_counter < 2) {
        ending_ptr_init();
        draw2d3d(1, 0);
        class_switch(1);
        back_clear_switch(1);
        tpage_buf_clear();
        prim_init();
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        text_sys_init();
        shared_system_state_reset();
        screen_bri = 128;
        fade_bri = 0;
        bg_change_tbl2 = 0;
        return 0;
    }
    if (step_counter == 5) {
        SetDispMask(0);
    } else if (step_counter < 11) {
        return 0;
    }
    ending_gfx_init256();
    sub_sub_step_up();
}
