/* Function: date_cmd1288_step */
/* Overlay: DATE */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();

s32 date_cmd1288_step(void)
{
    device_cmd_send(1288);
    return sub_sub_step_up();
}
