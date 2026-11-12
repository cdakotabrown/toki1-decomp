/* @PostProc: scheduler_normalizer */
/* shugaku_step81_goto2 - SHUGAKU overlay, Address: 0x80135944, Size: 104 bytes (26 insns) */
#include "types.h"

extern s8 ret_step;
extern s8 ret_sub_step;
extern s8 ret_sub_sub_step;
extern u8 now_step;
extern u8 now_sub_step;
extern s32 rpg_bat_arg;
extern void class_clear_all(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void goto_step(s32 a0);

void shugaku_step81_goto2(void)
{
    rpg_bat_arg = 7;
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    ret_step = (s8)(u8)now_step;
    ret_sub_step = (s8)(u8)now_sub_step;
    ret_sub_sub_step = (s8)56;
    goto_step(129);
}
