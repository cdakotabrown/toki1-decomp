/* @SymbolStart: 0x80152360 */
/* @SymbolSize: 256 */
/* Address: 0x80152368 */
/* Size: 256 bytes (64 instructions, incl. the 2-insn hoisted head) */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x24:rt:a0:v1 0x28:rs:a0:v1 0x28:rt:a0:v1 0x30:rs:a0:v1 0x34:rt:a1:a0 0x38:rs:a1:a0 0x38:rt:a1:a0 0x40:rs:a1:a0 0x58:rt:a0:v1 0x5c:rt:a1:a0 0x60:rs:a1:a0 0x60:rt:a1:a0 0x64:rs:a0:v1 0x64:rt:a0:v1 0x84:rs:a0:v1 0x8c:rs:a1:a0 0xb0:rs:a0:v1 0xb8:rs:a1:a0 */
#include "types.h"

extern u8 D_800B1746;

extern s16 D_80094730;
extern s32 D_800EECBC;

extern s32 func_80011DFC(void);

s32 func_80152368(void) {
    u8 v0_v = D_800B1746;

    s16 cnt;
    s32 flag;

    if (v0_v == 0 && D_80094730 == 10 && D_800EECBC == 0) {
        func_80011DFC();
        return 0;
    }
    cnt = D_80094730;
    flag = D_800EECBC;
    if ((v0_v == 1 || v0_v == 4 || v0_v == 9) && cnt == 6 && flag == 0) {
        func_80011DFC();
        return 0;
    }
    if (v0_v == 10 && cnt == 6 && flag == 0) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    return func_80011DFC();
}
