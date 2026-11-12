/* @SymbolStart: 0x8015525C */
/* @SymbolSize: 88 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t9:t0 0x48:rs:t9:t0 0x48:rt:t0:t1 0x4c:rt:t0:t1 */
/* Address: 0x8015525C (cross-prologue head) / 0x80155264 (body) */
/* Size: 88 bytes (22 instructions) */
#include "types.h"

extern s32 serifu_no;
extern u16 now_serifuev;
extern u8  girl1_flag_hi;

extern void sub_sub_step_up(void);

void date_girl1_serifu_flag_step(void) {
    if (serifu_no != 0) {
        now_serifuev = now_serifuev + 1;
    } else {
        girl1_flag_hi = girl1_flag_hi | 0x40;
    }
    sub_sub_step_up();
}
