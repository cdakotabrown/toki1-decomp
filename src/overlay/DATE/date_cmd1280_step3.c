/* Function: date_cmd1280_step3 */
/* Overlay: DATE */
/* Address: 0x8015724C */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();

s32 date_cmd1280_step3(void)
{
    device_cmd_send(1280);
    return sub_sub_step_up();
}
