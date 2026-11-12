/* Function: save_menu_init */
/* Address: 0x8005BF64 */
/* Size: 416 bytes (104 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init2(void *);
extern void k_sub_reset_point_set(void);
extern void icon_disp_switch(s32);
extern void data_save_load_class_init(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_disp_switch(s32, s32);
extern void k_sub_disp_start(s32);
extern void save_flag_clear(void);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void sub_sub_step_up(void);

extern u8 save_menu_init_str0;
extern u8 save_menu_init_str1;
extern u8 save_menu_init_str2;
extern u8 save_menu_init_str3;
extern u8 save_menu_init_str4;
extern u8 menu_csr;

void save_menu_init(void) {
    s32 pad;
    s16 arr_x[2];
    s16 arr_y[2];
    s16 arr_w[2];
    s16 arr_h[2];

    text_sys_init();
    basyo_init2(&save_menu_init_str0);
    k_sub_reset_point_set();
    icon_disp_switch(0);
    data_save_load_class_init();

    arr_x[1] = 16;
    arr_y[1] = -16;
    arr_w[0] = 48;
    arr_w[1] = 48;
    arr_h[0] = 20;
    arr_h[1] = 20;
    arr_y[0] = -16;
    arr_x[0] = -80;

    set_kanji_string(arr_x[0], arr_y[0], 15, &save_menu_init_str1, 0);
    set_kanji_string(arr_x[1], arr_y[1], 15, &save_menu_init_str2, 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &save_menu_init_str3, 0);
    set_kanji_string(-128, 66, 0, &save_menu_init_str4, 0);
    set_kanji_string(-128, 74, 0, &save_menu_init_str4, 0);
    set_kanji_string(-128, 82, 0, &save_menu_init_str4, 0);
    set_kanji_string(-128, 90, 0, &save_menu_init_str4, 0);
    k_sub_disp_start(1);
    save_flag_clear();
    menu_set(1, 2, arr_x, arr_y, arr_w, arr_h);
    menu_csr = 0;
    sub_sub_step_up();
}
