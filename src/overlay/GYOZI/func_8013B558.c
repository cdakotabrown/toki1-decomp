/* @SymbolStart: 0x8013B550 */
/* @SymbolSize: 96 */
/* func_8013B558 -- GYOZI overlay, Address: 0x8013B550, Size: 96 bytes (24 insns) */
#include "types.h"

extern void func_8013D5B0(void);
extern void func_8013D62C(void);
extern void func_8013D6B4(void);

extern u8 D_801474B8;

void func_8013B558(void)
{
    u32 temp = D_801474B8;

    switch (temp) {
    case 0:
        func_8013D5B0();
        break;
    case 1:
        func_8013D62C();
        break;
    default:
        func_8013D6B4();
        break;
    }
}
