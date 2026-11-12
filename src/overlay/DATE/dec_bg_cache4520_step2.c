/* Function: dec_bg_cache4520_step2 */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern void dec_bg_cache_step();

void dec_bg_cache4520_step2(void)
{
    device_cmd_send(515);
    dec_bg_cache_step(17696);
    sub_sub_step_up();
}
