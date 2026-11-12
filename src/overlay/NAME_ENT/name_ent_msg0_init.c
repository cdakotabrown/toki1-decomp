/* name_ent_msg0_init - NAME_ENT overlay, Address: 0x8013C69C, Size: 112 bytes (28 insns) */
#include "types.h"

extern void save_flag_clear();
extern void text_sys_init();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 name_ent_msg0_init_str0;
extern u8 name_ent_msg0_init_str1;

void name_ent_msg0_init(void)
{
    save_flag_clear();
    text_sys_init();
    set_kanji_string(-64, 32, 15, &name_ent_msg0_init_str0, 0);
    set_kanji_string(-64, 48, 15, &name_ent_msg0_init_str1, 0);
    k_disp_start(2);
    sub_sub_step_up();
}
