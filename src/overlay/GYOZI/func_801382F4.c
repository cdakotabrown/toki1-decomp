/* @SymbolSize: 500 */
/* @RodataBase: 0x80145AB0 */
/* func_801382F4 -- GYOZI overlay, Address: 0x801382F4, Size: 500 bytes (125 insns) */
#include "types.h"

extern void func_8008E6AC(void);
extern void func_8013A5F4(void);
extern void func_8013A76C(void);
extern void func_8013A7C8(void);
extern void func_80089244(void);
extern void func_8004DDD8(void);
extern void func_80052060(void);
extern void func_8013C4D0(void);
extern void func_8013AE80(void);
extern void func_8013B920(void);
extern void func_8013D550(void);
extern void func_8013CDD0(void);
extern void func_8013D120(void);
extern void func_8013BFB4(void);
extern void func_8013A920(void);
extern void func_8013DB50(void);
extern void func_8013A4E8(void);
extern void func_8008A148(void);
extern void func_8008F54C(void);
extern void func_80076450(s32 arg0);
extern void func_80074A14(void);
extern void func_80075BFC(void);
extern void func_800748B8(void);
extern void func_8008E8B4(void);
extern void func_800530E0(void);

extern u8 D_800F6479;
extern u8 D_800F647A;
extern u8 D_801474B0;

void func_801382F4(void)
{
    func_8008E6AC();

    switch ((u32)D_800F6479) {
    case 0:
        func_8013A5F4();
        break;
    case 1:
        func_8013A76C();
        break;
    case 2:
        func_8013A7C8();
        break;
    case 3:
        func_80089244();
        if (D_800F647A != 0) {
            func_8004DDD8();
        }
        break;
    case 4:
        switch ((u32)D_801474B0) {
        case 0:
            func_80052060();
            break;
        case 1:
            func_8013C4D0();
            break;
        case 2:
            func_8013AE80();
            break;
        case 3:
            func_8013B920();
            break;
        case 4:
            func_8013D550();
            break;
        case 5:
            func_8013CDD0();
            break;
        case 6:
            func_8013D120();
            break;
        case 7:
            func_8013BFB4();
            break;
        case 8:
            func_8013A920();
            break;
        case 9:
            func_80052060();
            break;
        case 10:
            func_8013DB50();
            break;
        default:
            func_80052060();
            break;
        }
        break;
    case 5:
        func_8013A4E8();
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
