/* @SymbolSize: 92 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 now_step;
extern u8 now_sub_step;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern void class_clear_all(s32);
extern void set_flag_b3_high_bit(s32);
extern void goto_step(s32);

void date_basyo18_step130_goto(void)
{
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    ret_step = now_step;
    ret_sub_step = now_sub_step;
    ret_sub_sub_step = 58;
    goto_step(130);
}
