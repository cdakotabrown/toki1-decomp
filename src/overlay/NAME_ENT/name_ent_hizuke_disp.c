/* @PostProc: patch_stack_complete --old 56 --new 72 */
/* @PostProc: remap_stack_slots --from 44 --to 68 --from 48 --to 60 --from 52 --to 64 --from 32 --to 52 */
/* name_ent_hizuke_disp - NAME_ENT overlay, Address: 0x80137350, Size: 872 bytes (218 insns) */
#include "types.h"

extern u8 name_ent_p_mon;
extern u8 name_ent_p_day;
extern u8 name_ent_p_blood;
extern u8 name_ent_fuji_mon;
extern u8 name_ent_fuji_day;
extern u8 name_ent_fuji_blood;

extern u32 album_info_disp_tbl0[10];
extern u32 space_str[];

extern u32 name_ent_hizuke_disp_str0[];
extern u32 name_ent_hizuke_disp_str1[];
extern u32 name_ent_hizuke_disp_str2[];
extern u32 name_ent_hizuke_disp_str3[];
extern u32 name_ent_hizuke_disp_str4[];
extern u32 name_ent_hizuke_disp_str5[];
extern u32 name_ent_hizuke_disp_str6[];
extern u32 name_ent_hizuke_disp_str7[];
extern u32 name_ent_hizuke_disp_str8[];
extern u32 name_ent_hizuke_disp_str9[];
extern u32 name_ent_hizuke_disp_str10[];
extern u32 name_ent_hizuke_disp_str11[];
extern u32 name_ent_hizuke_disp_str12[];

extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, u32 *, s32);
extern void k_sub_disp_start(s32);

void name_ent_hizuke_disp(s32 mode)
{
    s32 fieldA;
    s32 fieldB;
    s32 fieldC;
    s32 digit;

    if (mode != 0) {
        fieldA = name_ent_fuji_day;
        fieldB = name_ent_fuji_blood;
        fieldC = name_ent_fuji_mon;
    } else {
        fieldA = name_ent_p_day;
        fieldB = name_ent_p_blood;
        fieldC = name_ent_p_mon;
    }

    k_sub_reset();

    if (fieldC == 255) {
        set_kanji_string(-80, -88, 14, name_ent_hizuke_disp_str0, 0);
        set_kanji_string(-64, -88, 14, name_ent_hizuke_disp_str1, 0);
    } else {
        if (fieldC >= 9) {
            set_kanji_string(-80, -88, 14, album_info_disp_tbl0[1], 0);
        } else {
            set_kanji_string(-80, -88, 14, name_ent_hizuke_disp_str2, 0);
        }
        set_kanji_string(-64, -88, 14, album_info_disp_tbl0[(fieldC + 1) % 10], 0);
    }

    if (fieldA == 255) {
        set_kanji_string(-32, -88, 14, name_ent_hizuke_disp_str3, 0);
        set_kanji_string(-16, -88, 14, name_ent_hizuke_disp_str4, 0);
    } else {
        if (fieldA >= 9) {
            digit = (fieldA + 1) / 10;
            set_kanji_string(-32, -88, 14, album_info_disp_tbl0[digit], 0);
        } else {
            set_kanji_string(-32, -88, 14, space_str, 0);
        }
        set_kanji_string(-16, -88, 14, album_info_disp_tbl0[(fieldA + 1) % 10], 0);
    }

    switch (fieldB) {
    case 0:
        set_kanji_string(96, -88, 14, name_ent_hizuke_disp_str5, 0);
        set_kanji_string(112, -88, 14, name_ent_hizuke_disp_str6, 0);
        break;
    case 1:
        set_kanji_string(96, -88, 14, name_ent_hizuke_disp_str7, 0);
        set_kanji_string(112, -88, 14, name_ent_hizuke_disp_str8, 0);
        break;
    case 2:
        set_kanji_string(96, -88, 14, name_ent_hizuke_disp_str9, 0);
        set_kanji_string(112, -88, 14, name_ent_hizuke_disp_str10, 0);
        break;
    case 3:
        set_kanji_string(96, -88, 14, name_ent_hizuke_disp_str11, 0);
        set_kanji_string(112, -88, 14, name_ent_hizuke_disp_str12, 0);
        break;
    }

    k_sub_disp_start(6);
}
