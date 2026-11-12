/* geko_reset_step -- GEKO overlay, Address: 0x80134694, Size: 48 bytes (12 insns) */
#include "types.h"

extern void k_reset(s32 a0);
extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

void geko_reset_step(void)
{
    k_reset(1);
    device_cmd_send(512);
    sub_sub_step_up();
}
