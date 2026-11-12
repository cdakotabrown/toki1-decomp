/* name_ent_kata_disp - NAME_ENT overlay, Address: 0x801332FC, Size: 532 bytes (133 insns) */
#include "types.h"

extern u32 name_ent_kata_tbl;
extern u32 name_ent_kata_tbl_1;
extern u32 name_ent_kata_tbl_2;
extern u32 name_ent_kata_tbl_3;
extern u32 name_ent_kata_tbl_4;
extern u32 name_ent_kata_tbl_5;
extern u32 name_ent_kata_tbl_6;
extern u32 name_ent_kata_tbl_7;
extern u32 name_ent_kata_tbl_8;
extern u32 name_ent_kata_tbl_9;
extern u32 name_ent_kata_tbl_10;
extern u32 name_ent_kata_tbl_11;
extern u32 name_ent_kata_tbl_12;
extern u32 name_ent_kata_tbl_13;
extern u32 name_ent_kata_tbl_14;
extern u32 name_ent_kata_tbl_15;
extern u32 name_ent_kata_tbl_16;

extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u32, s32);

void name_ent_kata_disp(u32 title)
{
    text_sys_init();

    set_kanji_string(-152, -104, 10, title, 0);

    set_kanji_string(-120, -48, 10, name_ent_kata_tbl, 0);
    set_kanji_string(-120, -32, 10, name_ent_kata_tbl_3, 0);
    set_kanji_string(-120, -16, 10, name_ent_kata_tbl_6, 0);
    set_kanji_string(-120, 0, 10, name_ent_kata_tbl_9, 0);
    set_kanji_string(-120, 16, 10, name_ent_kata_tbl_12, 0);
    set_kanji_string(-120, 32, 10, name_ent_kata_tbl_15, 0);

    set_kanji_string(-24, -48, 10, name_ent_kata_tbl_1, 0);
    set_kanji_string(-24, -32, 10, name_ent_kata_tbl_4, 0);
    set_kanji_string(-24, -16, 10, name_ent_kata_tbl_7, 0);
    set_kanji_string(-24, 0, 10, name_ent_kata_tbl_10, 0);
    set_kanji_string(-24, 16, 10, name_ent_kata_tbl_13, 0);
    set_kanji_string(-24, 32, 10, name_ent_kata_tbl_16, 0);

    set_kanji_string(72, -48, 10, name_ent_kata_tbl_2, 0);
    set_kanji_string(72, -32, 10, name_ent_kata_tbl_5, 0);
    set_kanji_string(72, -16, 10, name_ent_kata_tbl_8, 0);
    set_kanji_string(72, 0, 10, name_ent_kata_tbl_11, 0);
    set_kanji_string(72, 16, 10, name_ent_kata_tbl_14, 0);
}
