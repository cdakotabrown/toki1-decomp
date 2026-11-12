/* Function: config_click_mode_init */
/* Address: 0x8005EEF8 */
/* Size: 328 bytes (82 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(s32 *);
extern void system_config_disp(s32);
extern void set_kanji_string(s32, s32, s32, s32 *, s32);
extern void k_sub_reset_point_set(void);
extern s32 menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern s32 config_click_mode_init_str0;
extern s32 config_click_mode_init_str1;
extern s32 config_click_mode_init_str2;
extern s32 config_click_mode_init_str3;
extern u8 click_mode;
extern u8 menu_csr;

void config_click_mode_init(void) {
    s32 i;
    s32 v0;
    s16 arr_v1[4];
    s16 arr_a0[4];
    s16 arr_a2[4];
    s16 arr_v0[4];

    text_sys_init();
    basyo_init(&config_click_mode_init_str0);
    system_config_disp(2);

    for (i = 0; i < 2; i++) {
        arr_v1[i] = -4;
        arr_a0[i] = (s16)(i * 16 - 64);
        arr_a2[i] = 108;
        arr_v0[i] = 16;
    }

    set_kanji_string(arr_v1[0], arr_a0[0], 3, &config_click_mode_init_str1, 0);
    set_kanji_string(arr_v1[1], arr_a0[1], 3, &config_click_mode_init_str2, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &config_click_mode_init_str3, 0);

    menu_set(1, 2, arr_v1, arr_a0, arr_a2, arr_v0);

    v0 = click_mode;
    switch (v0) {
    case 0:
        menu_csr = 0;
        break;
    case 1:
        menu_csr = 1;
        break;
    }
    k_sub_disp_start(0);
    sub_sub_step_up();
}
