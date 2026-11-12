/* @SymbolStart: 0x80156504 */
/* @SymbolSize: 68 */
/* @PostProc: patch_insn_reg_field 0x1c:rd:t7:t8 0x20:rs:t7:t8 0x28:rt:t8:t9 0x2c:rs:t8:t9 0x2c:rt:t8:t9 0x34:rs:t8:t9 0x34:rt:t9:t0 0x38:rt:t9:t0 */
/* Address: 0x80156504 (cross-prologue head) / 0x8015650C (body) */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern s32 girl5_flag;
extern u16 now_serifuev;

extern void sub_sub_step_up(void);

void date_girl5_flag_on_step(void)
{
    if ((s32)(girl5_flag << 17) < 0) {
        now_serifuev = now_serifuev + 1;
    }
    sub_sub_step_up();
}
