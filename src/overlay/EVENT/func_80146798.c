/* @SymbolStart: 0x80146790 */
/* @SymbolSize: 80 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v0 0x14:rs:a0:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x28:rt:v1:v0 0x38:rt:v1:v0 0x40:rs:v1:v0 */
/* Address: 0x80146798, Size: 72 bytes (18 instructions) */
#include "types.h"

extern u8 D_800B0E72;

extern u16 D_80094714;

extern void func_80011DFC(void);

void func_80146798(void) {
    u8 v0_param_v = D_800B0E72;

    if ((u32)v0_param_v >= 2) {
        v0_param_v = 1;
    }
    D_80094714 = (u16)(D_80094714 + v0_param_v);
    D_800B0E72 = (u8)(v0_param_v + 1);
    func_80011DFC();
}
