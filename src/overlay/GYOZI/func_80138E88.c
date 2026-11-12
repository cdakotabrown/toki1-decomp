/* @SymbolStart: 0x80138E80 */
/* @SymbolSize: 156 */
/* @RodataBase: 0x80145B60 */
/* func_80138E88 -- GYOZI overlay, Address: 0x80138E80, Size: 156 bytes (39 insns) */
#include "types.h"

extern void func_8013AF1C(void);
extern void func_8013B03C(void);
extern void func_8013B0C4(void);
extern void func_8013B2B8(void);
extern void func_8013B340(void);
extern void func_8013B3B4(void);

extern u8 D_801474B8;

void func_80138E88(void) {
    u32 cmd = D_801474B8;

    switch (cmd) {
    case 0:
        func_8013AF1C();
        break;
    case 1:
        func_8013B03C();
        break;
    case 2:
        func_8013B0C4();
        break;
    case 3:
        func_8013B2B8();
        break;
    case 4:
        func_8013B340();
        break;
    default:
        func_8013B3B4();
        break;
    }
}
