/* omimai_bg_event_step - OMIMAI overlay, Address: 0x80132788, Size: 52 bytes (13 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void omimai_bg_event_step(void)
{
    dec_bg_cache_step(16708);
    event_log_add(15, 0);
    sub_sub_step_up();
}
