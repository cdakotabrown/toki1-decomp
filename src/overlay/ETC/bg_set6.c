/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 140 */
/* Address: 0x80136948 */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern void bg_set();

void bg_set6(s32 a0, s32 a1, s32 a2, s32 a3,
                   s32 arg4, s32 arg5)
{
    s32 data[6];
    s32 unused;

    data[0] = a0;
    data[1] = a1;
    data[2] = a2;
    data[3] = a3;
    data[4] = arg4;
    data[5] = arg5;

    bg_set(data, 0, 6, 3,
                  2, 497, -160, -101, 12, 5);
}
