/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=128 IDO52_FORCE_WEB_COLOR=61:v0,62:v1,63:a0,33:a1,20:a2,35:a3,37:t0,12:t1 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* Address: 0x8013A624, Size: 488 bytes (122 insns) */
#include "types.h"

extern void cursor_init();
extern void menu_set();
extern void pn_set();
extern void strcpy();
extern void text_sys_init();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 return_schedule_tbl0;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 p_sei[];
extern u8 p_mei[];
extern u8 p_adana[];
extern u8 option_menu0_init_str0[];
extern u8 option_menu0_init_str1[];
extern u8 option_menu0_init_str2[];
extern u8 option_menu0_init_str3[];
extern u8 option_menu0_init_str4[];

void option_menu0_init(void) {
    s16 padTop[4];
    s16 buf4[16];
    s16 buf3[16];
    s16 buf2[16];
    s16 buf1[16];
    s16 padBot[8];
    s32 i;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 y4;

    (void)&padTop;
    (void)&padBot;

    return_schedule_tbl0 = 1;
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
    back_clear_g = 128;
    back_clear_r = 128;
    back_clear_b = 128;
    pn_set(4);
    strcpy(p_sei, option_menu0_init_str0);
    strcpy(p_mei, option_menu0_init_str1);
    strcpy(p_adana, option_menu0_init_str2);
    text_sys_init();
    set_kanji_string(-128, -24, 15, option_menu0_init_str3, 0);
    set_kanji_string(-128, 0, 15, option_menu0_init_str4, 0);
    set_kanji_string(32, -24, 15, p_sei, 0);
    set_kanji_string(32, -12, 15, p_mei, 0);
    set_kanji_string(32, 0, 15, p_adana, 0);
    k_disp_start(5);
    sub_sub_step_up();
}
