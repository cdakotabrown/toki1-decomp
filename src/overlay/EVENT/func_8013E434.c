/* @SymbolStart: 0x8013E42C */
/* @SymbolSize: 88 */
/* @PostProc: rename_register_chain --chain t6:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:t6 0x14:rs:t7:t6 0x14:rt:t7:t6 0x1c:rs:t7:t6 0x24:rt:t8:t7 0x28:rs:t8:t7 0x28:rt:t8:t7 0x30:rs:t8:t7 0x40:rt:t9:t0 0x48:rs:t9:t0 0x48:rt:t0:t1 0x4c:rt:t0:t1 */
/* Address: 0x8013E434 */
/* Size: 88 bytes (22 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern u16 D_80094714;
extern u8 D_800B0A45;

extern void func_80011DFC(void);

void func_8013E434(void) {
    s32 a0_v = D_800EECBC;

    if (a0_v != 0) {
        D_80094714 = D_80094714 + 1;
    } else {
        D_800B0A45 = D_800B0A45 | 0x40;
    }
    func_80011DFC();
}
