/* @SymbolStart: 0x80155AFC */
/* @SymbolSize: 92 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x20:rs:v1:v0 0x2c:rs:v1:v0 0x34:rs:v1:v0 0x3c:rs:v1:v0 */
/* Address: 0x80155B04 */
/* Size: 92 bytes (23 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_800B1AF6;

extern void func_80011DFC();

void func_80155B04(void) {
    u8 v0_v = D_800B1746;

    if (v0_v == 2 || v0_v == 8 || v0_v == 5 || v0_v == 10) {

        D_800B1AF6 += 5;
    }

    func_80011DFC();
}
