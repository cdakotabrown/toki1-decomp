/* Function: telephone_choice_menu_init */
/* Address: 0x8007837C */
/* Size: 788 bytes (197 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_8007837C.py */
#include "types.h"

extern s32 telephone_choice_menu_init_tbl2;
extern s32 telephone_choice_menu_init_tbl1;
extern u32 *telephone_choice_menu_init_tbl0[];
extern u8 screen_w[];
extern u8 now_icon;
extern s8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u32 *cal_cw_img;
extern u32 *cal_cw_anim_tbl;
extern u32 *cal_cw_hdr;
extern s32 cal_cw_attr;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 basyo_init_tbl3;
extern u8 telephone_choice_menu_init_str0;
extern u8 telephone_choice_menu_init_str1;
extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;

extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void class_clear_n(s32);

void telephone_choice_menu_init(void) {
    s16 spBC[14];
    s16 spA0[14];
    s16 sp84[14];
    s16 sp68[14];
    s32 i;

    set_kanji_string(-128, -84, 3, &telephone_choice_menu_init_str0, 0);

    for (i = 0; i < 14; i++) {
        sp84[i] = 72;
        spBC[i] = ((i / 5) * 88) - 136;
        sp68[i] = 14;
        spA0[i] = ((i % 5) * 16) - 56;

        if (i == 0) {
            set_kanji_string(spBC[i], spA0[i], 3, (u8 *)telephone_choice_menu_init_tbl2, 0);
        } else if (i == 13) {
            spA0[i] += 16;
            set_kanji_string(spBC[i], spA0[i], 3, &telephone_choice_menu_init_str1, 0);
        } else if (i == 12) {
            set_kanji_string(spBC[i], spA0[i], 3, (u8 *)telephone_choice_menu_init_tbl1, 0);
        } else if ((*(u32 *)(screen_w + i * 56 + 400) << 28) >> 31) {
            set_kanji_string(spBC[i], spA0[i], 3, (u8 *)telephone_choice_menu_init_tbl0[i], 0);
        } else {
            sp84[i] = 0;
        }
    }

    menu_set(0, 14, spBC, spA0, sp84, sp68);

    now_icon = 0;
    class_clear_n(96);

    cal_cw_prio = 0;
    cal_cw_mode = 64;
    cal_cw_attr = 0x40000000;
    cal_cw_disp = 132;
    cal_cw_img = &basyo_init_tbl1;
    cal_cw_anim_tbl = &basyo_init_tbl2;
    cal_cw_hdr = &_sys_default_tpage_set_tbl2;
    cal_cw_aux14 = basyo_init_tbl3;
    cal_cw_flag4 = 8;
    cal_cw_sel = 1;
    cal_cw_anim = 0;
    cal_cw_rate = 2;
    cal_cw_frame = 63;
    cal_cw_x = -16;
    cal_cw_y = -19;
}
