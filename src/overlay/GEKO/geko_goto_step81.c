/* geko_goto_step81 -- GEKO overlay, Address: 0x8013B2A0, Size: 112 bytes (28 insns) */
/* @PostProc: scheduler_normalizer --target GEKO/0x8013b2a0 */
#include "types.h"

extern void class_clear_all(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void goto_step(s32 a0);

extern s8 ret_step;
extern s8 ret_sub_step;
extern s8 ret_sub_sub_step;
extern u8 now_step;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 rpg_bat_arg;

void geko_goto_step81(void)
{
    rpg_bat_arg = 6;
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    ret_step = now_step;
    ret_sub_step = now_sub_step;
    ret_sub_sub_step = (now_sub_sub_step + 1);
    goto_step(129);
}
