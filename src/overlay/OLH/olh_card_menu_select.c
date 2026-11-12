/* @RodataBase: 0x80137CA4 */
/* @SymbolSize: 280 */
/* Address: see asm. Sister cluster matched via named-global pattern: olh_sys_menu_select, */
#include "types.h"

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void device_cmd_send(s32 a0);
extern void goto_sub_step(s32 a0);
extern void goto_sub_sub_step(s32 a0);

extern s32 pad_trig;
extern s8 now_icon;
extern u8 now_icon_u;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void olh_card_menu_select(void)
{
    u8 idx;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_bar_show(0);
    menu_cursor_move(0);
    if (!(pad_trig & 0x0020)) {
        return;
    }
    if (now_icon == -1) {
        return;
    }
    device_cmd_send(1281);
    idx = now_icon_u;
    switch (idx) {
    case 0: goto_sub_sub_step(16); break;
    case 1: goto_sub_sub_step(32); break;
    case 2: goto_sub_sub_step(48); break;
    case 3: goto_sub_sub_step(64); break;
    case 4: goto_sub_sub_step(80); break;
    case 5: goto_sub_sub_step(96); break;
    case 6: goto_sub_sub_step(112); break;
    case 7: goto_sub_step(0); break;
    }
}
