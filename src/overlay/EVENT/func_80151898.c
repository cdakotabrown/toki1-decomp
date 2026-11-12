/* @SymbolStart: 0x80151890 */
/* @SymbolSize: 244 */
/* @RodataBase: 0x8011FFB4 */
/* Address: 0x80151898 */
/* Size: 244 bytes (61 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_80125314;

extern void func_800469F4();
extern void func_80011DFC();

void func_80151898(void) {
    u8 selector_v = D_80125314;

    switch (selector_v) {
        case 0:
            func_800469F4(17266);
            break;
        case 1:
            func_800469F4(17275);
            break;
        case 2:
            func_800469F4(17284);
            break;
        case 3:
            func_800469F4(17292);
            break;
        case 4:
            func_800469F4(17302);
            break;
        case 5:
            func_800469F4(17311);
            break;
        case 6:
            func_800469F4(17319);
            break;
        case 7:
            func_800469F4(17328);
            break;
        case 8:
            func_800469F4(17337);
            break;
        case 9:
            func_800469F4(17347);
            break;
        case 10:
            func_800469F4(17357);

        default:
            break;
    }

    func_80011DFC();
}
