/* Address: 0x800468D0 */
/* Size: 244 bytes (61 instructions) */
/* Region: Game */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 keta_n(s32 num, s32 pos) {
    s32 divisor;
    u32 i;
    s32 ten;

    num = (s16)num;
    pos &= 0xff;
    divisor = 1;
    if ((u32)pos >= 6) {
        return 0;
    }
    ten = 10;
    for (i = 0; i < pos; i++) {
        divisor = (u32)divisor * ten;
    }
    return (num / (divisor / ten)) % ten;
}
