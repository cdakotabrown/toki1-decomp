/* bunka_sd_init2c - BUNKA_SD overlay, Address: 0x80135C3C, Size: 52 bytes (13 insns) */
#include "types.h"

extern void device_port_init(s32, s32);
extern void bunka_sd_slot_init(void);
extern void sub_sub_step_up(void);

void bunka_sd_init2c(void)
{
    device_port_init(1, 513);
    bunka_sd_slot_init();
    sub_sub_step_up();
}
