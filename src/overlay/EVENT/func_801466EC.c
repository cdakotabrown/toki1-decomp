/* @SymbolStart: 0x801466E4 */
/* @SymbolSize: 172 */
/* @PostProc: patch_insn_reg_field 0x30:rt:a0:v1 0x34:rs:a0:v1 0x34:rt:a0:v1 0x38:rs:a0:v1 0x48:rs:a0:v1 0x60:rt:v1:t6 0x68:rs:v1:t6 0x68:rt:v1:t6 0x70:rt:a0:v1 0x7c:rd:t6:t7 0x7c:rt:v1:t6 0x80:rs:a0:v1 0x80:rt:a0:v1 0x88:rd:v1:t8 0x88:rt:t6:t7 0x8c:rs:a0:v1 0x8c:rt:t8:t2 0x90:rs:v1:t8 0x90:rt:t0:t9 0x94:rd:t1:t0 0x94:rt:t0:t9 0x98:rs:t1:t0 0x98:rt:t2:t1 0x9c:rs:t8:t2 0x9c:rt:t9:t3 0xa0:rd:t3:t4 0xa0:rs:t9:t1 0xa0:rt:t2:t3 0xa8:rs:a0:v1 0xa8:rt:t3:t4 */
/* Address: 0x801466EC (window starts at the absorbed cross-prologue head */
/* Size: 172 bytes (43 insns) including the head */
#include "types.h"

extern volatile u8 D_800B0860[];
extern volatile s32 D_800B0C4C;
extern u16 D_80094714;
extern void func_80011DFC(void);

void func_801466EC(void)
{
    u32 sel;
    u32 tmp;

    sel = ((u32)D_800B0C4C << 20) >> 29;
    if (sel >= 5) {
        D_800B0860[1005] = (D_800B0860[1005] & 0xFFF1) | 0x0008;
        sel = ((u32)D_800B0C4C << 20) >> 29;
    }
    D_80094714 = D_80094714 + (sel << 1);
    tmp = ((u32)D_800B0C4C << 20) >> 29;
    D_800B0860[1005] = (D_800B0860[1005] & 0xFFF1) | (((tmp + 1) << 1) & 0x000E);
    func_80011DFC();
}
