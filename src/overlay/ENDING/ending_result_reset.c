/* @SymbolSize: 84 */
/* ending_result_reset - ENDING overlay, Address: 0x80139D74, Size: 92 bytes (23 instructions) */
#include "types.h"

extern void class_clear_all();
extern void set_flag_b3_high_bit();
extern void prim_init();
extern void text_sys_init();
extern void vram_rect_clear();
extern void sub_sub_step_up();

extern s32 serifu_state;
extern s32 event_girl_no;

void ending_result_reset(void)
{
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    prim_init();
    text_sys_init();
    vram_rect_clear(3);
    serifu_state = 0;
    event_girl_no = 0;
    sub_sub_step_up();
}
