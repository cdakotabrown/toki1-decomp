/* @SymbolSize: 648 */
/* name_ent_fuji_confirm_init - NAME_ENT overlay, Address: 0x801388E0, Size: 664 bytes (166 insns) */
#include "types.h"

extern volatile u8 name_ent_fuji_mon;
extern volatile u8 name_ent_fuji_day;
extern u8 name_ent_fuji_blood;

extern u32 album_info_disp_tbl0[10];

extern u32 name_ent_fuji_confirm_init_str0[];
extern u32 name_ent_fuji_confirm_init_str1[];
extern u32 name_ent_fuji_confirm_init_str2[];
extern u32 name_ent_fuji_confirm_init_str3[];
extern u32 name_ent_fuji_confirm_init_str4[];
extern u32 name_ent_fuji_confirm_init_str5[];
extern u32 name_ent_fuji_confirm_init_str6[];

extern s32 name_ent_kakunin_xa[];

extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u32 *, s32);
extern void name_slots_init(void);
extern void set_XAsector(u32, u32, s32);
extern void device_cmd_send(s32);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

void name_ent_fuji_confirm_init(void)
{
    u32 val;

    text_sys_init();

    set_kanji_string(-24, 64, 10, name_ent_fuji_confirm_init_str0, 0);
    set_kanji_string(-56, -56, 10, name_ent_fuji_confirm_init_str1, 0);
    set_kanji_string(0, -56, 10, name_ent_fuji_confirm_init_str2, 0);

    val = name_ent_fuji_mon + 1;
    if (!((u32)val < 10)) {
        set_kanji_string(-56, -24, 10, album_info_disp_tbl0[val / 10], 0);
        val = name_ent_fuji_mon + 1;
    }

    set_kanji_string(-40, -24, 10, album_info_disp_tbl0[val % 10], 0);

    val = name_ent_fuji_day + 1;
    if (!((u32)val < 10)) {
        set_kanji_string(-8, -24, 10, album_info_disp_tbl0[val / 10], 0);
        val = name_ent_fuji_day + 1;
    }

    set_kanji_string(8, -24, 10, album_info_disp_tbl0[val % 10], 0);

    switch (name_ent_fuji_blood) {
    case 0:
        set_kanji_string(-40, 8, 10, name_ent_fuji_confirm_init_str3, 0);
        break;
    case 1:
        set_kanji_string(-40, 8, 10, name_ent_fuji_confirm_init_str4, 0);
        break;
    case 2:
        set_kanji_string(-56, 8, 10, name_ent_fuji_confirm_init_str5, 0);
        break;
    case 3:
        set_kanji_string(-40, 8, 10, name_ent_fuji_confirm_init_str6, 0);
        break;
    }

    name_slots_init();
    set_XAsector((*(u32 *)&name_ent_kakunin_xa[0]), (*(u32 *)&name_ent_kakunin_xa[1]), 0);
    device_cmd_send(768);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    sub_sub_step_up();
}
