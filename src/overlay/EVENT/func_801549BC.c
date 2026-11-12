/* @SymbolStart: 0x801549B4 */
/* @SymbolSize: 172 */
/* @RodataBase: 0x8012003C */
/* Address: 0x801549BC, Size: 164 bytes (41 insns) */
#include "types.h"

extern u8 D_800B1746;

extern void func_800335E0(s32);

void func_801549BC(void) {
    u8 sel_v = D_800B1746;

    switch (sel_v) {
    case 0:
        func_800335E0(21946);
        break;
    case 1:
        func_800335E0(25816);
        break;
    case 2:
        func_800335E0(24196);
        break;
    case 3:
        func_800335E0(24781);
        break;
    case 4:
        func_800335E0(23386);
        break;
    case 5:
        func_800335E0(20686);
        break;
    case 6:
    case 7:
    case 8:
    default:
        func_800335E0(26401);
        break;
    }
}
