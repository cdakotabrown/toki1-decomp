/* @SymbolStart: 0x80154A60 */
/* @SymbolSize: 236 */
/* @RodataBase: 0x80120060 */
#include "types.h"

extern u8 D_800B1746;

extern void func_800335E0(s32 a0);

void func_80154A68(void) {
    u8 cmd_v = D_800B1746;

    switch (cmd_v) {
    case 0:
        func_800335E0(22036);
        break;
    case 1:
        func_800335E0(23656);
        break;
    case 2:
        func_800335E0(20956);
        break;
    case 3:
        func_800335E0(22846);
        break;
    case 4:
        func_800335E0(25906);
        break;
    case 5:
        func_800335E0(24016);
        break;
    case 6:
        func_800335E0(24871);
        break;
    case 7:
        func_800335E0(23521);
        break;
    case 8:
        func_800335E0(20371);
        break;
    case 9:
        func_800335E0(25366);
        break;
    case 10:
        func_800335E0(26221);
        break;
    }
}
