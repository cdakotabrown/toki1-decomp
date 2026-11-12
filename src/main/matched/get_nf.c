/* Address: 0x80066BB4 */
/* Size: 72 bytes (18 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @SymbolStart: 0x80066BB4 */
/* @SymbolSize: 72 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_month;

s32 get_nf(void)
{
    switch (now_month - 1) {
    default:
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 10:
    case 11:
        return 0;
    case 5:
    case 6:
    case 7:
    case 8:
        return 1;
    }
}
