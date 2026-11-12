/* @SymbolStart: 0x80137BE4 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v0:t7 0x30:rs:v0:t7 0x30:rt:v0:t7 0x34:rt:t8:t9 0x38:rd:t7:t8 0x38:rt:v0:t7 0x3c:rd:t8:t9 0x3c:rs:t8:t9 0x3c:rt:t7:t8 0x40:rs:t8:t9 0x40:rt:t8:t9 0x48:rs:t8:t9 0x50:rs:v0:t7 0x50:rt:t9:t0 0x58:rt:t9:t0 0x74:rt:v0:t1 0x78:rs:v0:t1 0x78:rt:v0:t1 0x7c:rt:t1:t3 0x80:rd:t0:t2 0x80:rt:v0:t1 0x84:rd:t1:t3 0x84:rs:t1:t3 0x84:rt:t0:t2 0x88:rs:t1:t3 0x88:rt:t1:t3 0x90:rs:t1:t3 0x98:rs:v0:t1 0x98:rt:t2:t4 0xa0:rt:t2:t4 */
/* Function: date_basyo9_bg_step */
/* Overlay: DATE */
/* Address: 0x80137BE4 (cross-prologue head) / 0x80137BEC (body) */
/* Size: 180 bytes (45 instructions) */
#include "types.h"

extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern u8 rec_idx;
extern u8 rec_prev_val[];

void date_basyo9_bg_step(void)
{
    s32 idx;

    if (serifu_no == 0) {
        dec_bg_cache_step(17631);
        idx = rec_idx;
        if (rec_prev_val[idx * 8] == 1) {
            rec_idx = idx - 1;
        }
        event_log_add(0, 16);
    } else {
        dec_bg_cache_step(17640);
        idx = rec_idx;
        if (rec_prev_val[idx * 8] == 1) {
            rec_idx = idx - 1;
        }
        event_log_add(0, 17);
    }
    sub_sub_step_up();
}
