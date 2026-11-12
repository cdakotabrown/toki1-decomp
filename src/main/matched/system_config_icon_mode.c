/* Function: system_config_icon_mode */
/* Address: 0x8005E808 */
/* Size: 476 bytes (119 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:14 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8 menu_csr;
extern u8 icon_mode;
extern u8 icon_num;

extern s32 system_config_icon_mode_str0;
extern s32 system_config_icon_mode_str1;
extern s32 system_config_icon_mode_str2;
extern s32 system_config_icon_mode_str3;
extern s32 system_config_icon_mode_str4;
extern s32 system_config_icon_mode_str5;

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 a0);
extern void k_sub_reset(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, void *a3, s32 a4);
extern void icon_init(void);
extern void sub_sub_step_up(void);
extern void icon_menu_set(s32 a0);

void system_config_icon_mode(void) {
    s32 buttons;
    s32 mode;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    buttons = pad_trig;

    if (buttons & 0x40) {
        goto_sub_sub_step(0);
        return;
    }

    if (!(buttons & 0x20)) {
        return;
    }

    mode = menu_csr;

    switch (mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &system_config_icon_mode_str0, 0);
        set_kanji_string(-128, 66, 0, &system_config_icon_mode_str1, 0);
        icon_mode = 0;
        icon_init();
        sub_sub_step_up();
        break;
    case 1:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &system_config_icon_mode_str2, 0);
        set_kanji_string(-128, 66, 0, &system_config_icon_mode_str3, 0);
        icon_mode = 1;
        icon_init();
        sub_sub_step_up();
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &system_config_icon_mode_str4, 0);
        set_kanji_string(-128, 66, 0, &system_config_icon_mode_str5, 0);
        if (icon_mode != 2) {
            icon_num = 15;
        }
        icon_mode = 2;
        icon_menu_set(icon_num);
        sub_sub_step_up();
        break;
    }
}
