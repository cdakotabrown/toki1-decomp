/* Address: 0x80136C10 */
/* Size: 96 bytes (24 insns) */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 taco_view_flag;

extern void sub_sub_step_up();
extern void back_clear_switch();
extern void device_port_init2();
extern void all_object_disp_off();
extern void taco_drop_init();

void taco_clear0(void)
{
    taco_drop_init();
    device_port_init2(0, 514);
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    back_clear_switch(1);
    taco_view_flag = 0;
    all_object_disp_off();
    sub_sub_step_up();
}
