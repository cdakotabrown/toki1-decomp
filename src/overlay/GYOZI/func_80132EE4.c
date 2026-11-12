/* @SymbolStart: 0x80132EDC */
/* @SymbolSize: 244 */
/* @RodataBase: 0x801458A8 */
/* func_80132EE4 -- GYOZI overlay, Address: 0x80132EDC, Size: 244 bytes (61 insns) */
#include "types.h"

extern void func_80072734(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;

void func_80132EE4(void) {
    u32 cmd = D_800F62CF;

    switch (cmd) {
    case 0:
        func_80072734(22129);
        break;
    case 1:
        func_80072734(23884);
        break;
    case 2:
        func_80072734(21094);
        break;
    case 3:
        func_80072734(22759);
        break;
    case 4:
        func_80072734(26179);
        break;
    case 5:
        func_80072734(24694);
        break;
    case 6:
        func_80072734(25099);
        break;
    case 7:
        func_80072734(23344);
        break;
    case 8:
        func_80072734(20509);
        break;
    case 9:
        func_80072734(25729);
        break;
    case 10:
        func_80072734(26764);
        break;
    }
    func_8004DE1C();
}
