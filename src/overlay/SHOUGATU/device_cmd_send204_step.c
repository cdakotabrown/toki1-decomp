/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send204_step(void)
{
    device_cmd_send(516);
    sub_sub_step_up();
}
