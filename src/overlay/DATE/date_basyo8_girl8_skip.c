/* @SymbolStart: 0x8013E110 */
/* @SymbolSize: 104 */
/* @PostProc: patch_insn_reg_field 0x50:rs:v0:v1 0x50:rt:v1:v0 */
/* Address: 0x8013E110 (cross-prologue head) / 0x8013E118 (body) */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern u8 now_girl;
extern u8 girl8_spot15_cnt;
extern u8 now_sub_sub_step;
extern s16 ans;

extern void sub_sub_step_up(void);

void date_basyo8_girl8_skip(void) {
    s32 v;
    s32 cmp = 8;

    v = (s32)now_girl;
    if (v != cmp) goto path_b;
    if (girl8_spot15_cnt == 0) goto path_b;
    now_sub_sub_step += 30;
    goto end;
path_b:
    if (v != cmp) goto end;
    ans = 1;
end:
    sub_sub_step_up();
}
