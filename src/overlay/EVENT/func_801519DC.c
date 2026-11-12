/* @SymbolStart: 0x801519D4 */
/* @SymbolSize: 260 */
/* @RodataBase: 0x8011FFE0 */
/* Address: 0x801519DC */
/* Size: 260 bytes (65 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_80125318;

extern void func_800469F4();
extern void func_80011DFC();

void func_801519DC(void) {
    u8 selector_v = D_80125318;

    switch (selector_v) {
        case 0:
            func_800469F4(17456);
            break;
        case 1:
            func_800469F4(17385);
            break;
        case 2:
            func_800469F4(17422);
            break;
        case 3:
            func_800469F4(17440);
            break;
        case 4:
            func_800469F4(17395);
            break;
        case 5:
            func_800469F4(17413);
            break;
        case 6:
            func_800469F4(17456);
            break;
        case 7:
            func_800469F4(17431);
            break;
        case 8:
            func_800469F4(17404);
            break;
        case 9:
            func_800469F4(17456);
            break;
        case 10:
            func_800469F4(17448);
            break;
        case 11:
            func_800469F4(17465);

        default:
            break;
    }

    func_80011DFC();
}
