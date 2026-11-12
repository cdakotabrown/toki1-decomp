/* Address: 0x8005E500 */
/* Size: 332 bytes (83 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8  menu_csr;

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s16, s16, s32, void *, s32);
extern void k_speed_set(s32);
extern void sub_sub_step_up(void);

extern u8 system_config_moji_speed_str0;
extern u8 system_config_moji_speed_str1;
extern u8 system_config_moji_speed_str2;

void system_config_moji_speed(void) {
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
        set_kanji_string(-128, 50, 0, &system_config_moji_speed_str0, 0);
        k_speed_set(3);
        sub_sub_step_up();
        break;
    case 1:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &system_config_moji_speed_str1, 0);
        k_speed_set(1);
        sub_sub_step_up();
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &system_config_moji_speed_str2, 0);
        k_speed_set(0);
        sub_sub_step_up();
        break;
    }
}
