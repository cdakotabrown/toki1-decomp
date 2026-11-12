/* geko_name_set_step -- GEKO overlay, Address: 0x801395AC, Size: 72 bytes (18 insns), Matched */
#include "types.h"

extern s32 get_g_zyotai_h(s32 a0);
extern void bustup_timer_clamp(s32 a0);
extern void device_cmd_send(s32 a0);
extern void date_name_set(s32 a0);
extern void sub_sub_step_up(void);

extern u8 now_girl;

void geko_name_set_step(void)
{
    s32 result;
    result = get_g_zyotai_h(8);
    bustup_timer_clamp(result);
    device_cmd_send(1281);
    date_name_set(now_girl);
    sub_sub_step_up();
}
