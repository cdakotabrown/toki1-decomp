/* @SymbolStart: 0x80138154 */
/* @SymbolSize: 208 */
/* @PostProc: patch_insn_reg_field 0x38:rt:v0:t7 0x3c:rs:v0:t7 0x3c:rt:v0:t7 0x40:rt:t8:t9 0x44:rd:t7:t8 0x44:rt:v0:t7 0x48:rd:t8:t9 0x48:rs:t8:t9 0x48:rt:t7:t8 0x4c:rs:t8:t9 0x4c:rt:t8:t9 0x54:rs:t8:t9 0x5c:rs:v0:t7 0x5c:rt:t9:t0 0x64:rt:t9:t0 0x8c:rt:v0:t1 0x90:rs:v0:t1 0x90:rt:v0:t1 0x94:rt:t1:t3 0x98:rd:t0:t2 0x98:rt:v0:t1 0x9c:rd:t1:t3 0x9c:rs:t1:t3 0x9c:rt:t0:t2 0xa0:rs:t1:t3 0xa0:rt:t1:t3 0xa8:rs:t1:t3 0xb0:rs:v0:t1 0xb0:rt:t2:t4 0xb8:rt:t2:t4 */
/* Function: date_basyo13_bg_step */
/* Overlay: DATE */
/* Address: 0x8013815C */
/* Size: 200 bytes (50 instructions) */
#include "types.h"

extern void device_cmd_send(s32 a);
extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern void sub_sub_step_up(void);

extern u8 date_pattern;
extern u8 rec_idx;
extern u8 rec_prev_val[];
extern u8 now_date_spot;

void date_basyo13_bg_step(void)
{
    s32 idx;

    if ((u32)date_pattern < 2) {
        device_cmd_send(512);
        dec_bg_cache_step(17921);
        idx = rec_idx;
        if (rec_prev_val[idx * 8] == 1) {
            rec_idx = idx - 1;
        }
        event_log_add(0, now_date_spot);
    } else {
        device_cmd_send(513);
        dec_bg_cache_step(17930);
        idx = rec_idx;
        if (rec_prev_val[idx * 8] == 1) {
            rec_idx = idx - 1;
        }
        event_log_add(0, now_date_spot);
    }
    sub_sub_step_up();
}
