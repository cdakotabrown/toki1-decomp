/* @SymbolStart: 0x80137AB8 */
/* @SymbolSize: 200 */
/* @PostProc: rename_register_chain --chain v0:t6:t7:t8:t9 */
/* @PostProc: patch_insn_reg_field 0x54:rt:t9:t0 0x58:rs:t9:t0 0x58:rt:t9:t0 0x5c:rt:t0:t1 0x60:rs:t9:t0 0x78:rs:t0:t1 0x78:rt:t0:t1 0x7c:rt:t1:t2 0x80:rs:t0:t1 0x98:rs:t1:t2 0x98:rt:t1:t2 0xa0:rs:t1:t2 */
/* Address: 0x80137AD0 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 sound_id);
extern void event_log_add(s32 a0, s32 a1);
extern u8 rec_idx;
extern u8 rec_prev_val[];
extern u8 now_date_spot;
extern u8 date_spot12_flag;
extern u8 date_spot13_flag;
extern u8 date_spot14_flag;

void date_basyo7_bg_step(void)
{
    s32 idx;

    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, now_date_spot);
    if (date_spot12_flag != 0) {
        dec_bg_cache_step(17667);
    } else if (date_spot13_flag != 0) {
        dec_bg_cache_step(17677);
    } else if (date_spot14_flag != 0) {
        dec_bg_cache_step(17686);
    } else {
        dec_bg_cache_step(17658);
    }
    sub_sub_step_up();
}
