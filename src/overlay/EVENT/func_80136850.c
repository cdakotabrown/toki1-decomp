/* Address: 0x80136850 */
/* Size: 104 bytes */
/* @SymbolSize: 104 */
#include "types.h"

extern u8 D_800B1AF6;
extern u32 D_800B1AE4;
extern u8 D_800EAFA7;
extern u8 D_800EB02F;
extern u8 D_800EB02A;

void func_80136850(void)
{
    u8 c;

    c = D_800B1AF6;
    if ((u32)c >= 27) {
        D_800EB02F = D_800EAFA7;
    }
    if ((u32)c >= 27) {
        if (D_800B1AE4 % 300 == 20) {
            D_800EB02A |= 3;
        }
    }
}
