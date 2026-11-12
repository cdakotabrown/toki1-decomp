/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Size: 176 bytes (44 insns). Named-extern destinations, integer- */
#include "types.h"

extern s32 D_80122280;
extern s32 D_80122284;
extern s32 D_80122288;
extern s16 D_8012228C;
extern s32 D_80122290;
extern s32 D_80122294;
extern s32 D_80122298;
extern s32 D_8012229C;
extern s32 D_801222A0;
extern s32 D_801222A4;
extern s32 D_801222A8;
extern s16 D_801CE184;

void func_8013D460(void) {
    D_80122280 = 0x801CE148;
    D_80122284 = 0x801CE14C;
    D_80122288 = 0x801CE170;
    D_8012228C = D_801CE184;
    D_80122290 = 0x801B0000;
    D_80122294 = 0x801B2000;
    D_80122298 = 0x801B6000;
    D_8012229C = 0x801BA000;
    D_801222A0 = 0x801BE000;
    D_801222A4 = 0x801C2000;
    D_801222A8 = 0x801C6000;
}
