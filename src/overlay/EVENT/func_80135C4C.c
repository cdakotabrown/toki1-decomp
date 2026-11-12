/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Size: 176 bytes (44 insns). Named-extern destinations, integer- */
#include "types.h"

extern s32 D_80120CC4;
extern s32 D_80120CC8;
extern s32 D_80120CCC;
extern s16 D_80120CD0;
extern s32 D_80120CD4;
extern s32 D_80120CD8;
extern s32 D_80120CDC;
extern s32 D_80120CE0;
extern s32 D_80120CE4;
extern s32 D_80120CE8;
extern s32 D_80120CEC;
extern s16 D_801D2358;

void func_80135C4C(void) {
    D_80120CC4 = 0x801D22D0;
    D_80120CC8 = 0x801D22D8;
    D_80120CCC = 0x801D233C;
    D_80120CD0 = D_801D2358;
    D_80120CD4 = 0x801B0000;
    D_80120CD8 = 0x801B2000;
    D_80120CDC = 0x801B6000;
    D_80120CE0 = 0x801BA000;
    D_80120CE4 = 0x801BE000;
    D_80120CE8 = 0x801C2000;
    D_80120CEC = 0x801C6000;
}
