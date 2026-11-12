/* Address: 0x8005EC78 */
/* Size: 316 bytes (79 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 mouse_btn_mode;
extern u8 menu_csr;
extern char mouse_btn_title_str[];
extern char mouse_btn_right_str[];
extern char mouse_btn_left_str[];
extern char mouse_btn_help_str[];

extern void text_sys_init(void);
extern void basyo_init(char *);
extern void system_config_disp(s32);
extern void set_kanji_string(s32, s32, s32, char *, s32);
extern void k_sub_reset_point_set(void);
extern s32 menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void k_sub_disp_start(s32);
extern s32 sub_sub_step_up(void);

void system_config_mouse_botton_mode_init(void)
{
    s32 i;
    s32 j;
    s16 xs[4];
    s16 ys[4];
    s16 ws[4];
    s16 hs[4];

    text_sys_init();
    basyo_init(mouse_btn_title_str);
    system_config_disp(2);
    for (i = 0; i < 2; i++) {
        xs[i] = -4;
        ys[i] = -64 + i * 16;
        ws[i] = 108;
        hs[i] = 16;
    }
    set_kanji_string(xs[0], ys[0], 3, mouse_btn_right_str, 0);
    set_kanji_string(xs[1], ys[1], 3, mouse_btn_left_str, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, mouse_btn_help_str, 0);
    menu_set(1, 2, xs, ys, ws, hs);
    if (mouse_btn_mode == 0 || mouse_btn_mode != 1) {
        menu_csr = 1;
    } else {
        menu_csr = 0;
    }
    k_sub_disp_start(0);
    sub_sub_step_up();
}
