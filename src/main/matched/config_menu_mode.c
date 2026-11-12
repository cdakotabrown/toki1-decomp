/* Address: 0x8005F6B8 */
/* Size: 620 bytes (155 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8  menu_csr;
extern u8  menu_mode;

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s16, s16, s32, void *, s32);
extern void sub_sub_step_up(void);

extern u8 config_menu_mode_str8;
extern u8 config_menu_mode_str9;
extern u8 config_menu_mode_str10;
extern u8 config_menu_mode_str11;
extern u8 config_menu_mode_str12;
extern u8 config_menu_mode_str13;
extern u8 config_menu_mode_str14;
extern u8 config_menu_mode_str15;
extern u8 config_menu_mode_str16;
extern u8 config_menu_mode_str17;
extern u8 config_menu_mode_str18;
extern u8 config_menu_mode_str19;

void config_menu_mode(void) {
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    if (pad_trig & 0x40) {
        goto_sub_sub_step(0);
        return;
    }

    if (!(pad_trig & 0x20)) {
        return;
    }

    switch (menu_csr) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &config_menu_mode_str8, 0);
        set_kanji_string(-86, 66, 1, &config_menu_mode_str9, 0);
        set_kanji_string(-128, 82, 0, &config_menu_mode_str10, 0);
        menu_mode = 0;
        sub_sub_step_up();
        break;
    case 1:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &config_menu_mode_str11, 0);
        set_kanji_string(-86, 66, 1, &config_menu_mode_str12, 0);
        set_kanji_string(-128, 82, 0, &config_menu_mode_str13, 0);
        menu_mode = 1;
        sub_sub_step_up();
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &config_menu_mode_str14, 0);
        set_kanji_string(-86, 66, 1, &config_menu_mode_str15, 0);
        set_kanji_string(-128, 82, 0, &config_menu_mode_str16, 0);
        menu_mode = 3;
        sub_sub_step_up();
        break;
    case 3:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &config_menu_mode_str17, 0);
        set_kanji_string(-86, 66, 1, &config_menu_mode_str18, 0);
        set_kanji_string(-128, 82, 0, &config_menu_mode_str19, 0);
        menu_mode = 2;
        sub_sub_step_up();
        break;
    }
}
