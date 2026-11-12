/* Function: date_cmd512_step3 */
/* Overlay: DATE */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();

s32 date_cmd512_step3(void)
{
    device_cmd_send(512);
    return sub_sub_step_up();
}
