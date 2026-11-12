/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
#include "types.h"

extern s32 D_80122FC8;
extern s32 D_80122FCC;
extern s32 D_80122FD0;
extern s16 D_80122FD4;
extern s32 D_80122FD8;
extern s32 D_80122FDC;
extern s32 D_80122FE0;
extern s32 D_80122FE4;
extern s32 D_80122FE8;
extern s32 D_80122FEC;
extern s32 D_80122FF0;
extern s16 D_801D22A0;

void func_80141250(void) {
    D_80122FC8 = 0x801D2240;
    D_80122FCC = 0x801D2248;
    D_80122FD0 = 0x801D2288;
    D_80122FD4 = D_801D22A0;
    D_80122FD8 = 0x801B0000;
    D_80122FDC = 0x801B2000;
    D_80122FE0 = 0x801B6000;
    D_80122FE4 = 0x801BA000;
    D_80122FE8 = 0x801BE000;
    D_80122FEC = 0x801C2000;
    D_80122FF0 = 0x801C6000;
}
