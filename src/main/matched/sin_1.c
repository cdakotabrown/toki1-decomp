/* Address: 0x800A4088 */
/* Size: 148 bytes (37 instructions) */
/* Region: Library */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --delay-slot-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 rsin_tbl[];

s32 sin_1(s32 a0) {
    if (a0 < 2049) {
        if (a0 < 1025) {
            return rsin_tbl[a0];
        }
        return rsin_tbl[2048 - a0];
    }
    if (a0 < 3073) {
        return -rsin_tbl[a0 - 2048];
    }
    return -rsin_tbl[4096 - a0];
}
