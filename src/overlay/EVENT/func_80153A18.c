/* @SymbolStart: 0x80153A10 */
/* @SymbolSize: 88 */
/* Address: 0x80153A18 */
/* Size: 88 bytes (22 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_801252E4;

extern u8 D_801243B4;

extern s32 func_80011DFC();
extern void func_80117A68(void);

s32 func_80153A18(void) {
    u8 t6_param_v = D_801252E4;

    if (t6_param_v != 0) goto L_dfc;
    if (D_801243B4 != 1) goto L_other;
L_dfc:
    func_80011DFC();
    func_80011DFC();
    return 0;
L_other:
    func_80117A68();
    return;
}
