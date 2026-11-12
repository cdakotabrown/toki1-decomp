/* @SymbolStart: 0x80142A74 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v1:t1 0x40:rs:v1:t1 0x40:rt:v1:t1 0x44:rt:a1:a0 0x48:rs:v1:t1 0x48:rt:a1:a0 0x54:rt:a0:v1 0x58:rs:a0:v1 0x58:rt:a0:v1 0x68:rt:v1:t5 0x6c:rd:t1:t2 0x70:rs:a0:v1 0x70:rt:t1:t2 0x74:rt:t2:t3 0x7c:rs:t2:t3 0x7c:rt:t3:t4 0x80:rt:t3:t4 0x84:rs:v1:t5 0x84:rt:v1:t5 0x8c:rs:v1:t5 0x8c:rt:a1:a0 0x90:rd:t4:t6 0x94:rs:a0:v1 0x94:rt:t4:t6 0x98:rt:t5:t7 0xa0:rs:t5:t7 0xa0:rt:t6:t8 0xa8:rt:t6:t8 */
/* func_80142A7C -- GYOZI overlay, Address: 0x80142A74, Size: 180 bytes (45 insns) */
#include "types.h"

extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern volatile u16 D_800D9250;
extern volatile u8 D_800F62CF;
extern volatile u8 D_800F647A;
extern s16 D_800F53A0[][28];

void func_80142A7C(void)
{
    s32 cond = D_8012E66C;
    s32 idx;

    if (cond == 0) {
        D_800F647A = D_800F647A + 24;
    } else {
        idx = D_800F62CF;
        D_800D9250 = D_800D9250 + 2;
        D_800F53A0[idx][215] = D_800F53A0[idx][215] + 1;
        idx = D_800F62CF;
        D_800F53A0[idx][217] = D_800F53A0[idx][217] + 2;
        func_8008FB00();
        func_8004DE1C();
    }
}
