/* Address: 0x8009E1F0 */
/* Size: 256 bytes (64 instructions) */
/* Region: Library */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 gte_scale_matrix(s16 *m, s16 sx, s16 sy, s16 sz) {
    s32 changed = 0;

    if (sx != 4096) {
        changed = 1;
        m[0] = (s16)(((s32)m[0] * sx) >> 12);
        m[3] = (s16)(((s32)m[3] * sx) >> 12);
        m[6] = (s16)(((s32)m[6] * sx) >> 12);
    }
    if (sy != 4096) {
        changed = 1;
        m[1] = (s16)(((s32)m[1] * sy) >> 12);
        m[4] = (s16)(((s32)m[4] * sy) >> 12);
        m[7] = (s16)(((s32)m[7] * sy) >> 12);
    }
    if (sz != 4096) {
        changed = 1;
        m[2] = (s16)(((s32)m[2] * sz) >> 12);
        m[5] = (s16)(((s32)m[5] * sz) >> 12);
        m[8] = (s16)(((s32)m[8] * sz) >> 12);
    }
    return changed;
}
