/* Address: 0x800466B4 */
/* Size: 124 bytes (31 instructions) */
/* Region: Game (IDO 5.2) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -cckr -Wc,-pic1 */
#include "types.h"

s32 block_checksum_check(blocks, buf)
u8 blocks;
u32 buf;
{
    u16 sum;
    u32 i;
    u8 *p;
    u32 hi;
    u32 lo;

    sum = 0;
    for (i = 0; i < (blocks << 11) - 4; i += 2) {
        sum += *(u8 *)(buf + i);
        sum += *(u8 *)(buf + i + 1);
    }
    p = (u8 *)(i + buf);
    hi = p[0];
    lo = p[1];
    if (sum != (hi << 8) + lo) {
        return 0;
    }
    return 1;
}
