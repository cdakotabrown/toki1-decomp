/* Function: CdControlB */
/* Address: 0x80087C34 */
/* Size: 340 bytes (85 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: patch_insn_reg_field --patch 0x2c:rt:v0:v1 --patch 0x34:rs:v0:v1 --patch 0x34:rt:v0:v1 --patch 0x38:rd:v1:v0 --patch 0x48:rs:v1:v0 --patch 0x48:rt:v0:v1 */
#include "types.h"

extern s32 CD_cbsync;
extern s32 CD_status;
extern s32 libcd_cmd_setloc_flag_tbl[];

extern s32 CD_cw(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 CD_sync(s32 arg0, s32 arg1);

s32 CdControlB(s32 arg0, s32 arg1, s32 arg2) {
    s32 retry;
    s32 saved;
    s32 idx;
    s32 *tblPtr;
    s32 result;
    s32 one;
    s32 limit;

    idx = arg0 & 0xFF;
    saved = CD_cbsync;
    tblPtr = &libcd_cmd_setloc_flag_tbl[idx];
    result = 0;
    retry = 3;
    one = 1;
    limit = -1;

loop_top:
    if (idx == one) {
        goto process_call;
    }

    CD_cbsync = 0;

    if (CD_status & 0x10) {
        CD_cw(1, 0, 0, 0);
        goto decrement;
    }

    if (arg1 == 0) {
        goto restore_and_call;
    }

    if (*tblPtr == 0) {
        goto restore_and_call;
    }

    if (CD_cw(2, arg1, arg2, 0) != 0) {
        goto decrement;
    }

restore_and_call:
    CD_cbsync = saved;

process_call:
    if (CD_cw(arg0 & 0xFF, arg1, arg2, 0) != 0) {
        goto decrement;
    }

    result = 0;
    goto check_result;

decrement:
    retry--;
    if (retry != limit) {
        goto loop_top;
    }

    CD_cbsync = saved;
    result = -1;

check_result:
    if (result != 0) {
        return 0;
    }

    result = CD_sync(0, arg2);
    result ^= 2;
    return (u32)result < 1u;
}
