/* Address: 0x80136CE8 */
/* Size: 40 bytes (10 insns) */
#include "types.h"

extern void goto_sub_step();
extern void device_cmd_send();

void taco_cmd_send7(void)
{
    device_cmd_send(193);
    goto_sub_step(2);
}
