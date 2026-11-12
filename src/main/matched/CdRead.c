/* Function: CdRead */
/* Address: 0x80087E2C */
/* Size: 124 bytes (31 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
#include "types.h"

extern s32 CD_readm(s32 a0, s32 a1, s32 a2);

s32 CdRead(s32 arg0, s32 arg1, s32 arg2) {
    s32 count = 3;
    s32 limit = -1;
loop:
    if (CD_readm(arg1, arg0, arg2) != 0) {
        count--;
        if (count != limit) goto loop;
        return 0;
    }
    return 1;
}
