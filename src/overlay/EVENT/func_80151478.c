/* @SymbolStart: 0x80151470 */
/* @SymbolSize: 68 */
/* Address: 0x80151478 */
/* Size: 68 bytes (17 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern void func_800469F4();
extern void func_80011DFC();

void func_80151478(void) {
    s32 hidden_t6_v = D_800EECBC;

    if (hidden_t6_v == 0) {
        func_800469F4(17483);
    } else {
        func_800469F4(17492);
    }
    func_80011DFC();
}
