/* @PostProc: patch_stack_complete --old 208 --new 216 */
/* Address: 0x8013ADB4, Size: 380 bytes (95 instructions) */
#include "types.h"

extern void cursor_init();
extern void menu_set();
extern void text_sys_init();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 option_menu1_init_str0;
extern u8 option_menu1_init_str1;
extern u8 p_sei;
extern u8 p_mei;
extern u8 p_adana;

void option_menu1_init(void)
{
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];
    s16 pad0[4];
    s32 i;

    (void)&pad0;
    cursor_init(0);

    for (i = 0; i < 16; i++) {
        a[i] = 32;
        b[i] = -96 + (i * 12);
        c[i] = 16;
        d[i] = 12;
    }

    menu_set(0, 16, a, b, c, d);

    text_sys_init();
    set_kanji_string(-128, -24, 15, &option_menu1_init_str0, 0);
    set_kanji_string(-128, 0, 15, &option_menu1_init_str1, 0);
    set_kanji_string(96, -24, 15, &p_sei, 0);
    set_kanji_string(96, -12, 15, &p_mei, 0);
    set_kanji_string(96, 0, 15, &p_adana, 0);
    k_disp_start(5);
    sub_sub_step_up();
}
