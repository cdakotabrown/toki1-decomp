/* geko_bg_cache_step3 -- GEKO overlay, Address: 0x80141648, Size: 128 bytes (32 insns) */
/* @PostProc: patch_insn_reg_field 0x2c:rt:a0:v1 0x30:rs:a0:v1 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 value_cache_find_or_load(s32, s32);

extern u8 cdq_footer_chk;
extern u8 bg_change_tbl0;

void geko_bg_cache_step3(void)
{
    s32 result;
    u8 val;

    cdq_footer_chk = 0;
    result = value_cache_find_or_load(16310, 0);
    val = bg_change_tbl0;

    if (result == val) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    } else if (result == 1 - val) {
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
