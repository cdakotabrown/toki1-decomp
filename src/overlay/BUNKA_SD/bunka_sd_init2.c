/* bunka_sd_init2 - BUNKA_SD overlay, Address: 0x801347CC, Size: 52 bytes (13 insns) */
#include "types.h"

extern void device_port_init(s32, s32);
extern void sub_sub_step_up(void);
extern void tally_sprite_init(void);

void bunka_sd_init2(void)
{
    device_port_init(1, 513);
    tally_sprite_init();
    sub_sub_step_up();
}
