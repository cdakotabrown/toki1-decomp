/* @RodataBase: 0x80145C90 */
/* @SymbolSize: 296 */
/* Address: 0x8013DBF8 */
/* Size: 296 bytes (74 instructions) */
#include "types.h"

extern void func_8008E6AC(void);
extern void func_801400D0(void);
extern void func_80140780(void);
extern void func_80140C90(void);
extern void func_80141944(void);
extern void func_80142D40(void);
extern void func_80143160(void);
extern void func_80143D20(void);
extern void func_801447F0(void);
extern void func_80145384(void);
extern void func_80052060(void);
extern void func_8008A148(void);
extern void func_8008F54C(void);
extern void func_80076450(s32);
extern void func_80074A14(void);
extern void func_80075BFC(void);
extern void func_800748B8(void);
extern void func_8008E8B4(void);
extern void func_800530E0(void);

extern u8 D_800F6479;

void func_8013DBF8(void)
{
    func_8008E6AC();
    switch (D_800F6479) {
    case 0:
        func_801400D0();
        break;
    case 1:
        func_80140780();
        break;
    case 2:
        func_80140C90();
        break;
    case 3:
        func_80141944();
        break;
    case 4:
        func_80142D40();
        break;
    case 5:
        func_80143160();
        break;
    case 6:
        func_80143D20();
        break;
    case 7:
        func_801447F0();
        break;
    case 8:
        func_80145384();
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
    func_800530E0();
}
