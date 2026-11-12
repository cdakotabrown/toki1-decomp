/* @SymbolSize: 104 */
/* @RodataBase: 0x8014D8D4 */
#include "types.h"

extern s32 etc_siken_seikai;

s32 get_etc_index(void)
{
    switch ((u32)etc_siken_seikai) {
    case 10:
        return 0;
    case 8:
    case 9:
        return 1;
    case 7:
        return 2;
    case 5:
    case 6:
        return 3;
    case 3:
    case 4:
        return 4;
    case 1:
    case 2:
    default:
        return 5;
    }
}
