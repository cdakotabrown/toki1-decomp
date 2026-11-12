/* name_ent_dispa2 - NAME_ENT overlay, Address: 0x8013E544, Size: 104 bytes (26 insns) */
#include "types.h"

extern void text_sys_init();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 name_ent_dispa2_str0;
extern u8 name_ent_dispa2_str1;

void name_ent_dispa2(void)
{
    text_sys_init();
    set_kanji_string(-64, 32, 15, &name_ent_dispa2_str0, 0);
    set_kanji_string(-64, 48, 15, &name_ent_dispa2_str1, 0);
    k_disp_start(2);
    sub_sub_step_up();
}
