/* @SymbolStart: 0x801390C4 */
/* @SymbolSize: 256 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0x24:rt:t6:v1 0x28:rs:t6:v1 0x28:rt:t6:v1 0x30:rs:t6:v1 0x34:rt:t7:a0 0x38:rs:t7:a0 0x38:rt:t7:a0 0x40:rs:t7:a0 */
/* Function: date_basyo2_serifu_skip */
/* Overlay: DATE */
/* Address: 0x801390CC */
/* Size: 248 bytes (62 instructions) */
#include "types.h"

extern s32 sub_sub_step_up(void);

extern u8 now_girl;
extern volatile s16 now_serifu;
extern volatile s32 serifu_no;

s32 date_basyo2_serifu_skip(void)
{
    s32 v0;
    s32 v1;
    s32 a0;

    v0 = (s32)now_girl;
    if (v0 != 0) {
        goto L16;
    }
    if (now_serifu != 10) {
        goto L16;
    }
    if (serifu_no != 0) {
        goto L16;
    }
    sub_sub_step_up();
    return 0;

L16:
    v1 = now_serifu;
    a0 = serifu_no;
    if (v0 == 1) {
        goto L26;
    }
    if (v0 == 4) {
        goto L26;
    }
    if (v0 != 9) {
        goto L35;
    }
L26:
    if (v1 != 6) {
        goto L35;
    }
    if (a0 != 0) {
        goto L35;
    }
    sub_sub_step_up();
    return 0;

L35:
    if (v0 != 10) {
        goto L50;
    }
    if (v1 != 6) {
        goto L50;
    }
    if (a0 != 0) {
        goto L50;
    }
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    return 0;

L50:
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    return sub_sub_step_up();
}
