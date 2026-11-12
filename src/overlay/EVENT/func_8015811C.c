/* Size: 72 bytes */
/* @SymbolSize: 72 */
#include "types.h"

extern u8 D_800E9620[][68];

void func_8015811C(void)
{
    s16 i;

    for (i = 0; i < 20; i++) {
        D_800E9620[i][6667] &= 0x7F;
    }
}
