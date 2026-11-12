/* func_8013F8B0 -- GYOZI overlay, Address: 0x8013F8B0, Size: 80 bytes (20 insns) */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x5c:rt:t0:t1 */
#include "types.h"

extern s16 D_801D641C;

extern s32 D_801487F0;
extern s32 D_801487F4;
extern s32 D_801487F8;
extern s16 D_801487FC;
extern s32 D_80148800;

void func_8013F8B0(void) {
    D_801487F0 = 0x801D6360;
    D_801487F4 = 0x801D6384;
    D_801487F8 = 0x801D6414;
    D_801487FC = D_801D641C;
    D_80148800 = 0x801B0000;
}
