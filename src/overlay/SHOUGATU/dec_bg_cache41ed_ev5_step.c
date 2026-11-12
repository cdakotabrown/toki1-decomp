/* @SymbolStart: 0x80133EA8 */
/* @SymbolSize: 148 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern u8 rec_prev_val[];
extern u8 rec_idx;
extern s32 frame_counter;
extern u8 shougatu_girl0_good;
extern u8 shougatu_alone_flag;

void dec_bg_cache41ed_ev5_step(void)
{
    if (shougatu_alone_flag == 0 && !(frame_counter & 1) &&
        rec_prev_val[(s32)rec_idx * 8] != 5) {
        if (shougatu_girl0_good != 0) {
            event_log_add(5, 3);
        } else {
            event_log_add(5, 0);
        }
    }
    dec_bg_cache_step(16877);
    sub_sub_step_up();
}
