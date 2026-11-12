/* func_80141BA0 -- GYOZI overlay, Address: 0x80141BA0, Size: 208 bytes (52 insns) */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t6:t7 0xbc:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t7:t8 0xc8:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rs:t8:t9 0xd0:rt:t8:t9 0xdc:rt:t8:t9 */
#include "types.h"

extern s16 D_801D24B4;

extern s32 D_80148B74;
extern s32 D_80148B90;
extern s32 D_80148B94;
extern s32 D_80148B98;
extern s32 D_80148B9C;
extern s16 D_80148BA0;
extern s32 D_80148BA4;
extern s32 D_80148BA8;
extern s32 D_80148BAC;
extern s32 D_80148BB0;
extern s32 D_80148BB4;
extern s32 D_80148BB8;
extern s32 D_80148BBC;

void func_80141BA0(void) {
    D_80148B94 = 0x801D2430;
    D_80148B98 = 0x801D2434;
    D_80148B9C = 0x801D2498;
    D_80148BA0 = D_801D24B4;
    D_80148BA4 = 0x801B0000;
    D_80148B74 = 0x801D2000;
    D_80148BA8 = 0x801B2000;
    D_80148BAC = 0x801B6000;
    D_80148BB0 = 0x801BA000;
    D_80148BB4 = 0x801BE000;
    D_80148BB8 = 0x801C2000;
    D_80148BBC = 0x801C6000;
    D_80148B90 = 0x801CE000;
}
