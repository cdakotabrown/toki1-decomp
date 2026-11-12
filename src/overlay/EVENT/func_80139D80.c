/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
#include "types.h"

extern s32 D_80121858;
extern s32 D_8012185C;
extern s32 D_80121860;
extern s16 D_80121864;
extern s32 D_80121868;
extern s32 D_8012186C;
extern s32 D_80121870;
extern s32 D_80121874;
extern s32 D_80121878;
extern s32 D_8012187C;
extern s32 D_80121880;
extern s16 D_801D218C;

void func_80139D80(void) {
    D_80121858 = 0x801D2148;
    D_8012185C = 0x801D2150;
    D_80121860 = 0x801D2170;
    D_80121864 = D_801D218C;
    D_80121868 = 0x801B0000;
    D_8012186C = 0x801B2000;
    D_80121870 = 0x801B6000;
    D_80121874 = 0x801BA000;
    D_80121878 = 0x801BE000;
    D_8012187C = 0x801C2000;
    D_80121880 = 0x801C6000;
}
