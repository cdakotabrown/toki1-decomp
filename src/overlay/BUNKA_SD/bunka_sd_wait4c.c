/* @PostProc: patch_insn_reg_field 0x28:rt:v0:t6 0x2c:rs:v0:t6 0x2c:rt:v0:t6 0x34:rt:t6:t8 0x38:rs:v0:t6 0x38:rt:v0:t7 0x3c:rs:t6:t8 0x3c:rt:t6:t8 0x40:rt:v0:t7 0x48:rs:t6:t8 0x4c:rt:t7:t9 0x50:rs:v0:t7 0x6c:rs:t7:t9 0x6c:rt:t7:t9 0x74:rs:t7:t9 0x74:rt:t8:t0 0x78:rs:t8:t0 */
#include "types.h"

extern void bunka_sd_cue_send3(void);
extern void bunka_sd_cal_cw_mode_set(void);
extern void sub_sub_step_up(void);

extern s32 step_counter;
extern s32 bunka_sd_kind;
extern u8 cal_cw_mode;

void bunka_sd_wait4c(void)
{
    s32 counter;

    bunka_sd_cue_send3();
    bunka_sd_cal_cw_mode_set();
    counter = step_counter + 1;
    step_counter = counter;
    if (bunka_sd_kind == 3) {
        if ((u32)counter >= 603U) {
            sub_sub_step_up();
        }
        return;
    }
    if (!(cal_cw_mode & 1)) {
        sub_sub_step_up();
    }
}
