/* Function: dec_bg_cache480f_step */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern void dec_bg_cache_step();

void dec_bg_cache480f_step(void)
{
    device_cmd_send(516);
    dec_bg_cache_step(18447);
    sub_sub_step_up();
}
