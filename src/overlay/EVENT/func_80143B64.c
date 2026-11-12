/* @SymbolStart: 0x80143B5C */
/* @SymbolSize: 84 */
/* Address: 0x80143B64 */
/* Size: 84 bytes (21 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B0E43;

extern u16 D_80094714;
extern u16 D_80094718;

extern void func_80011DFC(void);

void func_80143B64(void) {
    u8 t6_param_v = D_800B0E43;

    if (t6_param_v != 0) {
        D_80094714 = D_80094714 + 1;
        D_80094718 = D_80094718 + 1;
    }
    func_80011DFC();
}
