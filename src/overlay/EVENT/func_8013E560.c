/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
#include "types.h"

extern s32 D_801226AC;
extern s32 D_801226B0;
extern s32 D_801226B4;
extern s16 D_801226B8;
extern s32 D_801226BC;
extern s32 D_801226C0;
extern s32 D_801226C4;
extern s32 D_801226C8;
extern s32 D_801226CC;
extern s32 D_801226D0;
extern s32 D_801226D4;
extern s16 D_801CE0A4;

void func_8013E560(void) {
    D_801226AC = 0x801CE07C;
    D_801226B0 = 0x801CE080;
    D_801226B4 = 0x801CE090;
    D_801226B8 = D_801CE0A4;
    D_801226BC = 0x801B0000;
    D_801226C0 = 0x801B2000;
    D_801226C4 = 0x801B6000;
    D_801226C8 = 0x801BA000;
    D_801226CC = 0x801BE000;
    D_801226D0 = 0x801C2000;
    D_801226D4 = 0x801C6000;
}
