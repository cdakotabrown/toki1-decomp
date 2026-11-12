/* @SymbolStart: 0x8013FBF8 */
/* @SymbolSize: 68 */
/* @PostProc: patch_insn_reg_field 0x1c:rd:t7:t8 0x20:rs:t7:t8 0x28:rt:t8:t9 0x2c:rs:t8:t9 0x2c:rt:t8:t9 0x34:rs:t8:t9 0x34:rt:t9:t0 0x38:rt:t9:t0 */
/* Address: 0x8013FC00 */
/* Size: 68 bytes (17 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800B0B14;

extern u16 D_80094714;

extern void func_80011DFC();

void func_8013FC00(void) {
    s32 t6_param_v = D_800B0B14;

    if ((t6_param_v << 17) < 0) {
        D_80094714 = D_80094714 + 1;
    }
    func_80011DFC();
}
