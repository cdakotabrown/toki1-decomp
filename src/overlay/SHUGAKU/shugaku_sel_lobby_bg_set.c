/* @SymbolStart: 0x8013A2C4 */
/* @SymbolSize: 104 */
#include "types.h"

extern s32 serifu_no;

extern u8 ba_string;
extern u8 shugaku_sel_lobby_bg_set_str0;
extern void dec_bg_cache_step(s32 a0);
extern void strcpy(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

void shugaku_sel_lobby_bg_set(void)
{
    s32 param_t6;

    param_t6 = serifu_no;
    if (param_t6 == 0) {
        dec_bg_cache_step(16933);
        strcpy((s32)&ba_string, (s32)&shugaku_sel_lobby_bg_set_str0);
    } else {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
