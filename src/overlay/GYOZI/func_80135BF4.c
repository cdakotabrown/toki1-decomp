/* @SymbolStart: 0x80135BEC */
/* @SymbolSize: 192 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:t7 0x28:rs:v1:t7 0x28:rt:v1:t7 0x2c:rt:a1:a0 0x30:rs:v1:t7 0x30:rt:a1:a0 0x34:rt:a0:v1 0x38:rs:a0:v1 0x38:rt:a0:v1 0x3c:rt:v1:t1 0x40:rd:t7:t8 0x44:rs:a0:v1 0x44:rt:t7:t8 0x48:rt:t8:t9 0x50:rs:t8:t9 0x50:rt:t9:t0 0x54:rt:t9:t0 0x58:rs:v1:t1 0x58:rt:v1:t1 0x60:rs:v1:t1 0x60:rt:a1:a0 0x64:rd:t0:t2 0x68:rs:a0:v1 0x68:rt:t0:t2 0x6c:rt:t1:t3 0x74:rs:t1:t3 0x74:rt:t2:t4 0x7c:rt:t2:t4 0x80:rt:t3:t5 0x84:rs:t3:t5 0x84:rt:t3:t5 0x8c:rs:t3:t5 0x8c:rt:t4:t6 0x94:rt:t4:t6 0x98:rt:t5:t7 0x9c:rs:t5:t7 0x9c:rt:t5:t7 0xa4:rs:t5:t7 0xa4:rt:t6:t8 0xa8:rt:t6:t8 */
/* func_80135BF4 -- GYOZI overlay, Address: 0x80135BF4, Size: 184 bytes (46 insns) */
#include "types.h"

extern s32 D_8012E66C;
extern volatile u8 D_800F62CF;
extern s16 D_800F53A0[][28];
extern volatile u8 D_800F647A;
extern volatile s16 D_800D9248;

extern void func_8008FB00(void);
extern void func_80090960(s32, s32);
extern void func_8004DE1C(void);

void func_80135BF4(void)
{
    s32 t6 = D_8012E66C;
    s32 idx;

    if (t6 != 0) {
        idx = D_800F62CF;
        D_800F53A0[idx][217] = D_800F53A0[idx][217] - 1;
        idx = D_800F62CF;
        D_800F53A0[idx][219] = D_800F53A0[idx][219] + 5;
        func_8008FB00();
        D_800F647A = D_800F647A + 11;
    } else {
        D_800D9248 = D_800D9248 + 1;
        func_80090960(10, 41);
        func_8004DE1C();
    }
}
