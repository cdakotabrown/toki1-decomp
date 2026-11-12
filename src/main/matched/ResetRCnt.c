/* Address: 0x800B58BC */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (Psy-Q GCC) */
/* Compiler: gcc260 */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern s16 *libapi_rcnt_regs_ptr;

s32 ResetRCnt(s32 a0) {
    s32 idx;

    idx = a0 & 0xffff;
    if (idx >= 3) {
        return 0;
    }
    libapi_rcnt_regs_ptr[idx * 8] = 0;
    return 1;
}
