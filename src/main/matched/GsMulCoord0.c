/* Function: GsMulCoord0 */
/* Address:  0x8009AF38 */
/* Size:     144 bytes (36 instructions) */
/* Region:   Library (0x80086000+) */
/* @Compiler: gcc281 */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x84 */
#include "types.h"

extern void ApplyMatrixLV(s32 *, s32 *, s32 *);
extern void MulMatrix0(s32 *, s32 *, s32 *);

void GsMulCoord0(s32 *a0, s32 *a1, s32 *a2) {
    ApplyMatrixLV(a0, a1 + 5, a2 + 5);
    MulMatrix0(a0, a1, a2);
    a2[5] = a2[5] + a0[5];
    a2[6] = a2[6] + a0[6];
    a2[7] = a2[7] + a0[7];
}
