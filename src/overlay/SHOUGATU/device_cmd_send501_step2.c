/* @SymbolSize: 40 */
/* /* device_cmd_send501_step2 - SHOUGATU overlay, Address: 0x8014137C */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send501_step2(void)
{
    device_cmd_send(1281);
    sub_sub_step_up();
}
