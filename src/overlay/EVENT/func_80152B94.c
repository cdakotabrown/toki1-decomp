/* @SymbolSize: 288 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x54 */
/* @PostProc: patch_insn_reg_field 0x34:rt:a1:t9 0x38:rs:a1:t9 0x38:rt:a1:t9 0x3c:rt:a0:v1 0x40:rs:a1:t9 0x40:rt:a0:v1 0x44:rt:t0:t1 0x48:rs:t0:t1 0x48:rt:t0:t1 0x64:rd:t9:t0 0x68:rd:t1:t2 0x68:rs:t9:t0 0x68:rt:t0:t1 0x6c:rd:t2:t3 0x6c:rs:t1:t2 0x70:rs:a1:t9 0x70:rt:a0:v1 0x74:rs:t2:t3 0x74:rt:t3:t4 0x7c:rt:t3:t4 0x84:rd:t4:t5 0x88:rd:t5:t6 0x88:rs:t4:t5 0x88:rt:t0:t1 0x8c:rd:t6:t7 0x8c:rs:t5:t6 0x90:rs:a1:t9 0x90:rt:a0:v1 0x94:rs:t6:t7 0x94:rt:t7:t8 0x98:rt:t4:t6 0x9c:rs:t4:t6 0x9c:rt:t4:t6 0xa0:rt:t7:t8 0xa8:rd:t8:t0 0xac:rd:t9:t2 0xac:rs:t8:t0 0xac:rt:t0:t1 0xb0:rd:t1:t3 0xb0:rs:t9:t2 0xb4:rs:a1:t9 0xb4:rt:a0:v1 0xb8:rs:t1:t3 0xb8:rt:t2:t4 0xc0:rt:t2:t4 0xc8:rd:t3:t5 0xcc:rd:t5:t7 0xcc:rs:t3:t5 0xcc:rt:t4:t6 0xd0:rd:t6:t8 0xd0:rs:t5:t7 0xd4:rs:a1:t9 0xd4:rt:a0:v1 0xd8:rs:t6:t8 0xd8:rt:t7:t0 0xe0:rt:t7:t0 0xe8:rd:t8:t1 0xec:rd:t0:t2 0xec:rs:t8:t1 0xec:rt:t4:t6 0xf0:rd:t9:t3 0xf0:rs:t0:t2 0xf4:rs:a1:t9 0xf4:rt:a0:v1 0xf8:rs:t9:t3 0xf8:rt:t1:t4 0x100:rt:t1:t4 0x108:rd:t2:t5 0x10c:rd:t3:t7 0x10c:rs:t2:t5 0x10c:rt:t4:t6 0x110:rd:t5:t8 0x110:rs:t3:t7 0x114:rs:t5:t8 0x114:rt:t6:t0 0x11c:rt:t6:t0 */
/* Function: func_80152B94 */
/* Overlay: EVENT */
#include "types.h"

extern s32 func_80011DFC();
extern s32 func_80114B0C();

extern volatile u8 D_800B1AF6;
extern volatile u8 D_800B1746;
extern u8 *D_80125260;
extern u8 *D_80125264;
extern u8 D_8012531C;
extern u16 D_8012439C;
extern u16 D_8012439E;
extern u16 D_801243A0;
extern u16 D_8012438C;
extern u16 D_8012438E;
extern u16 D_80124390;

void func_80152B94(void)
{
    u8 *p1;
    u8 *p2;
    u8 gv;
    s32 j;

    func_80011DFC();
    D_800B1AF6 = D_800B1AF6 + 22;
    gv = D_800B1746;
    j = D_8012531C * 3;
    p1 = D_80125260;
    D_8012439C = (p1 + gv * 36U)[j + 30];
    D_8012439E = (p1 + (u32)gv * 36U)[j + 31];
    p2 = D_80125264;
    D_801243A0 = (p1 + (gv | 0U) * 36U)[j + 32];
    D_8012438C = (p2 + (gv ^ 0U) * 36U)[j + 30];
    D_8012438E = (p2 + (gv * 1U) * 36U)[j + 31];
    D_80124390 = (p2 + (gv >> 0) * 36U)[j + 32];
    func_80114B0C();
}
