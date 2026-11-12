/* Function: rsin */
/* Address: 0x800A404C */
/* Size: 60 bytes (15 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 -G0 -mips1 */
/* @PostProc: strip_leading_instructions --count 22 */
#include "types.h"

extern s32 sin_1(s32);

s32 rsin(u32 a0) {
    s32 sa0 = (s32)a0;
    if (sa0 < 0) {
        sa0 = -sa0;
        return -sin_1(sa0 & 0xFFF);
    }
    return sin_1(a0 & 0xFFF);
}
