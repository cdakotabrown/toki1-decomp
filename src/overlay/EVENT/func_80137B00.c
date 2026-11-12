/* @SymbolStart: 0x80137AF8 */
/* @SymbolSize: 244 */
/* @RodataBase: 0x8011F660 */
#include "types.h"

extern u8 D_800B1746;

extern void func_800335E0(s32 a0);
extern void func_80011DFC(void);

void func_80137B00(void) {
    u8 cmd_v = D_800B1746;

    switch (cmd_v) {
    case 0:
        func_800335E0(22022);
        break;
    case 1:
        func_800335E0(23957);
        break;
    case 2:
        func_800335E0(21167);
        break;
    case 3:
        func_800335E0(22832);
        break;
    case 4:
        func_800335E0(26252);
        break;
    case 5:
        func_800335E0(24632);
        break;
    case 6:
        func_800335E0(25217);
        break;
    case 7:
        func_800335E0(23462);
        break;
    case 8:
        func_800335E0(20717);
        break;
    case 9:
        func_800335E0(25667);
        break;
    case 10:
        func_800335E0(26657);
        break;
    }
    func_80011DFC();
}
