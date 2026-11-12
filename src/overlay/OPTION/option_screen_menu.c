/* @SymbolStart: 0x8013992C */
/* @SymbolSize: 180 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern volatile u32 step_counter;
extern s16 cursor_cw_y;
extern s16 cursor_cw_x;
extern LIBGS_BG Bg[];
extern s32 rsin(s32);
extern void menu_check();
extern s32 menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void option_boxes_show();
extern s32 option_input_step();
extern s32 k_disp_inc();
extern void option_sprite_move();
extern s32 option_reverb_toggle();

void option_screen_menu(void)
{
    s32 v0;
    s16 a1, a2;

    v0 = rsin(step_counter << 5);
    Bg[0].scalex = v0;

    v0 = rsin(step_counter << 5);
    Bg[0].scaley = v0;

    a1 = cursor_cw_x;
    a2 = cursor_cw_y;
    Bg[0].rotate = step_counter << 12;

    menu_check(0, a1, a2);
    menu_bar_show(0);
    menu_cursor_move(0);
    option_boxes_show();
    option_input_step();
    k_disp_inc();
    option_sprite_move();
    option_reverb_toggle();
}
