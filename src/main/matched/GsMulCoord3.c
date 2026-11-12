/* Function: GsMulCoord3 */
/* Address:  0x8009B048 */
/* Size:     128 bytes (32 instructions) */
/* Region:   Library (0x80086000+) */
/* @Compiler: gcc281 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void ApplyMatrixLV(s32 *, s32 *, s32 *);
extern void MulMatrix(s32 *, s32 *);

void GsMulCoord3(s32 *a0, s32 *a1) {
    s32 local[3];

    ApplyMatrixLV(a0, a1 + 5, local);
    MulMatrix(a0, a1);
    a0[5] = local[0] + a0[5];
    a0[6] = local[1] + a0[6];
    a0[7] = local[2] + a0[7];
}
