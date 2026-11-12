/* valen_bg_event_step - VALEN overlay, Address: 0x801326B8, Size: 52 bytes (13 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void valen_bg_event_step(void)
{
    dec_bg_cache_step(16453);
    event_log_add(6, 0);
    sub_sub_step_up();
}
