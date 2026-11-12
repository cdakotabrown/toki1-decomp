/* @SymbolStart: 0x80137A54 */
/* @SymbolSize: 100 */
/* @PostProc: rename_register_chain --chain v0:t6:t7:t8:t9 */
/* date_basyo6_bg_step - DATE overlay, Address: 0x80137A6C, Size: 76 bytes (19 insns) */
#include "types.h"

extern u8 rec_idx;
extern u8 rec_prev_val[];
extern u8 now_date_spot;

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 id);
extern void event_log_add(s32 a0, s32 a1);

void date_basyo6_bg_step(void)
{
    s32 idx;

    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, now_date_spot);
    dec_bg_cache_step(17904);
    sub_sub_step_up();
}
