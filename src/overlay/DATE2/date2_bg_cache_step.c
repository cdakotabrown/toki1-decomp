/* date2_bg_cache_step - DATE2 overlay, Address: 0x8013279C, Size: 128 bytes (32 insns) */
/* @SymbolSize: 128 */
#include "types.h"

extern u8 cdq_footer_chk;
extern u8 bg_change_tbl0;

s32 value_cache_find_or_load(s32 arg0, s32 arg1);
void sub_sub_step_up(void);

void date2_bg_cache_step(void)
{
    s32 v0;
    s32 v1;
    s32 t7;

    cdq_footer_chk = 0;
    v0 = value_cache_find_or_load(16326, 0);
    v1 = bg_change_tbl0;
    t7 = 1 - v1;
    if (v0 == v1)
    {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
    else if (v0 == t7)
    {
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
