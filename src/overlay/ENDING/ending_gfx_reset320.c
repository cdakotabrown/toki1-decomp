/* @SymbolStart: 0x8013409C */
/* @SymbolSize: 180 */
/* ending_gfx_reset320 - ENDING overlay, Address: 0x8013409C, Size: 180 bytes (45 instructions) */
/* @CompileFlags: -cckr */
#include "types.h"

extern void draw2d3d();
extern void class_switch();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void prim_init();
extern void class_clear_all();
extern void set_flag_b3_high_bit();
extern void text_sys_init();
extern void k_reset();
extern void ending_gfx_init320();
extern void sub_sub_step_up();
extern u32 step_counter;

s32 ending_gfx_reset320()
{
    step_counter++;
    if (step_counter < 2) {
        draw2d3d(1, 0);
        class_switch(1);
        back_clear_switch(1);
        tpage_buf_clear();
        prim_init();
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        text_sys_init();
        k_reset(1);
        return 0;
    }
    if (step_counter < 11) {
        return 0;
    }
    ending_gfx_init320();
    sub_sub_step_up();
}
