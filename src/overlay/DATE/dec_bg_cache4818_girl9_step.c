/* Function: dec_bg_cache4818_girl9_step */
/* Overlay: DATE */
#include "types.h"

extern u8 now_girl;
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4818_girl9_step(void)
{
    now_girl = 9;
    dec_bg_cache_step(18456);
    sub_sub_step_up();
}
