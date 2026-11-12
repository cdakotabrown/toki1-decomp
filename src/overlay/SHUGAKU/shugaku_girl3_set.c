/* Address: 0x8013413C (true start 0x80134128, .sym late by 5 insns) */
/* @SymbolStart: 0x80134128 */
/* @SymbolSize: 76 */
#include "types.h"

extern void date_name_set(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 cal_cw_anim;
extern s16 cw98_anim;

void shugaku_girl3_set(void)
{
    now_girl = 3;
    cal_cw_anim = 2;
    cw98_anim = 4;
    date_name_set(3);
    sub_sub_step_up();
}
