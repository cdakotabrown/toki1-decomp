/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* Address: 0x8006FB9C */
/* Size: 92 bytes (23 instructions) */
/* Status: ASM_FINAL (hand-edited assembly — will not match from C) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Status: BLOCKED - ASM-only: dead tail fragment */
#include "types.h"

s32 func_8006FB9C(s32 t0, s32 t1, s32 a2, s32 a3) {
    s32 t2 = 0;

    if (t0 < a2 && a2 > 0) {
        return a3;
    }

    if (t2 != 0 && t0 < t1 && t1 > 0) {
        return t2;
    }

    if (t0 <= 0) {
        return 15;
    }

    return 0;
}
