/* @RodataBase: 0x80145CB4 */
/* @SymbolStart: 0x8013DE2C */
/* @SymbolSize: 428 */
/* func_8013DE34 -- GYOZI overlay, Address: 0x8013DE34, Size: 416 bytes (104 insns), Semi-matched */
#include "types.h"

extern u8 D_800F62CF;

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_80052060(void);
extern void func_8013ED84(void);
extern void func_8004DE1C(void);

extern s32 D_80145CB4;

void func_8013DE34(void)
{
    u32 t6 = D_800F62CF;

    switch (t6) {
    case 0:
        func_80051DD8(6, 0x80198000, 0xA2E3);
        break;
    case 1:
        func_80051DD8(3, 0x80198000, 0xA2F6);
        break;
    case 2:
        func_80051DD8(4, 0x80198000, 0xA2E9);
        break;
    case 3:
        func_80051DD8(5, 0x80198000, 0xA2ED);
        break;
    case 4:
        func_80051DD8(6, 0x80198000, 0xA309);
        break;
    case 5:
        func_80051DD8(5, 0x80198000, 0xA2F9);
        break;
    case 6:
        func_80051DD8(6, 0x80198000, 0xA2FE);
        break;
    case 7:
        func_80051DD8(4, 0x80198000, 0xA2F2);
        break;
    case 8:
        func_80051DD8(4, 0x80198000, 0xA2DF);
        break;
    case 9:
        func_80051DD8(5, 0x80198000, 0xA304);
        break;
    case 10:
        func_80051DD8(6, 0x80198000, 0xA316);
        break;
    case 11:
        func_80051DD8(7, 0x80198000, 0xA30F);
        break;
    case 13:
        break;
    case 12:
    default:
        func_80052060();
        break;
    }
    func_8013ED84();
    func_8004DE1C();
}
