/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: patch_insn_reg_field 0x24:rd:a2:a0 0x38:rs:a2:a0 0x48:rs:a2:a0 0x58:rs:a2:a0 0x68:rs:a2:a0 0x78:rs:a2:a0 0x88:rs:a2:a0 0x98:rs:a2:a0 */
/* tt_bitmask_code_set - TT overlay, Address: 0x8013C764. */
#include "types.h"

extern u8 *tt_game_wk;

void tt_bitmask_code_set(u16 a0, u16 a1)
{
    u8 *p = tt_game_wk;
    s32 v1 = 0;
    if (a0 == 0) {
        return;
    }
    if (a0 & 0x20) v1 = 5;
    else if (a0 & 0x40) v1 = 6;
    else if (a0 & 0x10) v1 = 4;
    else if (a0 & 0x80) v1 = 7;
    else if (a0 & 0x04) v1 = 2;
    else if (a0 & 0x08) v1 = 3;
    else if (a0 & 0x01) v1 = 0;
    else if (a0 & 0x02) v1 = 1;
    if (a1 == 0) {
        p[68] = v1;
    } else {
        p[69] = v1;
    }
}
