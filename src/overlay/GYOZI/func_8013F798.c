/* @RodataBase: 0x80145D78 */
/* @SymbolStart: 0x8013F790 */
/* @SymbolSize: 280 */
/* func_8013F798 -- GYOZI overlay, Address: 0x8013F798, Size: 272 bytes. */
#include "types.h"

extern void func_80072734(s32 arg0);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern u8 D_80148750;

extern s32 D_80145D78;

void func_8013F798(void)
{
    u32 t6 = D_800F62CF;

    switch (t6) {
    case 0:
        func_80072734(22039);
        break;
    case 1:
        func_80072734(23974);
        break;
    case 2:
        func_80072734(21184);
        break;
    case 3:
        func_80072734(22849);
        break;
    case 4:
        func_80072734(26314);
        break;
    case 5:
        func_80072734(24649);
        break;
    case 6:
        func_80072734(25234);
        break;
    case 7:
        func_80072734(23479);
        break;
    case 8:
        func_80072734(20734);
        break;
    case 9:
        if (D_80148750 != 0) {
            func_80072734(25864);
        } else {
            func_80072734(25504);
        }
        break;
    case 10:
        func_80072734(26494);
        break;
    }

    func_8004DE1C();
}
