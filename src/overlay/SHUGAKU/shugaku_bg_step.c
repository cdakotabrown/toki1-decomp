/* shugaku_bg_step - SHUGAKU overlay, Address: 0x80135C70, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void shugaku_bg_step(void)
{
    device_cmd_send(0x203);
    dec_bg_cache_step(0x4761);
    sub_sub_step_up();
}
