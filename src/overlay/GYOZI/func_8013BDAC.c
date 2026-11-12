/* @SymbolStart: 0x8013BDA4 */
/* @SymbolSize: 68 */
/* func_8013BDAC -- GYOZI overlay, Address: 0x8013BDA4, Size: 68 bytes (17 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F5833;
extern s16 D_801474A8;

void func_8013BDAC(void)
{
    u32 temp = D_800F5833;

    if (temp < 2U) {
        D_801474A8 = D_801474A8 + 1;
    }
    func_8004DE1C();
}
