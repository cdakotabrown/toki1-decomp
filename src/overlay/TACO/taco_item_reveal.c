/* @SymbolStart: 0x80138CD0 */
/* @SymbolSize: 1040 */
/* @RodataBase: 0x8015D8F0 */
/* Address: 0x80138CD0 (cross-prologue head) / 0x80138CD8 (prologue) */
/* Size: 1040 bytes (260 insns incl. the 2-insn head) */
#include "types.h"

extern u8 step_mode;
extern volatile u8 step_mode_v;
extern s32 pad_trig;

extern u8 taco_item_reveal_str0[];
extern u8 taco_item_reveal_str1[];
extern u8 taco_item_reveal_str2[];
extern u8 taco_item_reveal_str3[];
extern u8 taco_item_reveal_str4[];

extern u8 *taco_tally_base;
extern u8 *taco_tally_power;
extern u8 *taco_tally_rank;
extern u8 *taco_tally_time;
extern u8 *taco_tally_total;
extern u8 *taco_score;

extern void sub_sub_step_up(void);
extern void frame_buf_clear(void);
extern u8 *taco_num_roll_disp(u8 *, s32, s32, s32);
extern void taco_str_disp2(u8 *, s32, s32, s32);
extern void taco_num_disp(u8 *, s32, s32, s32);

void taco_item_reveal(void)
{
    switch ((u32)step_mode) {
    default:
    case 8:
    case 9:
        taco_str_disp2(taco_item_reveal_str0, -228, 72, 1);
    case 6:
    case 7:
        taco_str_disp2(taco_item_reveal_str1, -228, 24, 5);
    case 4:
    case 5:
        taco_str_disp2(taco_item_reveal_str2, -228, -8, 2);
    case 2:
    case 3:
        taco_str_disp2(taco_item_reveal_str3, -228, -40, 3);
    case 0:
    case 1:
        taco_str_disp2(taco_item_reveal_str4, -228, -72, 6);
    }

    switch ((u32)step_mode) {
    default:
    case 10:
        taco_num_disp(taco_tally_total, -64, 88, 0);
    case 8:
    case 9:
        taco_num_disp(taco_tally_power, -64, 40, 0);
    case 6:
    case 7:
        taco_num_disp(taco_tally_rank, -64, 8, 0);
    case 4:
    case 5:
        taco_num_disp(taco_tally_time, -64, -24, 0);
    case 2:
    case 3:
        taco_num_disp(taco_tally_base, -64, -56, 0);
    case 0:
    case 1:
        ;
    }

    switch ((u32)step_mode_v) {
    case 0:
        taco_num_roll_disp(taco_tally_base, -64, -56, 0);
        step_mode = step_mode + 1;
        break;
    case 1:
        if (taco_num_roll_disp(taco_tally_base, -64, -56, 1) == taco_tally_base) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        taco_num_roll_disp(taco_tally_time, -64, -24, 0);
        step_mode = step_mode + 1;
        break;
    case 3:
        if (taco_num_roll_disp(taco_tally_time, -64, -24, 1) == taco_tally_time) {
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        taco_num_roll_disp(taco_tally_rank, -64, 8, 0);
        step_mode = step_mode + 1;
        break;
    case 5:
        if (taco_num_roll_disp(taco_tally_rank, -64, 8, 1) == taco_tally_rank) {
            step_mode = step_mode + 1;
        }
        break;
    case 6:
        taco_num_roll_disp(taco_tally_power, -64, 40, 0);
        step_mode = step_mode + 1;
        break;
    case 7:
        if (taco_num_roll_disp(taco_tally_power, -64, 40, 1) == taco_tally_power) {
            step_mode = step_mode + 1;
        }
        break;
    case 8:
        taco_num_roll_disp(taco_tally_total, -64, 88, 0);
        step_mode = step_mode + 1;
        break;
    case 9:
        if (taco_num_roll_disp(taco_tally_total, -64, 88, 1) == taco_tally_total) {
            step_mode = step_mode + 1;
        }
        break;
    }

    if ((pad_trig & 0x20) != 0) {
        if (step_mode < 10U) {
            step_mode = 10;
        } else {
            taco_score = taco_tally_total;
            frame_buf_clear();
            sub_sub_step_up();
        }
    }
}
