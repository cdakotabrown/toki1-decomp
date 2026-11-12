/* @SymbolStart: 0x801352FC */
/* @SymbolSize: 420 */
/* season_bg_cache_set -- GEKO overlay, Address: 0x80135304, Size: 412 bytes */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 value_cache_find_or_load(s32 a0, s32 a1);

extern u8 cdq_footer_chk;
extern u8 bg_change_tbl0;
extern u8 now_month;
extern u8 day_flag;
extern u8 geko_sv_f10_flag;

void season_bg_cache_set(void)
{
    s32 hidden_param = geko_sv_f10_flag;

    u32 v0;
    s32 v1;
    s32 t0;

    cdq_footer_chk = 0;

    if (hidden_param != 0) {
        goto check_range;
    }

    if ((day_flag & 0x20) == 0) {
        goto path_b;
    }

check_range:
    v0 = now_month;
    if (v0 >= 3U && v0 < 6U) {
        v0 = value_cache_find_or_load(16361, 0);
        goto end_check;
    }
    if (v0 >= 6U && v0 < 9U) {
        v0 = value_cache_find_or_load(16370, 0);
        goto end_check;
    }
    if (v0 >= 9U && v0 < 12U) {
        v0 = value_cache_find_or_load(16343, 0);
        goto end_check;
    }
    v0 = value_cache_find_or_load(16352, 0);
    goto end_check;

path_b:
    v0 = now_month;
    if (v0 >= 3U && v0 < 6U) {
        v0 = value_cache_find_or_load(16318, 0);
        goto end_check;
    }
    if (v0 >= 6U && v0 < 9U) {
        v0 = value_cache_find_or_load(16326, 0);
        goto end_check;
    }
    if (v0 >= 9U && v0 < 12U) {
        v0 = value_cache_find_or_load(16301, 0);
        goto end_check;
    }
    v0 = value_cache_find_or_load(16310, 0);

end_check:
    v1 = bg_change_tbl0;
    t0 = 1 - v1;

    if ((s32)v0 == v1) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
        goto final_call;
    }

    if ((s32)v0 == t0) {
        sub_sub_step_up();
        sub_sub_step_up();
    }

final_call:
    sub_sub_step_up();
}
