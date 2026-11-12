/* @RodataBase: 0x80145840 */
/* @SymbolSize: 256 */
/* Address: 0x801322A4 */
/* Size: 256 bytes (64 instructions) */
#include "types.h"

extern void func_8008E6AC(void);
extern void func_80134520(void);
extern void func_80134C10(void);
extern void func_80134FD0(void);
extern void func_80135900(void);
extern void func_80135CA0(void);
extern void func_801362A0(void);
extern void func_80137798(void);
extern void func_80052060(void);
extern void func_8008A148(void);
extern void func_8008F54C(void);
extern void func_80076450(s32);
extern void func_80074A14(void);
extern void func_80075BFC(void);
extern void func_800748B8(void);
extern void func_8008E8B4(void);

extern u8 D_800F6479;

void func_801322A4(void)
{
    func_8008E6AC();
    switch (D_800F6479) {
    case 0:
        func_80134520();
        break;
    case 1:
        func_80134C10();
        break;
    case 2:
        func_80134FD0();
        break;
    case 3:
        func_80135900();
        break;
    case 4:
        func_80135CA0();
        break;
    case 5:
        func_801362A0();
        break;
    case 6:
        func_80137798();
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
