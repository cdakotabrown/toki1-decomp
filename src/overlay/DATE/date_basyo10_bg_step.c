/* @SymbolStart: 0x80137C98 */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:t6 0x14:rs:v1:t6 0x14:rt:v1:t6 0x18:rt:t7:t8 0x1c:rt:v1:t6 0x1c:rd:t6:t7 0x20:rs:t7:t8 0x20:rt:t6:t7 0x20:rd:t7:t8 0x24:rs:t7:t8 0x24:rt:t7:t8 0x30:rs:t7:t8 0x38:rs:v1:t6 0x38:rt:t8:t9 0x40:rt:t8:t9 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern u8 rec_prev_val[];
extern u8 rec_idx;
extern u8 now_date_spot;

void date_basyo10_bg_step(void) {
    u32 i;

    i = rec_idx;
    if (rec_prev_val[i * 8] == 1) {
        rec_idx = (u8)(i - 1);
    }
    event_log_add(0, now_date_spot);
    dec_bg_cache_step(17959);
    sub_sub_step_up();
}
