/* @SymbolStart: 0x8013977C */
/* @SymbolSize: 64 */
/* Address: 0x80139784 */
/* Size: 64 bytes (16 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern u16 D_80094714;

extern void func_80011DFC();

void func_80139784(void) {
    s32 hidden_t6_v = D_800EECBC;

    if (hidden_t6_v != 0) {
        D_80094714 += 5;
    }
    func_80011DFC();
}
