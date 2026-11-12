/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
#include "types.h"

extern s32 D_8012345C;
extern s32 D_80123460;
extern s32 D_80123464;
extern s16 D_80123468;
extern s32 D_8012346C;
extern s32 D_80123470;
extern s32 D_80123474;
extern s32 D_80123478;
extern s32 D_8012347C;
extern s32 D_80123480;
extern s32 D_80123484;
extern s16 D_801CE15C;

void func_80142CD0(void) {
    D_8012345C = 0x801CE124;
    D_80123460 = 0x801CE128;
    D_80123464 = 0x801CE148;
    D_80123468 = D_801CE15C;
    D_8012346C = 0x801B0000;
    D_80123470 = 0x801B2000;
    D_80123474 = 0x801B6000;
    D_80123478 = 0x801BA000;
    D_8012347C = 0x801BE000;
    D_80123480 = 0x801C2000;
    D_80123484 = 0x801C6000;
}
