/* @SymbolStart: 0x801565A0 */
/* @SymbolSize: 76 */
/* Address: 0x801565A8 */
/* Size: 76 bytes (19 instructions, incl. the 2-insn hoisted head) */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x20:rs:v1:v0 0x2c:rs:v1:v0 */
#include "types.h"

extern u8 D_800B1746;

extern void func_8004A764(void);
extern void func_8004A734(void);

void func_801565A8(void) {
    u8 hidden_v0_v = D_800B1746;

    s32 at;

    at = 5;
    if (hidden_v0_v == at) goto call_764;
    at = 7;
    if (hidden_v0_v != at) goto call_734;
call_764:
    func_8004A764();
    return;
call_734:
    func_8004A734();
}
