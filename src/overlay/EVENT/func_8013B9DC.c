/* @SymbolStart: 0x8013B9D4 */
/* @SymbolSize: 60 */
/* Address: 0x8013B9DC */
/* Size: 60 bytes (15 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern void func_80011DFC();

void func_8013B9DC(void) {
    s32 hidden_t6_v = D_800EECBC;

    if (hidden_t6_v == 0) {
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}
