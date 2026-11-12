/* @SymbolStart: 0x80137CFC */
/* @SymbolSize: 100 */
/* @PostProc: rename_register_chain --chain v0:t6:t7:t8:t9 */
/* Address: 0x80137CFC (cross-prologue head) / 0x80137D14 (body) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern u8 rec_idx;
extern u8 rec_prev_val[];
extern u8 now_date_spot;

void date_basyo11_bg_step(void) {
    s32 idx;

    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, now_date_spot);
    dec_bg_cache_step(17949);
    sub_sub_step_up();
}
