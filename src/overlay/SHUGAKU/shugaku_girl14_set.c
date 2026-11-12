/* Address: 0x80134188 (true start 0x80134174, .sym late by 5 insns) */
/* @SymbolStart: 0x80134174 */
/* @SymbolSize: 88 */
#include "types.h"

extern void strcpy(void *dst, void *src);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 cal_cw_anim;
extern s16 cw98_anim;
extern u8 g_name;
extern u8 shugaku_girl14_set_str0;

void shugaku_girl14_set(void)
{
    now_girl = 14;
    cal_cw_anim = 4;
    cw98_anim = 2;
    strcpy(&g_name, &shugaku_girl14_set_str0);
    sub_sub_step_up();
}
