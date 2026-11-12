/* @SymbolStart: 0x80135188 */
/* @SymbolSize: 176 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:t6 0x1c:rs:v0:t6 0x1c:rt:t6:t7 0x24:rs:t6:t7 0x28:rd:v1:v0 0x28:rs:t6:t7 0x2c:rs:t6:t7 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x30:rt:t7:t8 0x34:rd:v1:v0 0x34:rs:t7:t8 0x38:rd:t8:t9 0x38:rt:v1:v0 0x3c:rt:t9:t0 0x40:rs:t9:t0 0x40:rt:t9:t0 0x44:rd:t8:t9 0x44:rs:t8:t9 0x44:rt:v1:v0 0x50:rt:v1:v0 0x5c:rs:t8:t9 0x5c:rt:t9:t0 0x64:rt:v1:v0 0x68:rt:t0:t1 0x6c:rs:v1:v0 0x70:rt:t2:t3 0x74:rs:t0:t1 0x74:rt:t0:t1 0x7c:rs:t0:t1 0x7c:rt:t1:t2 0x80:rt:t1:t2 0x88:rt:v1:v0 0x90:rs:v1:v0 0x98:rs:t2:t3 0x98:rt:t2:t3 0xa0:rs:t2:t3 0xa0:rt:t3:t4 0xa4:rt:t3:t4 */
/* func_80135190 -- GYOZI overlay, Address: 0x80135188, Size: 176 bytes (44 insns) */
#include "types.h"

extern void func_800BCE10(void *a0, void *a1);
extern void func_8008FB00(void);

extern s32 D_800F6468;
extern s16 D_800F54B6;
extern u8 D_80146178;

extern s8 D_800D9320[];

extern s8 D_8014617C[];

void func_80135190(void)
{
    s32 param = D_800F6468;
    s32 v0;

    v0 = param & 0x7;
    if (v0 != 0) {
        v0 = (v0 - 1) & 0xFF;
    }

    D_80146178 = (u8)v0;
    func_800BCE10(D_800D9320, &D_8014617C[v0 * 5]);

    v0 = D_80146178;
    if (v0 == 0) {
        D_800F54B6 = D_800F54B6 - 5;
    }
    D_80146178 = (u8)v0;
    if (v0 == 6) {
        D_800F54B6 = D_800F54B6 + 5;
    }
    func_8008FB00();
}
