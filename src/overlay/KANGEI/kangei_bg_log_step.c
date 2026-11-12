/* kangei_bg_log_step - KANGEI overlay, Address: 0x8013536C, Size: 52 bytes (13 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void kangei_bg_log_step(void)
{
    dec_bg_cache_step(17055);
    event_log_add(14, 12);
    sub_sub_step_up();
}
