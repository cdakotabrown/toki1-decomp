/* Address: 0x8015903C */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void dec_bg_cache_step(s32);

void dec_bg_cache4520_cmd515_step3(void)
{
    device_cmd_send(515);
    dec_bg_cache_step(17696);
    sub_sub_step_up();
}
