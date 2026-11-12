/* @SymbolStart: 0x8013A4D0 */
/* @SymbolSize: 144 */
/* func_8013A4D8 -- GYOZI overlay, Address: 0x8013A4D0, Size: 144 bytes (36 insns) */
#include "types.h"

extern void func_8013C560(void);
extern void func_8013C5E8(void);
extern void func_8013C670(void);
extern void func_8013C6EC(void);
extern void func_8013C774(void);

extern u8 D_801474B8;

void func_8013A4D8(void)
{
    u32 temp = D_801474B8;

    switch (temp) {
    case 0:
        func_8013C560();
        break;
    case 1:
        func_8013C5E8();
        break;
    case 2:
        func_8013C670();
        break;
    case 3:
        func_8013C6EC();
        break;
    default:
        func_8013C774();
        break;
    }
}
