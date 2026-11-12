/* Function: config_click_mode */
/* Likely Symbol: config_click_mode */
/* Address: 0x8005F040 */
/* Size: 260 bytes (65 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8 menu_csr;
extern u8 click_mode;
extern u8 config_click_mode_str4;
extern u8 config_click_mode_str5;

extern void menu_check(s32 a0, s32 a1, s32 a2);
extern void menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 a0);
extern void k_sub_reset(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, void *a3, s32 stack0);
extern void sub_sub_step_up(void);

void config_click_mode(void)
{
    s32 val;
    s32 mode;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    val = pad_trig;

    if ((val & 0x40) != 0) {
        goto_sub_sub_step(0);
        return;
    }

    if ((val & 0x20) == 0) {
        return;
    }

    mode = menu_csr;
    switch (mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &config_click_mode_str4, 0);
        click_mode = 0;
        sub_sub_step_up();
        return;
    case 1:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, &config_click_mode_str5, 0);
        click_mode = 1;
        sub_sub_step_up();
        break;
    }
}
