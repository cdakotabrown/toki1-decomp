/* @SymbolStart: 0x80155DA0 */
/* @SymbolSize: 140 */
/* @PostProc: patch_insn_reg_field 0x60:rd:v0:v1 0x38:rt:v1:v0 0x3c:rs:v1:v0 0x3c:rt:v1:v0 0x44:rs:v1:v0 0x4c:rs:v1:v0 0x60:rd:v1:v0 */
/* Address: 0x80155DA8 */
/* Size: 140 bytes (35 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_8012531C;

extern void func_8004A7F4(void);
extern void func_80011DFC(void);

s32 func_80155DA8(void) {
    u8 t6_v = D_800B1746;

    u8 branch;

    if (t6_v != 7) goto fallback;
    if (D_800EECBC != 1) goto fallback;
    branch = D_8012531C;
    if (branch == 2) goto do_action;
    if (branch != 0) goto fallback;
do_action:
    func_8004A7F4();
    return 0;
fallback:
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}
