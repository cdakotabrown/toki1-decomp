/* @SymbolSize: 48 */
/* device_cmd_send1281_step - KANGEI overlay, Address: 0x80135230, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send1281_step(void)
{
    device_cmd_send(36);
    device_cmd_send(1281);
    sub_sub_step_up();
}
