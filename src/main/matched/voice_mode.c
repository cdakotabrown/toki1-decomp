/* Address: 0x800601A0 */
/* Size: 412 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s16 x, s16 y, s16 a2, void *a3, s32 stack16);
extern void sub_sub_step_up(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8 menu_csr;
extern s8 voice_level;
extern u8 voice_mode_str1[];
extern u8 voice_mode_str0[];
extern u8 voice_mode_str3[];
extern u8 voice_mode_str2[];
extern u8 voice_mode_str4[];
extern u8 voice_mode_str5[];

void voice_mode(void)
{
    s32 v0;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    v0 = pad_trig;
    if (v0 & 0x40) {
        goto_sub_sub_step(0);
        return;
    }
    if (!(v0 & 0x20)) {
        return;
    }

    switch (menu_csr) {
        case 0:
            k_sub_reset();
            set_kanji_string(-128, 50, 0, voice_mode_str0, 0);
            set_kanji_string(-128, 66, 0, voice_mode_str1, 0);
            voice_level = 1;
            sub_sub_step_up();
            break;
        case 1:
            k_sub_reset();
            set_kanji_string(-128, 50, 0, voice_mode_str2, 0);
            set_kanji_string(-128, 66, 0, voice_mode_str3, 0);
            voice_level = 0;
            sub_sub_step_up();
            break;
        case 2:
            k_sub_reset();
            set_kanji_string(-128, 50, 0, voice_mode_str4, 0);
            set_kanji_string(-128, 66, 0, voice_mode_str5, 0);
            voice_level = 2;
            sub_sub_step_up();
            break;
    }
}
