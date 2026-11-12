/* Address: 0x8005EB64 */
/* Size: 200 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 a0);

extern u32 pad_trig;
extern s8 menu_csr;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void system_config_mouse_menu(void)
{
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    if (pad_trig & 0x40) {
        goto_sub_sub_step(0);
    } else if (pad_trig & 0x20) {
        switch (menu_csr) {
            case 0:
                goto_sub_sub_step(51);
                break;
            case 1:
                goto_sub_sub_step(54);
                break;
            case 2:
                goto_sub_sub_step(57);
                break;
        }
    }
}
