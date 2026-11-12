/* Function: dec_bg_cache_step */
/* Address: 0x8007EEC8 */
/* Size: 116 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 cdq_footer_chk;
extern u8 bg_change_tbl1;

extern s32 value_cache_find_or_load(s32 arg0, s32 arg1);
extern void sub_sub_step_up(void);

void dec_bg_cache_step(s32 arg0) {
    s32 result;
    u8 flag;
    s32 other;

    cdq_footer_chk = 0;

    result = value_cache_find_or_load(arg0, 1);
    flag = bg_change_tbl1;
    other = 1 - (s32)flag;

    if (result == (s32)flag) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    } else if (result == other) {
        sub_sub_step_up();
        sub_sub_step_up();
    }
}
