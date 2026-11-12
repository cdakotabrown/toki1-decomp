/* @SymbolStart: 0x80133374 */
/* @SymbolSize: 156 */
#include "types.h"

extern u8 taco_round_no;

extern s16 taco_time_left;
extern s32 pad_trig;
extern u8 *taco_actor_rec;

extern void goto_sub_step(s32 a0);

void play_end_set(void)
{
    u8 sel = taco_round_no;

    s16 h;

    if ((u32)sel < 3U) {
        h = taco_time_left;
        taco_time_left = h - 1;
    }

    if ((s16)taco_time_left == 0) {
        goto_sub_step(45);
    }

    if ((pad_trig & 0x800) != 0) {
        goto_sub_step(45);
    }

    if (taco_actor_rec[133] == 0) {
        goto_sub_step(45);
    }
}
