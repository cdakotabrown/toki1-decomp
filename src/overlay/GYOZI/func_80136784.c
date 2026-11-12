/* @SymbolSize: 212  * Status: BYTE-PERFECT 🎉 */
/* func_80136784 -- GYOZI overlay, Address: 0x80136784, Size: 212 bytes (53 insns), Semi-matched */
#include "types.h"

extern void func_8008E6AC(void);
extern void func_801388E0(void);
extern void func_80138BF0(void);
extern void func_80139920(void);
extern void func_80139D34(void);
extern void func_80052060(void);
extern void func_8008A148(void);
extern void func_8008F54C(void);
extern void func_80076450(s32);
extern void func_80074A14(void);
extern void func_80075BFC(void);
extern void func_800748B8(void);
extern void func_8008E8B4(void);

extern u8 D_800F6479;

void func_80136784(void)
{
    func_8008E6AC();
    switch (D_800F6479) {
        case 0:
            func_801388E0();
            break;
        case 1:
            func_80138BF0();
            break;
        case 2:
            func_80139920();
            break;
        case 3:
            func_80139D34();
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
