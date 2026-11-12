/* shugaku_girl2_bg_set - SHUGAKU overlay, Address: 0x80135C34, Size: 60 bytes (15 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void shugaku_girl2_bg_set(void)
{
    device_cmd_send(0x203);
    dec_bg_cache_step(0x4761);
    event_log_add(2, 0x13);
    sub_sub_step_up();
}
