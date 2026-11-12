/* @SymbolStart: 0x80144734 */
/* @SymbolSize: 116 */
/* Address: 0x8014473C */
/* Size: 116 bytes (29 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B0E6A;

extern void func_80011DFC(void);

void func_8014473C(void) {
    u8 hidden_t6_v = D_800B0E6A;

    if (hidden_t6_v == 0) goto skip;
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
skip:
    func_80011DFC();
}
