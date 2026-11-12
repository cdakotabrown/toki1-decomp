/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Size: 176 bytes (44 insns). Named-extern destinations, integer- */
#include "types.h"

extern s32 D_80120C24;
extern s32 D_80120C28;
extern s32 D_80120C2C;
extern s16 D_80120C30;
extern s32 D_80120C34;
extern s32 D_80120C38;
extern s32 D_80120C3C;
extern s32 D_80120C40;
extern s32 D_80120C44;
extern s32 D_80120C48;
extern s32 D_80120C4C;
extern s16 D_801D2210;

void func_801359CC(void) {
    D_80120C24 = 0x801D21D4;
    D_80120C28 = 0x801D21DC;
    D_80120C2C = 0x801D21FC;
    D_80120C30 = D_801D2210;
    D_80120C34 = 0x801B0000;
    D_80120C38 = 0x801B2000;
    D_80120C3C = 0x801B6000;
    D_80120C40 = 0x801BA000;
    D_80120C44 = 0x801BE000;
    D_80120C48 = 0x801C2000;
    D_80120C4C = 0x801C6000;
}
