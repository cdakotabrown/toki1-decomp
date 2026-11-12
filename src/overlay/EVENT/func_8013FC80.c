/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Size: 176 bytes (44 insns). Named-extern destinations, integer- */
#include "types.h"

extern s32 D_80122B30;
extern s32 D_80122B34;
extern s32 D_80122B38;
extern s16 D_80122B3C;
extern s32 D_80122B40;
extern s32 D_80122B44;
extern s32 D_80122B48;
extern s32 D_80122B4C;
extern s32 D_80122B50;
extern s32 D_80122B54;
extern s32 D_80122B58;
extern s16 D_801CE15C;

void func_8013FC80(void) {
    D_80122B30 = 0x801CE124;
    D_80122B34 = 0x801CE128;
    D_80122B38 = 0x801CE148;
    D_80122B3C = D_801CE15C;
    D_80122B40 = 0x801B0000;
    D_80122B44 = 0x801B2000;
    D_80122B48 = 0x801B6000;
    D_80122B4C = 0x801BA000;
    D_80122B50 = 0x801BE000;
    D_80122B54 = 0x801C2000;
    D_80122B58 = 0x801C6000;
}
