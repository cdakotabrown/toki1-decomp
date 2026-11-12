/* @SymbolStart: 0x80138920 */
/* @SymbolSize: 120 */
/* func_80138928 -- GYOZI overlay, Address: 0x80138920, Size: 120 bytes (30 insns) */
#include "types.h"

extern void func_8013A998(void);
extern void func_8013AAAC(void);
extern void func_8013AB34(void);
extern void func_8013ABBC(void);

extern u8 D_801474B8;

void func_80138928(void)
{
    u32 temp = D_801474B8;

    switch (temp) {
    case 0:
        func_8013A998();
        break;
    case 1:
        func_8013AAAC();
        break;
    case 2:
        func_8013AB34();
        break;
    default:
        func_8013ABBC();
        break;
    }
}
