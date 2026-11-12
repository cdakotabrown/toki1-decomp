/* @SymbolSize: 52 */
/* Address: 0x8014A2AC */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern s16 para_zat;
extern s16 para_you;

s32 etc_sum_check(void)
{
    if (para_zat + para_you > 256) {
        return 1;
    }
    return 0;
}
