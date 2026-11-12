/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x30:rs:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x34:rs:v1:t6 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x38:rs:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x3c:rs:v1:t6 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t1:t0 */
/* @PostProc: patch_insn_reg_field 0x44:rs:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x48:rt:t1:t0 */
/* @PostProc: patch_insn_reg_field 0x48:rd:t2:t1 */
/* @PostProc: patch_insn_reg_field 0x4c:rs:t2:t1 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:t3:t2 */
/* @PostProc: patch_insn_reg_field 0x58:rt:t3:t2 */
/* Address: 0x8015251C */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern u8  D_800B1746;
extern s16 D_80094730;

extern s32  func_8002328C(s32 a0);
extern void func_80011DFC(void);

void func_8015251C(void)
{
    s32 v0;
    s32 m;
    u32 ge2;
    u32 ge3;
    s32 t9;
    s32 t1;

    v0 = func_8002328C((s32)D_800B1746);
    m = v0 & 0x7F;
    ge2 = (u32)m < 2U;
    ge2 = ge2 ^ 1U;
    ge3 = (u32)m < 3U;
    ge3 = ge3 ^ 1U;
    t9  = (s32)D_80094730 - (s32)ge2;
    t1  = t9 - (s32)ge3;
    D_80094730 = (s16)(t1 + 2);
    func_80011DFC();
}
