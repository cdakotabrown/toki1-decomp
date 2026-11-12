/* @SymbolSize: 40 */
/* /* device_cmd_send200_step - SHOUGATU overlay, Address: 0x80134F38, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send200_step(void)
{
    device_cmd_send(512);
    sub_sub_step_up();
}
