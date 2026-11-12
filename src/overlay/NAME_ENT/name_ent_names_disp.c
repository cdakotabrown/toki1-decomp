/* name_ent_names_disp - NAME_ENT overlay, Address: 0x80134B24, Size: 124 bytes (31 insns) */
#include "types.h"

extern void k_sub_reset();
extern void set_kanji_string();
extern void k_sub_disp_start();

extern u8 name_ent_sei;
extern u8 name_ent_mei;
extern u8 name_ent_adana;

void name_ent_names_disp(void)
{
    k_sub_reset();
    set_kanji_string(-104, -88, 14, &name_ent_sei, 0);
    set_kanji_string(-48, -88, 14, &name_ent_mei, 0);
    set_kanji_string(72, -88, 14, &name_ent_adana, 0);
    k_sub_disp_start(3);
}
