/* Address: 0x80087AF8 */
/* Size: 316 bytes (79 instructions) */
/* Region: Library */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -mhard-float --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x8 0x2c */
/* @PostProc: swap_instructions 0xc 0x3c */
/* @PostProc: swap_instructions 0x10 0x3c */
/* @PostProc: swap_instructions 0x14 0x1c */
/* @PostProc: swap_instructions 0x18 0x24 */
/* @PostProc: swap_instructions 0x1c 0x2c */
/* @PostProc: swap_instructions 0x20 0x4c */
/* @PostProc: swap_instructions 0x24 0x30 */
/* @PostProc: swap_instructions 0x28 0x40 */
/* @PostProc: swap_instructions 0x2c 0x34 */
/* @PostProc: swap_instructions 0x30 0x34 */
/* @PostProc: swap_instructions 0x34 0x38 */
/* @PostProc: swap_instructions 0x38 0x48 */
/* @PostProc: swap_instructions 0x3c 0x54 */
/* @PostProc: swap_instructions 0x40 0x44 */
/* @PostProc: swap_instructions 0x44 0x58 */
/* @PostProc: swap_instructions 0x48 0x50 */
/* @PostProc: swap_instructions 0x4c 0x50 */
/* @PostProc: swap_instructions 0x50 0x58 */
/* @PostProc: swap_instructions 0x54 0x58 */
/* @PostProc: swap_instructions 0x100 0x104 */
/* @PostProc: swap_instructions 0x104 0x108 */
#include "types.h"

extern u32 CD_cbsync;
extern s32 CD_status;
extern s32 libcd_cmd_setloc_flag_tbl[];
extern s32 CD_cw(s32 a0, s32 a1, s32 a2, s32 a3);

s32 CdControlF(s32 a0, s32 a1)
{
    s32 saved;
    s32 tries;
    s32 attempt;
    s32 flags;
    s32 idx;

    idx = a0 & 0xFF;
    saved = CD_cbsync;
    attempt = 0;
    for (tries = 3; tries != -1; tries--) {
        if (idx != 1) {
            flags = CD_status;
            CD_cbsync = 0;
            if (flags & 0x10) {
                CD_cw(1, 0, 0, 0);
                continue;
            }
            if (a1 != 0 && libcd_cmd_setloc_flag_tbl[idx] != 0) {
                if (CD_cw(2, a1, 0, 0) != 0) {
                    continue;
                }
            }
            CD_cbsync = saved;
        }
        if (CD_cw(a0 & 0xFF, a1, 0, 1) == 0) {
            goto out;
        }
    }
    CD_cbsync = saved;
    attempt = -1;
out:
    return attempt + 1;
}
