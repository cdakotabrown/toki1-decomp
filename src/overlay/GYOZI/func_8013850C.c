/* @SymbolStart: 0x801384E8 */
/* @SymbolSize: 268 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:t9 0x30:rs:v0:t9 0x38:rt:t1:t2 0x40:rs:t1:t2 0x40:rt:t1:t2 0x44:rs:t1:t2 0x48:rt:t5:t6 0x4c:rd:t9:t0 0x50:rd:t0:t1 0x50:rt:t9:t0 0x54:rs:t0:t1 0x58:rs:t5:t6 0x58:rt:t5:t6 0x60:rd:t6:t7 0x60:rt:t5:t6 0x64:rd:t7:t8 0x64:rt:t6:t7 0x68:rs:t7:t8 0x68:rt:v0:v1 0x70:rs:v0:v1 0x78:rs:v0:v1 0x7c:rd:t8:t9 0x80:rd:v1:v0 0x80:rt:t8:t9 0x84:rs:v1:v0 0x84:rt:t9:t4 0x8c:rs:t9:t4 0x94:rs:v1:v0 0x98:rt:t2:t1 0x9c:rs:t2:t1 0x9c:rt:t2:t1 0xa4:rd:t0:t2 0xa4:rt:t2:t1 0xa8:rd:t1:t5 0xa8:rt:t0:t2 0xac:rs:t1:t5 0xb4:rs:v0:v1 0xb8:rd:t4:t6 0xbc:rd:v1:v0 0xbc:rt:t4:t6 0xc0:rs:v1:v0 0xc0:rt:t5:t8 0xc8:rs:t5:t8 0xc8:rt:t6:t9 0xcc:rs:v1:v0 0xcc:rt:t6:t9 0xd0:rt:t7:t0 0xd4:rs:t7:t0 0xd4:rt:t7:t0 0xd8:rt:t9:t3 0xdc:rd:t8:t4 0xdc:rt:t7:t0 0xe0:rs:t9:t3 0xe0:rt:t9:t3 0xe4:rd:t8:t4 0xe4:rs:t8:t4 0xe4:rt:t7:t0 0xe8:rd:t8:t4 0xe8:rt:t8:t4 0xf4:rs:t8:t4 0xf4:rt:t9:t3 */
#include "types.h"

typedef struct W4 { s32 w; } W4;

extern volatile u8  D_800F5AAC;
extern u8  D_800F53A0[][56];
extern W4 D_801474B4;
extern s32 D_800F5718;
extern u8  D_800F62CF;
extern u8  D_801474B8;

extern void func_80090960(s32, s32);
extern void func_80050D60(s32, s32);
extern void func_80081D30(s32);

void func_8013850C(void) {
    u8 v1;

    v1 = D_800F53A0[0][D_800F5AAC * 4 + 1677];
    *(W4 *)&D_800F53A0[v1][432] = D_801474B4;

    v1 = D_800F53A0[0][D_800F5AAC * 4 + 1677];
    if (v1 != 8) {
        D_800F53A0[v1][440] |= 2;
    } else if (((u32)(D_800F5718 << 20) >> 29) == 3) {
        D_800F53A0[v1][440] |= 2;
    }

    func_80090960(8, D_800F62CF * 10 + D_801474B8);
    func_80050D60(0, 0);
    func_80081D30(1);
}
