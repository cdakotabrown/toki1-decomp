/* @SymbolStart: 0x80156B94 */
/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x38:rt:v1:v0 0x3c:rs:v1:v0 */
/* Address: 0x80156B9C */
/* Size: 132 bytes (33 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1AF6;

extern void func_800433D0();
extern void func_80011DFC();
extern u8 D_80125324;

void func_80156B9C(void) {
    u8 a0_v = D_800B1AF6;

    u8 v0;

    if (a0_v < 20U) goto L1;
    func_800433D0(515);
    goto END;
L1:
    v0 = D_80125324;
    if (v0 != 0) goto L2;
    func_800433D0(512);
    goto END;
L2:
    if (v0 != 1) goto L3;
    func_800433D0(513);
    goto END;
L3:
    func_800433D0(514);
END:
    func_80011DFC();
}
