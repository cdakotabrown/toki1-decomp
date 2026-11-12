/* Likely Symbol: menu3_select (from o.bin, offset +80 bytes from 0x80083230) */
/* Address: 0x80083280 */
/* Debug Symbol: menu3_select (REVERTED - offset 80, uncertain) (from o.bin, high match) */
/* Size: 252 bytes (63 instructions) */
/* Compiler: IDO 5.2 */
/* Region: 0x80041000-0x80085FFF (uses or encoding for move) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern void menu_check(s32 arg0, s16 arg1, s16 arg2);
extern void menu_cursor_move(s32 arg0);
extern void menu_bar_show(s32 arg0);
extern s32 check_end_k(void);
extern void k_reset(s32 arg0);
extern void sub_sub_step_up(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern u8 menu1_hit0;
extern u8 menu1_hit1;
extern u8 menu1_hit2;
extern u8 junban;
extern u8 menu3_select_tbl0;
extern u8 menu3_select_tbl1;
extern u32 serifu_no;
extern u8 cursor_cw_disp;
extern u32 menu_on;

s32 menu3_select(void) {
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);

    if ((pad_trig & 0x200020) != 0) {
        if (check_end_k() != 0) {
            if (menu1_hit0 != 0) {
                serifu_no = junban;
            } else if (menu1_hit1 != 0) {
                serifu_no = menu3_select_tbl0;
            } else if (menu1_hit2 != 0) {
                serifu_no = menu3_select_tbl1;
            } else {
                return 0;
            }
            cursor_cw_disp &= ~0x80;
            menu_on = 0;
            k_reset(1);
            sub_sub_step_up();
        }
    }
}
