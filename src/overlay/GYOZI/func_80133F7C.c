/* @SymbolStart: 0x80133F74 */
/* @SymbolSize: 116 */
/* func_80133F7C -- GYOZI overlay, Address: 0x80133F74, Size: 116 bytes (29 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_801460C0;
extern s16 D_80145EB4;

void func_80133F7C(void)
{
    u32 temp = D_801460C0;

    switch (temp) {
    case 0:
        D_80145EB4 = D_80145EB4 + 3;
        break;
    case 1:
        D_80145EB4 = D_80145EB4 + 2;
        break;
    case 2:
    default:
        break;
    }
    func_8004DE1C();
}
