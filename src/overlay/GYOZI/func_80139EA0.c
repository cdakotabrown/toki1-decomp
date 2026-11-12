/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0xa0:rt:t5:t6 0xa4:rt:t5:t6 0xa8:rt:t6:t7 0xac:rs:t6:t7 0xac:rt:t6:t7 0xb4:rt:t6:t7 0xb8:rt:t7:t8 0xbc:rs:t7:t8 0xbc:rt:t7:t8 0xc4:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rt:t8:t9 0xd4:rt:t9:t0 0xd8:rs:t9:t0 0xd8:rt:t9:t0 0xe0:rt:t9:t0 0xe4:rt:t0:t1 0xe8:rs:t0:t1 0xe8:rt:t0:t1 0xf0:rt:t0:t1 0xf4:rt:t1:t2 0xf8:rs:t1:t2 0xf8:rt:t1:t2 0x100:rt:t1:t2 0x108:rt:t2:t3 0x10c:rt:t2:t3 0x110:rt:t3:t4 0x114:rs:t3:t4 0x114:rt:t3:t4 0x120:rt:t3:t4 */
/* @SymbolSize: 276 */
/* func_80139EA0 -- GYOZI overlay, Address: 0x80139EA0, Size: 276 bytes (69 insns) */
#include "types.h"

extern s32 D_801479B0;
extern s32 D_801479B4;
extern s32 D_801479B8;
extern s16 D_801479BC;
extern s32 D_801479C0;
extern s32 D_801479C4;
extern s32 D_801479C8;
extern s32 D_801479CC;
extern s32 D_801479D0;
extern s32 D_801479D4;
extern s32 D_801479D8;
extern s32 D_801479DC;
extern s32 D_801479E0;
extern s32 D_801479E4;
extern s32 D_801479E8;
extern s32 D_801479EC;
extern s32 D_801479F0;
extern s32 D_801479F4;
extern s16 D_801E80B4;

void func_80139EA0(void)
{
    D_801479B0 = 0x801E8088;
    D_801479B4 = 0x801E808C;
    D_801479B8 = 0x801E80A0;
    D_801479BC = D_801E80B4;
    D_801479C0 = 0x801B0000;
    D_801479C4 = 0x801B2000;
    D_801479C8 = 0x801B4000;
    D_801479CC = 0x801B8000;
    D_801479D0 = 0x801BC000;
    D_801479D4 = 0x801C0000;
    D_801479D8 = 0x801C4000;
    D_801479DC = 0x801C8000;
    D_801479E0 = 0x801D0000;
    D_801479E4 = 0x801D4000;
    D_801479E8 = 0x801D8000;
    D_801479EC = 0x801DC000;
    D_801479F0 = 0x801E0000;
    D_801479F4 = 0x801E4000;
}
