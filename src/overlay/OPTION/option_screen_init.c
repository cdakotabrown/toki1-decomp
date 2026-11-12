/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21c */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x220 */
/* Address: 0x801391B8, Size: 860 bytes (215 instructions) */
#include "types.h"
#include "tokimeki/libgs_types.h"

typedef struct {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 unk03;
    u8 unk04;
    u8 unk05;
    u8 unk06[6];
    void *unk0C;
    void *unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A[6];
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    void *unk34;
    u32 unk38;
    u32 unk3C[2];
} OptEnt;

extern void pn_set();
extern void draw2d3d();
extern void class_switch();
extern void prim_init();
extern void class_clear_all();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void text_sys_init();
extern void basyo_init();
extern void k_disp_start();
extern void set_kanji_string();
extern void cursor_init();
extern void menu_set();
extern void menu_bar_color();
extern void class_clear_n();
extern s32 rand();
extern void sub_sub_step_up();

extern u8 vblank_flag;
extern u32 class_max;
extern u8 now_icon;
extern u8 menu_csr;
extern u8 menu_mode;
extern u8 screen_bri;
extern u8 option_main_vab_wk;
extern u8 option_sub_vab_wk;
extern u8 cursor_cw_disp;

extern u8 option_screen_init_str0;
extern u8 option_screen_init_str1;
extern u8 option_screen_init_str2;
extern u8 option_screen_init_str3;
extern u8 option_screen_init_str4;
extern u8 option_screen_init_str5;
extern u8 option_screen_init_str6;
extern u8 option_screen_init_str7;

extern s16 option_screen_x_tbl[];
extern s16 option_screen_y_tbl[];
extern s16 option_screen_w_tbl[];
extern s16 option_screen_h_tbl[];

extern u8 mascot_init_tbl0;
extern u8 mascot_init_tbl1;
extern u8 _sys_default_tpage_set_tbl0;
extern s16 mascot_init_tbl2;

extern OptEnt class_w[];
extern LIBGS_BG Bg[];

void option_screen_init(void)
{
    s32 i;

    pn_set(4);
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(0);
    tpage_buf_clear();

    vblank_flag = 0;
    class_max = 160;

    text_sys_init();
    basyo_init(&option_screen_init_str0);
    k_disp_start(1);

    set_kanji_string(option_screen_x_tbl[0], option_screen_y_tbl[0], 0, &option_screen_init_str1, 0);
    set_kanji_string(option_screen_x_tbl[1], option_screen_y_tbl[1], 0, &option_screen_init_str2, 0);
    set_kanji_string(option_screen_x_tbl[2], option_screen_y_tbl[2], 0, &option_screen_init_str3, 0);
    set_kanji_string(option_screen_x_tbl[3], option_screen_y_tbl[3], 0, &option_screen_init_str4, 0);
    set_kanji_string(option_screen_x_tbl[4], option_screen_y_tbl[4], 0, &option_screen_init_str5, 0);
    set_kanji_string(option_screen_x_tbl[5], option_screen_y_tbl[5], 0, &option_screen_init_str6, 0);
    set_kanji_string(option_screen_x_tbl[6], option_screen_y_tbl[6], 3, &option_screen_init_str7, 0);

    now_icon = 0;
    menu_csr = 0;
    menu_mode = 0;

    cursor_init(0);

    cursor_cw_disp |= 1;

    option_main_vab_wk = 0;
    option_sub_vab_wk = 0;
    Bg[0].b = 0;
    Bg[0].g = 0;
    Bg[0].r = 0;
    screen_bri = 0;

    menu_set(0, 7, option_screen_x_tbl, option_screen_y_tbl, option_screen_w_tbl, option_screen_h_tbl);

    menu_bar_color(0, 0x005F7F3F, 9, 8);
    menu_bar_color(1, 0x007F7F7F, 2, 1);

    for (i = 0; i < 13; i++) {
        class_clear_n(i + 96);
        class_w[i + 96].unk01 = 2;
        class_w[i + 96].unk02 = 64;
        class_w[i + 96].unk38 = 0x40000000;
        class_w[i + 96].unk03 = 148;
        class_w[i + 96].unk0C = &mascot_init_tbl0;
        class_w[i + 96].unk10 = &mascot_init_tbl1;
        class_w[i + 96].unk34 = &_sys_default_tpage_set_tbl0;
        class_w[i + 96].unk14 = mascot_init_tbl2;
        class_w[i + 96].unk04 = 8;
        class_w[i + 96].unk05 = 1;
        class_w[i + 96].unk16 = 0;
        class_w[i + 96].unk18 = i;
        class_w[i + 96].unk2E = 3 - (rand() & 7);
        class_w[i + 96].unk32 = 4 - (rand() & 7);
        class_w[i + 96].unk2C = rand();
        class_w[i + 96].unk30 = rand();
        class_w[i + 96].unk26 = 0;
        class_w[i + 96].unk2A = 0;
    }

    sub_sub_step_up();
}
