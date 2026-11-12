/* Address: 0x800A0D28 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-copy-and-bump */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void SetPriority(u32 *p, s32 setmask, s32 chkmask) {
    u32 a, b;
    p[0] = (p[0] & 0xFFFFFF) | 0x2000000;
    a = setmask ? 2 : 0;
    b = chkmask ? 1 : 0;
    p[1] = (a | 0xE6000000) | b;
    p[2] = 0;
}
