/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: patch_insn_reg_field 0x24:rt:t9:t1 0x28:rt:t2:t4 0x2c:rd:v0:t9 0x30:rt:v0:t9 0x34:rs:t9:t0 0x34:rt:t0:t1 0x34:rd:t1:t2 0x38:rs:t2:t9 0x38:rt:v0:t4 0x38:rd:t3:t5 0x3c:rt:t1:t2 0x40:rt:t3:t5 0x44:rt:t4:t6 0x48:rt:t5:t7 0x4c:rt:t4:t6 0x54:rt:t5:t7 */
/* tt_recs_move - TT overlay, Address: 0x801490B0. */
#include "types.h"

extern s32 *tt_stage_wk;

void tt_recs_move(u8 *p)
{
    s32 diff;
    diff = *tt_stage_wk - *(s32 *)(p + 28);
    *(volatile s32 *)(p + 36) = diff;
    *(s32 *)(p + 32) = *(s32 *)(p + 32) + *(s32 *)(p + 40);
    *(volatile s32 *)(p + 36) = diff + *(s32 *)(p + 44);
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
}
