/* Address: 0x800603B0 */
/* Size: 584 bytes (146 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 mascot_sel_init_str0[];
extern u8 mascot_sel_init_str1[];
extern u8 mascot_sel_init_str2[];
extern void *date_name_set_tbl0[];
extern s16 cursor_work_copy_tbl0;
extern u8 mascot_flag2[];
extern s8 menu_csr;
extern u8 mascot_flag[];

extern void sub_sub_step_up(void);
extern void text_sys_init(void);
extern void set_kanji_string(s16 x, s16 y, u8 mode, void *str, s32 arg4);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32 arg0);
extern void menu_set(s32 arg0, s32 arg1, s16 *x, s16 *y, s16 *w, s16 *h);
extern void system_config_disp(s32 sel);
extern void basyo_init(u8 *str);

void mascot_sel_init(void)
{
    s16 unused[6];
    s16 x[14];
    s16 y[14];
    s16 w[14];
    s16 h[14];
    s32 i;

    text_sys_init();
    basyo_init(mascot_sel_init_str0);
    system_config_disp(6);

    for (i = 0; i < 14; i++) {
        x[i] = (s16)((i % 3) * 43 - 8);
        y[i] = (s16)((i / 3) * 16 - 64);
        w[i] = 40;
        h[i] = 16;
        if (i == 13) {
            w[i] = 70;
            set_kanji_string(x[i], y[i], 5, mascot_sel_init_str1, 0);
        } else if (mascot_flag[i] != 0 || mascot_flag2[i] != 0) {
            set_kanji_string(x[i], y[i], 3, date_name_set_tbl0[i], 0);
        } else {
            w[i] = 0;
        }
    }

    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, mascot_sel_init_str2, 0);
    menu_set(1, 14, x, y, w, h);

    if (cursor_work_copy_tbl0 == 0 || cursor_work_copy_tbl0 >= 14) {
        menu_csr = 0;
    } else {
        menu_csr = (s8)cursor_work_copy_tbl0;
    }

    k_sub_disp_start(0);
    sub_sub_step_up();
}
