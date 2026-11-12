/* date2_bg_cache_step2 - DATE2 overlay, Address: 0x801365D0, Size: 132 bytes (33 insns) */
#include "types.h"

extern u8 cdq_footer_chk;
extern u8 bg_change_tbl1;
extern s32 step_counter;
extern u8 date2_bg_cache_step2_str0;
extern u8 date2_bg_cache_step2_str1;

s32 value_cache_find_or_load(s32 ptr, s32 mode);
void printf(void *arg0);

void date2_bg_cache_step2(s32 ptr)
{
    s32 v0;
    u8 v1;
    s32 t9;

    cdq_footer_chk = 0;
    v0 = value_cache_find_or_load(ptr, 1);
    v1 = bg_change_tbl1;
    t9 = 1 - v1;
    if (v0 == (s32)v1) {
        printf(&date2_bg_cache_step2_str0);
        step_counter = step_counter + 3;
    } else if (v0 == t9) {
        printf(&date2_bg_cache_step2_str1);
        step_counter = step_counter + 2;
    }
}
