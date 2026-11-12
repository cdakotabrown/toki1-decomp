/* Function: cursor_speed */
/* Address: 0x8005FC58 */
/* Size: 896 bytes (224 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern u8 menu_csr;
extern u8 csr_speed;
extern u8 pad_type;

extern char cursor_speed_str0[];
extern char cursor_speed_str1[];
extern char cursor_speed_str2[];
extern char cursor_speed_str3[];
extern char cursor_speed_str4[];
extern char cursor_speed_str5[];
extern char cursor_speed_str6[];
extern char cursor_speed_str7[];
extern char cursor_speed_str8[];
extern char cursor_speed_str9[];
extern char cursor_speed_str10[];
extern char cursor_speed_str11[];
extern char cursor_speed_str12[];
extern char cursor_speed_str13[];
extern u8 mascot_flag2[];
extern u8 mascot_flag[];

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, char *, s32);
extern void sub_sub_step_up(void);

void cursor_speed(void) {
    s32 pad;
    u8 mode;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    pad = pad_trig;

    if (pad & 0x40) {
        goto_sub_sub_step(0);
        return;
    }

    if (!(pad & 0x20)) {
        return;
    }

    mode = menu_csr;

    switch (mode) {
    default:
        return;
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, cursor_speed_str0, 0);
        if (mascot_flag[8] != 0 || mascot_flag2[8] != 0) {
            set_kanji_string(-128, 66, 0, cursor_speed_str1, 0);
        } else {
            set_kanji_string(-128, 66, 0, cursor_speed_str2, 0);
        }
        csr_speed = 1;
        pad_type = 255;
        sub_sub_step_up();
        return;

    case 1:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, cursor_speed_str3, 0);
        if (mascot_flag[8] != 0 || mascot_flag2[8] != 0) {
            set_kanji_string(-128, 66, 0, cursor_speed_str4, 0);
        } else {
            set_kanji_string(-128, 66, 0, cursor_speed_str5, 0);
        }
        csr_speed = 2;
        pad_type = 255;
        sub_sub_step_up();
        return;

    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, cursor_speed_str6, 0);
        set_kanji_string(-128, 66, 0, cursor_speed_str7, 0);
        csr_speed = 3;
        pad_type = 255;
        sub_sub_step_up();
        return;

    case 3:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, cursor_speed_str8, 0);
        if (mascot_flag[8] != 0 || mascot_flag2[8] != 0) {
            set_kanji_string(-128, 66, 0, cursor_speed_str9, 0);
        } else {
            set_kanji_string(-128, 66, 0, cursor_speed_str10, 0);
        }
        csr_speed = 4;
        pad_type = 255;
        sub_sub_step_up();
        return;

    case 4:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, cursor_speed_str11, 0);
        if (mascot_flag[8] != 0 || mascot_flag2[8] != 0) {
            set_kanji_string(-128, 66, 0, cursor_speed_str12, 0);
        } else {
            set_kanji_string(-128, 66, 0, cursor_speed_str13, 0);
        }
        csr_speed = 6;
        pad_type = 255;
        sub_sub_step_up();
        return;
    }
}
