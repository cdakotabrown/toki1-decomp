/* Address: 0x800987D0, Size: 48 bytes (12 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* the decrement and uopt re-points the branch. `@Compiler: ido52-gas` */
/* @Compiler: ido52-gas */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xc:rt:v1:t6 0x24:rt:v1:t6 */
#include "types.h"

void _memcpy(u8 *dst, const u8 *src, s32 n) {
    s32 t;
    u8 c;

    t = n;
    n--;
    while (t != 0) {
        c = *src;
        t = n;
        n--;
        src++;
        dst++;
        dst[-1] = c;
    }
}
