/* Function: system_config_menu */
/* Address: 0x8005E258 */
/* Size: 312 bytes (78 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void k_disp_inc(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);

extern u32 pad_trig;
extern s8 now_icon;
extern u8 pad_type;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void system_config_menu(void)
{
    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_bar_show(0);
    menu_cursor_move(0);
    k_disp_inc();

    if (pad_trig & 0x40) {
        goto_sub_step(0);
        goto_sub_sub_step(2);
    } else if (pad_trig & 0x20) {
        switch (now_icon) {
            case 0:
                goto_sub_sub_step(16);
                break;
            case 1:
                goto_sub_sub_step(32);
                break;
            case 2:
                if (pad_type == 0) {
                    goto_sub_sub_step(48);
                }
                break;
            case 3:
                goto_sub_sub_step(64);
                break;
            case 4:
                goto_sub_sub_step(80);
                break;
            case 5:
                goto_sub_sub_step(96);
                break;
            case 6:
                goto_sub_sub_step(112);
                break;
            case 7:
                goto_sub_sub_step(255);
                break;
        }
    }
}
