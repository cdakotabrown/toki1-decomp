/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x2c:rs:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x38:rs:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x40:rs:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t8:t9 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t8:t9 */
/* Address: 0x80140B00 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern u16 D_80094714;
extern u8 D_800B1746;

extern s32 func_8002328C();
extern void func_80011DFC();

void func_80140B00(void) {
    u32 result = func_8002328C(D_800B1746);
    if ((result & 0x7F) >= 2) {
        D_80094714 += 5;
    }
    func_80011DFC();
}
