/* Address: 0x80138D94, Size: 148 bytes (37 insns) */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: patch_insn_reg_field 0x14:rt:17:2 */
/* @PostProc: patch_insn_reg_field 0x18:rs:17:2 */
/* @PostProc: patch_insn_reg_field 0x18:rt:17:2 */
/* @PostProc: patch_insn_reg_field 0x4c:rs:17:2 */
#include "types.h"

extern s32 tt_seq_step();

extern s32 tt_boss_wk;
extern u8 tt_group_on_seq_tbl[];
extern u8 tt_group_off_seq_tbl[];

void tt_seq_group_step(void)
{
    s32 i;
    u8 *ptr;
    s32 base;

    base = tt_boss_wk;
    ptr = (u8 *)(base + 2);

    for (i = 2; i != 6; i++) {
        if (ptr[849] != 0) {
            tt_seq_step(tt_group_on_seq_tbl, i);
        } else {
            tt_seq_step(tt_group_off_seq_tbl, i);
        }
        ptr++;
    }
}
