/* geko_xa_sector_step -- GEKO overlay, Address: 0x8013BCA4, Size: 56 bytes (14 insns) */
#include "types.h"

extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

void geko_xa_sector_step(void)
{
    set_XAsector(0, 0, 12);
    device_cmd_send(768);
    sub_sub_step_up();
}
