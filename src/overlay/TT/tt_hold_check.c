/* @PostProc: patch_insn_reg_field 0x18:rt:v1:a0 0x1c:rs:v1:a0 0x1c:rt:v1:a0 0x24:rs:v1:a0 0x24:rt:v1:a0 0x28:rd:a0:v1 0x28:rs:v1:a0 0x34:rs:v1:a0 0x48:rs:v1:a0 0x70:rs:a0:v1 */
/* tt_hold_check - TT overlay, Address: 0x8013EE80, Size: 176 bytes (44 insns) */
#include "types.h"

extern u8 *tt_step_wk;
extern u8 *tt_game_wk;

s32 tt_hold_check(void)
{
    u8 *r;
    u8 *pad;
    u8 *pad2;

    r = tt_step_wk;
    pad = tt_game_wk + 16;
    pad2 = pad;
    if (r[14] != 0) {
        if ((*(u16 *)(pad) & 0x090C) == 2316 &&
            (*(u16 *)(pad + 2) & 0x090C) != 2316) {
            r[15] = 1;
        }
        if (r[15] != 0 && (*(u16 *)pad2 & 0x090C) == 2316) {
            *(s16 *)(r + 12) = *(u16 *)(r + 12) + 1;
        } else {
            *(s16 *)(r + 12) = 0;
        }
    }
    if ((u32)*(u16 *)(r + 12) >= 31U) {
        return 1;
    }
    return 0;
}
