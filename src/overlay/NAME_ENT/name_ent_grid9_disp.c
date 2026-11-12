/* name_ent_grid9_disp - NAME_ENT overlay, Address: 0x80132FD4, Size: 276 bytes (69 insns) */
#include "types.h"

extern u32 name_ent_hira_tbl;
extern u32 name_ent_hira_tbl_1;
extern u32 name_ent_hira_tbl_2;
extern u32 name_ent_hira_tbl_3;
extern u32 name_ent_hira_tbl_4;
extern u32 name_ent_hira_tbl_5;
extern u32 name_ent_hira_tbl_6;
extern u32 name_ent_hira_tbl_8;
extern u8 name_ent_grid9_disp_str0[];

extern void set_kanji_string(s32, s32, s32, u32, s32);

void name_ent_grid9_disp(void)
{

    set_kanji_string(-120, 56, 10, name_ent_hira_tbl, 0);
    set_kanji_string(-24, 56, 10, name_ent_hira_tbl_1, 0);
    set_kanji_string(72, 56, 10, name_ent_hira_tbl_2, 0);

    set_kanji_string(-120, 72, 10, name_ent_hira_tbl_3, 0);
    set_kanji_string(-24, 72, 10, name_ent_hira_tbl_4, 0);
    set_kanji_string(72, 72, 10, name_ent_hira_tbl_5, 0);

    set_kanji_string(-120, 88, 10, name_ent_hira_tbl_6, 0);
    set_kanji_string(-24, 88, 10, (u32)name_ent_grid9_disp_str0, 0);
    set_kanji_string(72, 88, 10, name_ent_hira_tbl_8, 0);
}
