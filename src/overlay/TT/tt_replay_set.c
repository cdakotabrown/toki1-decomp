/* @PostProc: swap_instructions --offset1 0xe8 --offset2 0xf8 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: patch_insn_reg_field 0x60:rt:at:t0 0x64:rs:a0:t0 0x64:rt:at:a0 0x74:rt:t0:t1 0x7c:rt:t0:t1 0x84:rt:t1:t2 0x88:rt:t2:t3 0x8c:rt:t1:t2 0x90:rt:t2:t3 0xc0:rt:t5:t6 0xcc:rt:t7:t8 0xd8:rt:t0:t9 0xdc:rt:t3:t4 0xe0:rs:t3:t4 0xe0:rt:t3:t4 0xe4:rt:t3:t4 0xfc:rt:t4:t5 0x100:rs:t4:t5 0x100:rt:t4:t5 0x104:rt:t4:t5 0x11c:rs:t5:t6 0x11c:rt:t5:t6 0x120:rt:t5:t6 0x124:rt:t6:t7 0x128:rt:t6:t7 0x13c:rs:t7:t8 0x13c:rt:t7:t8 0x140:rt:t7:t8 0x144:rt:t8:t1 0x148:rt:t8:t1 0x15c:rs:t0:t9 0x15c:rt:t0:t9 0x160:rt:t0:t9 0x164:rt:t9:t2 0x168:rt:t9:t2 */
/* tt_replay_set - TT overlay, Address: 0x80136AD8, Size: 396 bytes (99 insns) */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_text_buf;
extern u8 tt_replay0_dat[];
extern u8 tt_replay2_dat[];
extern u8 tt_replay1_dat[];
extern u8 tt_replay3_dat[];
extern u8 tt_replay4_dat[];

extern void SetDispMask(s32 a0);

void tt_replay_set(void)
{
    u8 *p1;
    u8 *p2;
    u8 *p3;
    u8 *q;
    u8 *e;

    p1 = tt_game_wk;
    p2 = tt_step_wk;
    p3 = tt_scroll_wk;
    SetDispMask(1);
    if (p2[34] != 0) {
        if (p1[82] == 255) {
            q = tt_text_buf;
            e = q + 2520;
            *(s16 *)(q + 2530) = -36;
            *(s16 *)(e + 12) = 116;
            *(s16 *)(e + 14) = 9;
            e[2] = 2;
            e[0] = 1;
            *(s16 *)(e + 6) = 0;
            *(s16 *)(e + 16) = 0;
        }
        switch ((u32)p1[82]) {
        case 1:
            *(s32 *)(p1 + 76) = (s32)tt_replay1_dat;
            *(s16 *)(p3 + 8) = 0;
            p3[11] = 0;
            p3[12] = 2;
            p3[13] = 0;
            break;
        case 2:
            *(s32 *)(p1 + 76) = (s32)tt_replay2_dat;
            *(s16 *)(p3 + 8) = 3134;
            p3[11] = 1;
            p3[12] = 0;
            p3[13] = 0;
            break;
        case 3:
            *(s32 *)(p1 + 76) = (s32)tt_replay3_dat;
            *(s16 *)(p3 + 8) = 4895;
            p3[11] = 2;
            p3[12] = 0;
            p3[13] = 0;
            break;
        case 4:
            *(s32 *)(p1 + 76) = (s32)tt_replay4_dat;
            *(s16 *)(p3 + 8) = 9053;
            p3[11] = 3;
            p3[12] = 2;
            p3[13] = 0;
            break;
        default:
            *(s32 *)(p1 + 76) = (s32)tt_replay0_dat;
            *(s16 *)(p3 + 8) = 0;
            p3[11] = 0;
            p3[12] = 0;
            p3[13] = 0;
            break;
        }
    } else {
        *(s16 *)(p3 + 8) = 0;
        p3[11] = 0;
        p3[12] = 2;
        p3[13] = 0;
    }
}
