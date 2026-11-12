/* Likely Symbol: tate_last_date_check (from o.bin, offset +104 bytes from 0x8007EBE8) */
/* Address: 0x8007EC50 - 0x8007ECA0 (80 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_reset(s32 a0);
extern void bg_change(s32 a0);
extern u8 bg_change_tbl2;
extern s32 step_counter;
extern s32 fade_flag;

void sprite_fade_init(void) {
    k_reset(1);
    bg_change_tbl2 = 128;
    fade_flag = 0;
    step_counter++;
    bg_change(1);
}
