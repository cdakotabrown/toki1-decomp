/* @SymbolSize: 164 */
/* func_8013C0A4 -- GYOZI overlay, Address: 0x8013C0A4, Size: 164 bytes (41 insns) */
#include "types.h"

extern void func_8008E6AC(void);
extern void func_8013E1D0(void);
extern void func_8013E5B0(void);
extern void func_80052060(void);
extern void func_8008A148(void);
extern void func_8008F54C(void);
extern void func_80076450(s32 a0);
extern void func_80074A14(void);
extern void func_80075BFC(void);
extern void func_800748B8(void);
extern void func_8008E8B4(void);

extern u8 D_800F6479;

void func_8013C0A4(void)
{
    func_8008E6AC();

    switch (D_800F6479) {
    case 0:
        func_8013E1D0();
        break;
    case 1:
        func_8013E5B0();
        break;
    default:
        func_80052060();
        break;
    }

    func_8008A148();
    func_8008F54C();
    func_80076450(2);
    func_80074A14();
    func_80075BFC();
    func_800748B8();
    func_8008E8B4();
}
