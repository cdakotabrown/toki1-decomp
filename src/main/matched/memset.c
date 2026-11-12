/* Address: 0x800A2974, Size: 40 bytes (10 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* the decrement and uopt re-points the branch. `@Compiler: ido52-gas` */
/* @Compiler: ido52-gas */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void memset(u8 *dst, s32 val, s32 count) {
    s32 t;

    t = count;
    count--;
    while (t != 0) {
        t = count;
        count--;
        *dst++ = (u8)val;
    }
}
