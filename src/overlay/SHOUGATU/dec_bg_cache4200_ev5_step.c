/* @SymbolStart: 0x80134C34 */
/* @SymbolSize: 128 */
/* dec_bg_cache4200_ev5_step - SHOUGATU overlay, Address: 0x80134C3C, Size: 120 bytes (30 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern u8 rec_prev_val[];
extern u8 rec_idx;
extern u8 shougatu_girl0_good;
extern u8 shougatu_alone_flag;

void dec_bg_cache4200_ev5_step(void)
{
    if (shougatu_alone_flag == 0 && rec_prev_val[rec_idx * 8] != 5) {
        if (shougatu_girl0_good != 0) {
            event_log_add(5, 5);
        } else {
            event_log_add(5, 2);
        }
    }
    dec_bg_cache_step(16896);
    sub_sub_step_up();
}
