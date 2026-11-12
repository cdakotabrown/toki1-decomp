/* @SymbolStart: 0x80156FFC */
/* @SymbolSize: 80 */
/* Address: 0x80157004 */
/* Size: 80 bytes (20 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_801252E4;

extern u8 D_801243B4;

extern void func_80046B88(s32 a0);
extern void func_80011DFC(void);

void func_80157004(void) {
    u8 t6_param_v = D_801252E4;

    if (t6_param_v != 0) {
        goto do_dfc;
    }
    if (D_801243B4 != 0) {
        goto do_dfc;
    }
    func_80046B88(128);
    return;

do_dfc:
    func_80011DFC();
}
