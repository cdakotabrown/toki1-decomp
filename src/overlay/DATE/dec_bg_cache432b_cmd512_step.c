/* Address: 0x801580C0 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void dec_bg_cache_step(s32);

void dec_bg_cache432b_cmd512_step(void)
{
    device_cmd_send(512);
    dec_bg_cache_step(17195);
    sub_sub_step_up();
}
