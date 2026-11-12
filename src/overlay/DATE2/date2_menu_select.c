/* @PostProc: swap_branch_operands --offsets 0x5c */
/* @SymbolSize: 380 */
/* Address: 0x801388C0, Size: 380 bytes (95 insns) */
#include "types.h"

void menu_check(s32 a0, s32 a1, s32 a2);
void menu_cursor_move(s32 a0);
void menu_bar_show(s32 a0);
void device_cmd_send(s32 a0);
void k_reset(s32 a0);

extern s16 cursor_cw_y;
extern s16 cursor_cw_x;
extern volatile u8  date2_menu_select_tbl1;
extern u16 date2_menu_prev;
extern u8  date2_song_no;
extern u8  step_mode;
extern u8  date2_menu_select_tbl0;
extern u32 pad_trig;
extern u8  screen_w[];
extern s16 date2_menu_wid_tbl[];
extern u32 serifu_no;
extern u8  date2_sched_no;
extern u8  cursor_cw_disp;
extern u32 menu_on;
extern u32 step_counter;

s32 date2_menu_select(void)
{
    s32 v0;
    u16 t6;
    u32 t8;
    s16 i;

    menu_check(0, (s32)cursor_cw_x, (s32)cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);

    v0 = (s32)date2_menu_select_tbl1;
    if (v0 != 0) {
        if (v0 != (s32)date2_menu_prev) {
            device_cmd_send(1537);
            date2_song_no = date2_song_no + 1;
            if (date2_song_no >= 3U) {
                v0 = 1;
                date2_song_no = (u8)v0;
            }
            step_mode = 0;
            v0 = 0;
            date2_menu_select_tbl0 = 0;
        }
    }
    date2_menu_prev = (u16)v0;
    date2_menu_select_tbl1 = (u8)v0;
    t8 = pad_trig;
    if ((t8 & 0x00600060U) != 0) {
        for (i = 0; i < 4; i++) {
            if (screen_w[i * 16 + 3990] == 0) {
                continue;
            }
            if (date2_menu_wid_tbl[i] == 0) {
                continue;
            }
            serifu_no = (u32)i;
            date2_sched_no = (u8)i;
            menu_on = 0;
            cursor_cw_disp = (u8)(cursor_cw_disp & 0xFF7F);
            k_reset(1);
            step_counter = step_counter + 1;
            step_mode = 0;
            return 0;
        }
    }
}
