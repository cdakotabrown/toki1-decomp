/* Address: 0x8014361C */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();
extern void load_header_check();
extern void set_XAsector();

void etc_xa_start(void)
{
    load_header_check(0);
    set_XAsector(0x07000001, 0x02000E0C, 15);
    device_cmd_send(768);
    sub_sub_step_up();
}
