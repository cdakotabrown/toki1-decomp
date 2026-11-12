/* @SymbolStart: 0x80150BFC */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 */
/* Address: 0x80150C04 */
/* Size: 96 bytes (24 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_8012531C;

extern void func_800469F4(s32);
extern void func_80011DFC(void);

void func_80150C04(void) {
    u8 mode_v = D_8012531C;

    if (mode_v == 0) {
        func_800469F4(17033);
    } else if (mode_v == 1) {
        func_800469F4(17024);
    } else {
        func_800469F4(17042);
    }
    func_80011DFC();
}
