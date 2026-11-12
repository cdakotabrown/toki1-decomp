/* Function: girl_menu_select */
/* Address: 0x80084DA4 */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void menu_check(s32 arg0, s16 arg1, s16 arg2);
extern void menu_cursor_move(s32 arg0);
extern void menu_bar_show(s32 arg0);
extern void k_reset(s32 arg0);
extern void sub_sub_step_up(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;

extern u8 screen_w[];
extern s16 girl_menu_select_tbl0[];

extern u32 serifu_no;
extern u8 cursor_cw_disp;
extern u32 menu_on;

s32 girl_menu_select(s32 arg0) {
    u32 limit;
    s16 i;
    u8* base;
    s16* table;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);

    if ((pad_trig & 0x00200020) == 0) {
        return;
    }

    limit = 12 - (u8)arg0;
    i = 0;
    if (limit == 0) {
        return;
    }

    base = screen_w;
    table = girl_menu_select_tbl0;

    do {
        if (*((u8*)((u32)base + (i << 4)) + 0xF9E) != 0) {
            if (table[i] != 0) {
                serifu_no = i;
                cursor_cw_disp &= 0xFF7F;
                menu_on = 0;
                k_reset(1);
                sub_sub_step_up();
                return 0;
            }
        }
        i++;
    } while ((u32)i < limit);
}
