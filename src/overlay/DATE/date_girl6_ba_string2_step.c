/* Function: date_girl6_ba_string2_step */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 a0);
extern void strcpy(s32 a0, s32 a1);

extern u8 ba_string2;
extern u8 date_girl6_ba_string2_step_str0;

void date_girl6_ba_string2_step(void)
{
    strcpy((s32)&ba_string2, (s32)&date_girl6_ba_string2_step_str0);
    dec_bg_cache_step(17846);
    sub_sub_step_up();
}
