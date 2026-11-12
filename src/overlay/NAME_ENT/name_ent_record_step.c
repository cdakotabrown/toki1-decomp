/* name_ent_record_step - NAME_ENT overlay, Address: 0x80138CB8, Size: 80 bytes (20 insns) */
#include "types.h"

extern void device_cmd_send();
extern void class_clear_f();
extern void name_ent_girl_para_set();
extern void text_sys_init();
extern void sub_sub_step_up();

void name_ent_record_step(void)
{
    device_cmd_send(5);
    device_cmd_send(193);
    device_cmd_send(3);
    class_clear_f();
    name_ent_girl_para_set();
    text_sys_init();
    sub_sub_step_up();
}
