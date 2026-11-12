/* Function: date_cmd1285_step */
/* Overlay: DATE */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();

s32 date_cmd1285_step(void)
{
    device_cmd_send(1285);
    return sub_sub_step_up();
}
