/* Function: date_cmd1280_step2 */
/* Overlay: DATE */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();

s32 date_cmd1280_step2(void)
{
    device_cmd_send(1280);
    return sub_sub_step_up();
}
