/* Address: 0x8005F4AC */
/* Size: 524 bytes (131 instructions) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 menu_mode;
extern s8 menu_csr;

extern void text_sys_init(void);
extern void basyo_init(u8 *);
extern void system_config_disp(s32);
extern s32 set_kanji_string(s32, s32, s32, void *, s32);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern u8 config_menu_mode_init_str0[];
extern u8 config_menu_mode_init_str1[];
extern u8 config_menu_mode_init_str2[];
extern u8 config_menu_mode_init_str3[];
extern u8 config_menu_mode_init_str4[];
extern u8 config_menu_mode_init_str5[];
extern u8 config_menu_mode_init_str6[];
extern u8 config_menu_mode_init_str7[];

void config_menu_mode_init(void) {
    s32 i;
    u8 temp_v0;
    s16 w[4];
    s16 h[4];
    s16 x[4];
    s16 y[4];

    text_sys_init();
    basyo_init(config_menu_mode_init_str0);
    system_config_disp(3);

    for (i = 0; i < 4; i++) {
        w[i] = -4;
        h[i] = i * 16 - 64;
        x[i] = 108;
        y[i] = 16;
    }

    set_kanji_string(w[0], h[0], 3, config_menu_mode_init_str1, 0);
    set_kanji_string(w[1], h[1], 3, config_menu_mode_init_str2, 0);
    set_kanji_string(w[2], h[2], 3, config_menu_mode_init_str3, 0);
    set_kanji_string(w[3], h[3], 3, config_menu_mode_init_str4, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, config_menu_mode_init_str5, 0);
    set_kanji_string(-128, 66, 0, config_menu_mode_init_str6, 0);
    set_kanji_string(-128, 82, 0, config_menu_mode_init_str7, 0);
    menu_set(1, 4, w, h, x, y);

    temp_v0 = menu_mode;
    switch (temp_v0) {
    default:
    case 1:
        menu_csr = 1;
        break;
    case 0:
        menu_csr = 0;
        break;
    case 3:
        menu_csr = 2;
        break;
    case 2:
        menu_csr = 3;
        break;
    }
    k_sub_disp_start(0);
    sub_sub_step_up();
}
