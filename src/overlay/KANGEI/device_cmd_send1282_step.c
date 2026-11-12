/* @SymbolSize: 48 */
/* device_cmd_send1282_step - KANGEI overlay, Address: 0x80135818, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send1282_step(void)
{
    device_cmd_send(36);
    device_cmd_send(1282);
    sub_sub_step_up();
}
