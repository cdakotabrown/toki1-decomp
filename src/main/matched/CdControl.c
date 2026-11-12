/* Function: CdControl */
/* Address: 0x800879B8 */
/* Size: 320 bytes (80 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x34:rd:v1:v0 --patch 0x44:rt:v0:v1 --patch 0x48:rs:v0:v1 --patch 0x48:rt:v0:v1 --patch 0x50:rs:v1:v0 --patch 0x50:rt:v0:v1 --patch 0x60:rt:v0:t0 --patch 0x64:rt:v0:t0 --patch 0xfc:rt:v0:t0 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
#include "types.h"

extern s32 CD_cbsync;
extern s32 CD_status;
extern s32 libcd_cmd_setloc_flag_tbl[];

extern s32 CD_cw(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 CdControl(s32 arg0, s32 arg1, s32 arg2) {
    s32 retry;
    s32 saved;
    s32 idx;
    s32 *tblPtr;
    s32 flag;
    s32 limit;

    limit = -1;
    idx = arg0 & 0xFF;
    saved = CD_cbsync;
    flag = 0;
    tblPtr = &libcd_cmd_setloc_flag_tbl[idx];

    for (retry = 3; retry != limit; retry--) {
        if (idx != 1) {
            CD_cbsync = 0;
            if (CD_status & 0x10) {
                CD_cw(1, 0, 0, 0);
                continue;
            }
            if (arg1 != 0 && *tblPtr != 0) {
                if (CD_cw(2, arg1, arg2, 0) != 0) {
                    continue;
                }
            }
            CD_cbsync = saved;
        }
        if (CD_cw(arg0 & 0xFF, arg1, arg2, 0) == 0) {
            goto done;
        }
    }
    CD_cbsync = saved;
    flag = -1;
done:
    return flag + 1;
}
