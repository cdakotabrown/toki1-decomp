/* name_ent_fuji_hizuke_init - NAME_ENT overlay, Address: 0x80138694, Size: 140 bytes (35 insns) */
#include "types.h"

extern void text_sys_init();
extern void name_ent_key_init();
extern void set_kanji_string();
extern void set_XAsector();
extern void device_cmd_send();
extern void k_sub_reset_point_set();
extern void name_ent_hizuke_disp();
extern void k_sub_disp_start();
extern void sub_sub_step_up();

extern u8 name_ent_cell_no;
extern u8 name_ent_fuji_hizuke_init_str0;
extern s32 name_ent_fuji_hizuke_xa[];

void name_ent_fuji_hizuke_init(void)
{
    text_sys_init();
    name_ent_cell_no = 0;
    set_kanji_string(-152, -104, 10, &name_ent_fuji_hizuke_init_str0, 0);
    name_ent_key_init();
    set_XAsector(name_ent_fuji_hizuke_xa[0], name_ent_fuji_hizuke_xa[1], 0);
    device_cmd_send(768);
    k_sub_reset_point_set();
    name_ent_hizuke_disp(1);
    k_sub_disp_start(0);
    sub_sub_step_up();
}
