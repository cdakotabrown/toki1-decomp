/* shugaku_girl5_bg_set - SHUGAKU overlay, Address: 0x801347E0, Size: 52 bytes (13 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void shugaku_girl5_bg_set(void)
{
    dec_bg_cache_step(0x4774);
    event_log_add(2, 0x27);
    sub_sub_step_up();
}
