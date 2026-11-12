/* device_cmd_send200_step - ENDING overlay, Address: 0x80132F00, Size: 40 bytes (10 insns)  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void device_cmd_send();
extern void sub_sub_step_up();

void device_cmd_send200_step(void)
{
    device_cmd_send(512);
    sub_sub_step_up();
}
