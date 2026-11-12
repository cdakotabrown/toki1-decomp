/* @SymbolSize: 116 */
/* @PostProc: rename_register_chain --chain v0:t6:t7:t8:t9 */
/* @PostProc: swap_registers v1:t6 --unidirectional */
/* Function: date_basyo17_bg_step2e */
/* Overlay: DATE */
/* Address: 0x80138828 */
/* Size: 116 bytes (29 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 msgId);
extern void event_log_add(s32 arg0, s32 arg1);
extern void strcpy(void *dst, void *src);

extern u8 ba_string2;
extern u8 rec_prev_val[];
extern u8 rec_idx;
extern u8 date_basyo17_bg_step2e_str0;

void date_basyo17_bg_step2e(void)
{
    u8 idx;

    strcpy(&ba_string2, &date_basyo17_bg_step2e_str0);
    dec_bg_cache_step(17846);

    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, 40);
    sub_sub_step_up();
}
