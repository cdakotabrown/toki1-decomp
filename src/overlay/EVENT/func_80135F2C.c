/* @SymbolStart: 0x80135F24 */
/* @SymbolSize: 428 */
/* @RodataBase: 0x8011F628 */
/* Address: 0x80135F2C */
/* Size: 428 bytes (107 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern void func_80015D28();
extern void func_80015FE0();
extern void func_800F6000();
extern void func_80011DFC();

void func_80135F2C(void) {
    u8 selector_v = D_800B1746;

    switch (selector_v) {
        case 0:
            func_80015D28(6, 0x80198000, 0xA191);
            break;
        case 1:
            func_80015D28(3, 0x80198000, 0xA1A4);
            break;
        case 2:
            func_80015D28(4, 0x80198000, 0xA197);
            break;
        case 3:
            func_80015D28(5, 0x80198000, 0xA19B);
            break;
        case 4:
            func_80015D28(6, 0x80198000, 0xA1B7);
            break;
        case 5:
            func_80015D28(5, 0x80198000, 0xA1A7);
            break;
        case 6:
            func_80015D28(6, 0x80198000, 0xA1AC);
            break;
        case 7:
            func_80015D28(4, 0x80198000, 0xA1A0);
            break;
        case 8:
            func_80015D28(4, 0x80198000, 0xA18D);
            break;
        case 9:
            func_80015D28(5, 0x80198000, 0xA1B2);
            break;
        case 10:
            func_80015D28(6, 0x80198000, 0xA1C4);
            break;
        case 11:
            func_80015D28(7, 0x80198000, 0xA1BD);
            break;
        case 12:
        case 13:
        default:
            func_80015FE0();
            break;
    }

    func_800F6000();
    func_80011DFC();
}
