/* @SymbolStart: 0x801446F4 */
/* @SymbolSize: 64 */
/* Address: 0x801446FC */
/* Size: 64 bytes (16 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B0E6A;

extern u16 D_80094714;

extern void func_80011DFC(void);

void func_801446FC(void) {
    u8 hidden_t6_v = D_800B0E6A;

    if (hidden_t6_v != 0) {
        D_80094714 += 5;
    }
    func_80011DFC();
}
