/* @SymbolSize: 88 */
/* name_ent_intro_exit - NAME_ENT overlay, Address: 0x80144920, Size: 88 bytes (22 insns) */
#include "types.h"

extern void white_light();
extern void CDQ_init();
extern void vram_area_clear();
extern void prim_init();
extern void class_clear();
extern void device_cmd_send();
extern void sub_step_up();

void name_ent_intro_exit(void)
{
    white_light(255);
    CDQ_init();
    vram_area_clear(0);
    prim_init();
    class_clear(0);
    device_cmd_send(113);
    device_cmd_send(127);
    sub_step_up();
}
