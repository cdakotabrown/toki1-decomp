/* name_ent_album_create_slot_disp - NAME_ENT overlay, Address: 0x8013DBCC, Size: 132 bytes (33 insns) */
#include "types.h"

extern void text_sys_init();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 name_ent_disp44_str0;
extern u8 name_ent_disp44_str1;
extern u8 name_ent_disp44_str2;

void name_ent_album_create_slot_disp(void)
{
    text_sys_init();
    set_kanji_string(-64, 32, 15, &name_ent_disp44_str0, 0);
    set_kanji_string(-64, 48, 15, &name_ent_disp44_str1, 0);
    set_kanji_string(-64, 64, 15, &name_ent_disp44_str2, 0);
    k_disp_start(3);
    sub_sub_step_up();
}
