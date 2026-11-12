/* Address: 0x8014AD5C */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern void basyo_init();
extern void k_sub_reset_point_set();
extern void k_sub_disp_start();
extern void set_dec_bri();
extern void sub_step_up();

extern u8 etc_soudan_basyo_init_str0;

void etc_soudan_basyo_init(void)
{
    basyo_init(&etc_soudan_basyo_init_str0);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    set_dec_bri(128);
    sub_step_up();
}
