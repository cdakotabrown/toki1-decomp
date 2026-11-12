/* @SymbolSize: 168 */
/* etc_soudan_girl_select -- ETC overlay, Address: 0x8014BA0C, Size: 168 bytes (42 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void k_sub_reset(void);
extern void menu_check(s32 a, s32 b, s32 c);
extern void menu_cursor_move(s32 a);
extern void menu_bar_show(s32 a);
extern void gnsx(void *a);
extern void sndisp(s32 a, s32 b, s32 c);

extern s32 pad_trig;
extern s8 now_icon;
extern char p_sei[];
extern s32 etc_soudan_msg_tbl[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void etc_soudan_girl_select(void)
{
    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_bar_show(0);
    menu_cursor_move(0);
    if ((pad_trig & 0x20) == 0) return;
    if (now_icon == -1) return;
    k_sub_reset();
    gnsx(p_sei);
    sndisp(etc_soudan_msg_tbl[now_icon], 0, 15);
    sub_sub_step_up();
}
