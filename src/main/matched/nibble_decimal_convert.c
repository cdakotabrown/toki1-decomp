/* Address: 0x800469C4 */
/* Size: 208 bytes (52 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions 0x3c 0x44 */
/* @PostProc: rename_register_chain --chain a1:a2:a3:t0:t1:a1 */
#include "types.h"

s32 nibble_decimal_convert(u32 arg0) {
    s32 sum;
    s32 w;
    s32 s0, s1, s2, s3;

    sum = 0;
    w = 1;
    s0 = 0;
    s1 = 4;
    s2 = 8;
    s3 = 12;
    do {
        sum += (s32)(w * (arg0 & (0xFU << s0))) >> s0;
        w = w * 10;
        s0 += 16;
        sum += (s32)(w * (arg0 & (0xFU << s1))) >> s1;
        w = w * 10;
        s1 += 16;
        sum += (s32)(w * (arg0 & (0xFU << s2))) >> s2;
        w = w * 10;
        s2 += 16;
        sum += (s32)(w * (arg0 & (0xFU << s3))) >> s3;
        w = w * 10;
        s3 += 16;
    } while (s3 != 44);
    return sum;
}
