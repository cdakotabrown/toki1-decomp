/* device_port_init0_step -- GEKO overlay, Address: 0x8013BA70, Size: 44 bytes (11 insns) */
#include "types.h"

extern void device_port_init(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

void device_port_init0_step(void)
{
    device_port_init(0, 512);
    sub_sub_step_up();
}
