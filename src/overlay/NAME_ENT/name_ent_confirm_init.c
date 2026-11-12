/* name_ent_confirm_init - NAME_ENT overlay, Address: 0x80138074, Size: 676 bytes (169 insns) */
#include "types.h"

extern volatile u8 name_ent_p_mon;
extern volatile u8 name_ent_p_day;
extern u8 name_ent_p_blood;

extern u32 album_info_disp_tbl0[10];

extern u32 name_ent_init8_str0[];
extern u32 name_ent_sei[];
extern u32 name_ent_mei[];
extern u32 name_ent_adana[];
extern u32 name_ent_init8_str1[];
extern u32 name_ent_init8_str2[];
extern u32 name_ent_init8_str3[];
extern u32 name_ent_init8_str4[];

extern s32 name_ent_kakunin_xa[];

extern void text_sys_init(void);
extern void name_ent_confirm_cell_init(void);
extern void set_kanji_string(s32, s32, s32, u32 *, s32);
extern void set_XAsector(u32, u32, s32);
extern void device_cmd_send(s32);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

void name_ent_confirm_init(void)
{
    u32 val;

    text_sys_init();
    name_ent_confirm_cell_init();

    set_kanji_string(-24, 64, 10, name_ent_init8_str0, 0);
    set_kanji_string(-56, -55, 10, name_ent_sei, 0);
    set_kanji_string(0, -55, 10, name_ent_mei, 0);
    set_kanji_string(-56, -23, 10, name_ent_adana, 0);

    val = name_ent_p_mon + 1;
    if (!((u32)val < 10)) {
        set_kanji_string(-56, 8, 10, album_info_disp_tbl0[val / 10], 0);
        val = name_ent_p_mon + 1;
    }

    set_kanji_string(-40, 8, 10, album_info_disp_tbl0[val % 10], 0);

    val = name_ent_p_day + 1;
    if (!((u32)val < 10)) {
        set_kanji_string(-8, 8, 10, album_info_disp_tbl0[val / 10], 0);
        val = name_ent_p_day + 1;
    }

    set_kanji_string(8, 8, 10, album_info_disp_tbl0[val % 10], 0);

    switch (name_ent_p_blood) {
    case 0:
        set_kanji_string(-40, 40, 10, name_ent_init8_str1, 0);
        break;
    case 1:
        set_kanji_string(-40, 40, 10, name_ent_init8_str2, 0);
        break;
    case 2:
        set_kanji_string(-56, 40, 10, name_ent_init8_str3, 0);
        break;
    case 3:
        set_kanji_string(-40, 40, 10, name_ent_init8_str4, 0);
        break;
    }

    set_XAsector((*(u32 *)&name_ent_kakunin_xa[0]), (*(u32 *)&name_ent_kakunin_xa[1]), 0);
    device_cmd_send(768);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    sub_sub_step_up();
}
