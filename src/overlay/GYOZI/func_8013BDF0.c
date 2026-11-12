/* @SymbolStart: 0x8013BDE8 */
/* @SymbolSize: 68 */
/* func_8013BDF0 -- GYOZI overlay, Address: 0x8013BDE8, Size: 68 bytes (17 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F5833;
extern s16 D_801474A8;

void func_8013BDF0(void)
{
    u32 temp = D_800F5833;

    if (temp < 2U) goto label0;
    D_801474A8 = (D_801474A8 + 1);

label0:
    func_8004DE1C();
}
