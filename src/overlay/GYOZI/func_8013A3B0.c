/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t0:t1 0x50:rt:t9:t0 0x58:rs:t0:t1 0x58:rt:t0:t1 0x5c:rt:t0:t1 0x64:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t6:t7 0xbc:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t7:t8 0xc8:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rs:t8:t9 0xd0:rt:t8:t9 0xd8:rt:t8:t9 0xdc:rt:t9:t0 0xe0:rs:t9:t0 0xe0:rt:t9:t0 0xe8:rt:t9:t0 0xec:rt:t0:t1 0xf0:rs:t0:t1 0xf0:rt:t0:t1 0xf8:rt:t0:t1 0xfc:rt:t1:t2 0x100:rs:t1:t2 0x100:rt:t1:t2 0x108:rt:t1:t2 0x10c:rt:t2:t3 0x110:rs:t2:t3 0x110:rt:t2:t3 0x118:rt:t2:t3 0x11c:rt:t3:t4 0x120:rs:t3:t4 0x120:rt:t3:t4 0x12c:rt:t3:t4 */
/* @SymbolSize: 288 */
/* func_8013A3B0 -- GYOZI overlay, Address: 0x8013A3B0, Size: 288 bytes (72 insns) */
#include "types.h"

extern s32 D_80147AD0;
extern s32 D_80147AD4;
extern s32 D_80147AD8;
extern s16 D_80147ADC;
extern s32 D_80147AE0;
extern s32 D_80147AE4;
extern s32 D_80147AE8;
extern s32 D_80147AEC;
extern s32 D_80147AF0;
extern s32 D_80147AF4;
extern s32 D_80147AF8;
extern s32 D_80147AFC;
extern s32 D_80147B00;
extern s32 D_80147B04;
extern s32 D_80147B08;
extern s32 D_80147B0C;
extern s32 D_80147B10;
extern s32 D_80147B14;
extern s16 D_801E60F0;

void func_8013A3B0(void)
{
    D_80147AD0 = 0x801E60CC;
    D_80147AD4 = 0x801E60D0;
    D_80147AD8 = 0x801E60E4;
    D_80147ADC = D_801E60F0;
    D_80147AE0 = 0x801AE000;
    D_80147AE4 = 0x801B0000;
    D_80147AE8 = 0x801B2000;
    D_80147AEC = 0x801B6000;
    D_80147AF0 = 0x801BA000;
    D_80147AF4 = 0x801BE000;
    D_80147AF8 = 0x801C2000;
    D_80147AFC = 0x801C6000;
    D_80147B00 = 0x801CE000;
    D_80147B04 = 0x801D2000;
    D_80147B08 = 0x801D6000;
    D_80147B0C = 0x801DA000;
    D_80147B10 = 0x801DE000;
    D_80147B14 = 0x801E2000;
}
