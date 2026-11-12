/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send501_step(void)
{
    device_cmd_send(1281);
    sub_sub_step_up();
}
