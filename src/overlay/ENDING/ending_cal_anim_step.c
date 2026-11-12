/* @SymbolStart: 0x8013BE94 */
/* @SymbolSize: 152 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x40:rs:v1:v0 */
/* ending_cal_anim_step - ENDING overlay, Address: 0x8013BE94, Size: 152 bytes (38 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern u8  cal_cw_mode;
extern s16 cal_cw_frame;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s32 step_counter;

void ending_cal_anim_step(void)
{
    s32 v0 = step_counter;

    if (v0 == 0) {
        cal_cw_mode = 5;
    }
    v0 += 1;
    if (cal_cw_frame != 2) {
        goto done;
    }
    if (cal_cw_tick != 4) {
        goto done;
    }
    cal_cw_mode = 1;
    cal_cw_frame = 0;
    cal_cw_tick = 0;
    cal_cw_anim = 1;
done:
    step_counter = v0;
    if ((u32)v0 >= 181u) {
        sub_sub_step_up();
    }
}
