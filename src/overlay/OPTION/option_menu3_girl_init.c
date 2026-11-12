/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=56:v0,57:v1,58:a0,30:a1,17:a2,32:a3,34:t0,9:t1 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* Address: 0x8013B430, Size: 344 bytes (86 insns) */
#include "types.h"

extern void cursor_init();
extern void menu_set();
extern void text_sys_init();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 option_menu3_girl_no;
extern s32 girl_full_name_tbl[];
extern u8 option_menu3_girl_init_str0[];
extern u8 option_menu3_girl_init_str1[];

void option_menu3_girl_init(void) {
    s16 padTop[4];
    s16 buf4[16];
    s16 buf3[16];
    s16 buf2[16];
    s16 buf1[16];
    s16 padBot[4];
    s32 i;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 y4;

    (void)&padTop;
    (void)&padBot;

    cursor_init(0);

    y1 = -96;
    y2 = -84;
    y3 = -72;
    y4 = -60;
    i = 0;
    do {
        buf3[i * 4 + 3] = (s16)y4;
        y4 += 48;
        buf3[i * 4 + 1] = (s16)y2;
        buf3[i * 4 + 2] = (s16)y3;
        buf3[i * 4 + 0] = (s16)y1;
        y1 += 48;
        y3 += 48;
        y2 += 48;
        buf4[i * 4 + 1] = 32;
        buf2[i * 4 + 1] = 16;
        buf1[i * 4 + 1] = 12;
        buf4[i * 4 + 2] = 32;
        buf2[i * 4 + 2] = 16;
        buf1[i * 4 + 2] = 12;
        buf4[i * 4 + 3] = 32;
        buf2[i * 4 + 3] = 16;
        buf1[i * 4 + 3] = 12;
        i++;
        buf4[i * 4 - 4] = 32;
        buf2[i * 4 - 4] = 16;
        buf1[i * 4 - 4] = 12;
    } while (y4 != 132);

    menu_set(0, 16, buf4, buf3, buf2, buf1);
    text_sys_init();
    set_kanji_string(-128, -24, 15, option_menu3_girl_init_str0, 0);
    set_kanji_string(-128, 0, 15, option_menu3_girl_init_str1, 0);
    set_kanji_string(64, -24, 15, girl_full_name_tbl[option_menu3_girl_no & 0xF], 0);
    k_disp_start(3);
    sub_sub_step_up();
}
