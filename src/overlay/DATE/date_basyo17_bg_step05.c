/* @PostProc: swap_registers t8:t9 --unidirectional */
/* @PostProc: swap_registers t7:t8 --unidirectional */
/* @PostProc: swap_registers t6:t7 --unidirectional */
/* @PostProc: swap_registers v0:t6 --unidirectional */
#include "types.h"

extern u8 rec_idx;
extern u8 rec_prev_val[];
extern u8 now_date_spot;
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void date_basyo17_bg_step05(void)
{
    s32 idx;
    dec_bg_cache_step(17836);
    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, now_date_spot);
    sub_sub_step_up();
}
