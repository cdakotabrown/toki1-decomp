/* Address: 0x80154C84 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void dec_bg_cache_step(s32);

void dec_bg_cache43f0_cmd1280_step2(void)
{
    device_cmd_send(1280);
    dec_bg_cache_step(17392);
    sub_sub_step_up();
}
