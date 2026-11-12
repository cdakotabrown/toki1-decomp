/* Address: 0x80066B5C */
/* Size: 88 bytes (22 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @SymbolStart: 0x80066B5C */
/* @SymbolSize: 88 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_month;

s32 get_season(void)
{
    switch (now_month - 1) {
    default:
    case 2:
    case 3:
    case 4:
        return 0;
    case 5:
    case 6:
    case 7:
        return 1;
    case 8:
    case 9:
    case 10:
        return 2;
    case 0:
    case 1:
    case 11:
        return 3;
    }
}
