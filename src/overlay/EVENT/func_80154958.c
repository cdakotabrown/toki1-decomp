/* @SymbolStart: 0x80154950 */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 */
/* Address: 0x80154958 */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_801243B0;

extern void func_80118A60(void);
extern void func_80011DFC(void);
extern void func_801189B4(void);

void func_80154958(void) {
    u8 hidden_v0_v = D_801243B0;

    s32 at;

    at = 18;
    if (hidden_v0_v == at) {
        func_80118A60();
        func_80011DFC();
        goto end;
    }
    at = 20;
    if (hidden_v0_v != at) goto end;
    func_801189B4();
    func_80011DFC();
end:
    func_80011DFC();
}
