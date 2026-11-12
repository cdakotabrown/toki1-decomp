/* Address: 0x801327AC */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern u8 etc_basyo_init6_str0;

extern void sub_sub_step_up();
extern void device_cmd_send();
extern void text_sys_init();
extern void k_disp_start();
extern void basyo_init();

void etc_epilogue_prep_basyo_init(void)
{
    device_cmd_send(512);
    text_sys_init();
    basyo_init(&etc_basyo_init6_str0);
    k_disp_start(1);
    sub_sub_step_up();
}
