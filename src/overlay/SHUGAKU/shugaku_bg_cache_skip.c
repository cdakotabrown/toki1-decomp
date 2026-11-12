/* Address: 0x80137EAC (cross-prologue head absorbed from 0x80137EA4) */
/* Size: 140 bytes (35 instructions incl. hoisted head) */
/* @SymbolStart: 0x80137EA4 */
/* @SymbolSize: 140 */
#include "types.h"

extern s32 rpg_bat_arg;
extern u8  cdq_footer_chk;
extern u8  bg_change_tbl0;

extern s32  value_cache_find_or_load(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

void shugaku_bg_cache_skip(void)
{
    s32 v0;
    s32 v1;
    s32 t8;

    rpg_bat_arg = 255;
    cdq_footer_chk = 0;
    v0 = value_cache_find_or_load(16301, 0);
    v1 = (s32)bg_change_tbl0;
    t8 = 1 - v1;
    if (v0 != v1) {
        goto check_b;
    }
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    goto end;
check_b:
    if (v0 != t8) {
        goto end;
    }
    sub_sub_step_up();
    sub_sub_step_up();
end:
    sub_sub_step_up();
}
