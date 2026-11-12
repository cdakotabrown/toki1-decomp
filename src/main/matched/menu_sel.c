/* Function: menu_sel */
/* Address: 0x80083470 */
/* Size: 208 bytes (52 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern u8 menu1_hit0;
extern u8 menu1_hit1;
extern u8 cursor_cw_disp;
extern s32 serifu_no;
extern s32 menu_on;

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_cursor_move(s32 a0);
extern void menu_bar_show(s32 a0);
extern s32 check_end_k(void);
extern void k_reset(s32 a0);
extern void sub_sub_step_up(void);

s32 menu_sel(void) {
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);

    if (pad_trig & 0x200020) {
        if (check_end_k() != 0) {
            if (menu1_hit0 != 0) {
                serifu_no = 0;
            } else if (menu1_hit1 != 0) {
                serifu_no = 1;
            } else {
                return 0;
            }
            cursor_cw_disp &= 0xFF7F;
            menu_on = 0;
            k_reset(1);
            sub_sub_step_up();
        }
    }
}
