/* @SymbolStart: 0x80135440 */
/* @SymbolSize: 136 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x40:rs:v1:v0 */
/* bunka_sd_wait4d - BUNKA_SD overlay, Address: 0x80135440, Size: 136 bytes (34 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u32 step_counter;
extern u8 cal_cw_mode;
extern s32 bunka_sd_kind;

void bunka_sd_wait4d(void)
{
    u32 v;

    v = step_counter;
    if (v == 0) {
        cal_cw_mode = 5;
    }

    v = v + 1;

    if (bunka_sd_kind == 3) {
        step_counter = v;
        if (v >= 450) {
            sub_sub_step_up();
        }
        return;
    }

    step_counter = v;
    if ((cal_cw_mode & 1) == 0) {
        sub_sub_step_up();
    }
}
