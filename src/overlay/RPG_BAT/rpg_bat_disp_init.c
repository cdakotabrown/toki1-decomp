/* @SymbolSize: 812 */
/* @PostProc: patch_insn_reg_field 0x34:rt:a0:v1 0x40:rs:a0:v1 0x8c:rt:v1:v0 0x90:rs:v1:v0 0x90:rt:v1:v0 0x98:rs:v1:v0 0xa4:rs:v1:v0 0xb0:rs:v1:v0 0xbc:rs:v1:v0 0xc8:rs:v1:v0 0xd4:rs:v1:v0 0x2a0:rt:v1:v0 0x2a4:rs:v1:v0 0x2a4:rt:v1:v0 0x2ac:rs:v1:v0 0x2e0:rt:v1:v0 0x2e4:rs:v1:v0 0x2e4:rt:v1:v0 0x2f0:rs:v1:v0 0x304:rt:a0:v1 0x308:rs:a0:v1 0x308:rt:a0:v1 0x310:rs:a0:v1 */
/* rpg_bat_disp_init - RPG_BAT overlay, Address: 0x801525B0, Size: 812 bytes (203 insns) */
#include "types.h"

extern s32 k_disp_start();
extern s32 k_disp_switch();
extern s32 k_sub_reset_point_set();
extern s32 rpg_bat_kanji_set();

extern char p_sei[];
extern char rpg_bat_disp_init_str0[];
extern char rpg_bat_disp_init_str1[];
extern char rpg_bat_disp_init_str2[];
extern char rpg_bat_disp_init_str3[];
extern char rpg_bat_disp_init_str4[];
extern char rpg_bat_disp_init_str5[];
extern char rpg_bat_disp_init_str6[];
extern char rpg_bat_disp_init_str7[];
extern char rpg_bat_disp_init_str8[];
extern char rpg_bat_disp_init_str9[];
extern char rpg_bat_disp_init_str10[];
extern char rpg_bat_disp_init_str11[];

extern s32 date_name_set_tbl0[];
extern u8 screen_bri;
extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern volatile s32 rpg_bat_unit0_disp;
extern volatile s32 rpg_bat_unit1_disp;
extern volatile s32 rpg_bat_unit2_disp;
extern volatile s32 rpg_bat_unit3_disp;

void rpg_bat_disp_init(void)
{
    s32 unused;
    s32 pal;
    s32 v0;
    s32 v1;

    screen_bri = 0;
    rpg_bat_unit0_disp = rpg_bat_kanji_set(177, 179, (s32)p_sei) - 1;
    v1 = rpg_bat_girl;
    pal = 195;
    if (v1 != 14) {
        pal = 211;
        rpg_bat_unit2_disp = rpg_bat_kanji_set(177, 195, date_name_set_tbl0[v1]) - 1;
    } else {
        pal = 195;
    }

    v0 = rpg_bat_mode;
    switch (v0) {
    case 0x10000:
        rpg_bat_unit3_disp = rpg_bat_kanji_set(177, pal, (s32)rpg_bat_disp_init_str0) - 1;
        rpg_bat_unit1_disp = rpg_bat_kanji_set(19, 187, (s32)rpg_bat_disp_init_str1) - 1;
        break;

    case 0x20000:
        rpg_bat_unit3_disp = rpg_bat_kanji_set(177, pal, (s32)rpg_bat_disp_init_str2) - 1;
        rpg_bat_unit1_disp = rpg_bat_kanji_set(19, 187, (s32)rpg_bat_disp_init_str3) - 1;
        break;

    case 0x40000:
        rpg_bat_unit3_disp = rpg_bat_kanji_set(177, pal, (s32)rpg_bat_disp_init_str4) - 1;
        rpg_bat_unit1_disp = rpg_bat_kanji_set(19, 187, (s32)rpg_bat_disp_init_str5) - 1;
        break;

    case 0x100000:
        rpg_bat_unit3_disp = rpg_bat_kanji_set(19, 187, (s32)rpg_bat_disp_init_str6) - 1;
        rpg_bat_unit1_disp = rpg_bat_kanji_set(19, 187, (s32)rpg_bat_disp_init_str7) - 1;
        break;

    case 0x80000000:
        rpg_bat_unit1_disp = rpg_bat_kanji_set(19, 187, (s32)rpg_bat_disp_init_str8) - 1;
        break;

    case 0x8000000:
        rpg_bat_unit2_disp = rpg_bat_kanji_set(23, 187, (s32)rpg_bat_disp_init_str9) - 1;
        rpg_bat_unit3_disp = rpg_bat_kanji_set(23, 187, (s32)rpg_bat_disp_init_str10) - 1;
        rpg_bat_unit1_disp = rpg_bat_kanji_set(23, 187, (s32)rpg_bat_disp_init_str11) - 1;
        break;
    }

    k_disp_start(rpg_bat_unit1_disp + 1);
    k_disp_switch(rpg_bat_unit0_disp, 1);
    k_disp_switch(rpg_bat_unit1_disp, 1);
    k_disp_switch(rpg_bat_unit3_disp, 0);
    if (rpg_bat_mode == 0x8000000) {
        k_disp_switch(rpg_bat_unit2_disp, 1);
        k_disp_switch(rpg_bat_unit3_disp, 0);
        k_disp_switch(rpg_bat_unit1_disp, 0);
    }
    if (rpg_bat_mode == 0x80000000) {
        k_disp_switch(rpg_bat_unit1_disp, 0);
    }
    if (rpg_bat_girl != 14) {
        k_disp_switch(rpg_bat_unit2_disp, 1);
    }
    k_sub_reset_point_set();
}
