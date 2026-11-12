/* @SymbolSize: 172 */
/* Address: 0x8014B84C */
/* Size: 172 bytes (43 instructions) */
#include "types.h"

extern void goto_sub_step(s32);
extern void menu_check(s32, s32, s32);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern s32 pad_trig;
extern s8 now_icon;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
volatile long long etc_menu_main(void)
{
  menu_check(0, cursor_cw_x, cursor_cw_y);
  menu_bar_show(0);
  menu_cursor_move(0);
  if ((pad_trig & 0x20) == 0)
  {
    return;
  }
  switch (now_icon)
  {
    case 0:
      goto_sub_step(4);
      break;

    case 1:
      goto_sub_step(5);
      break;

    case 2:
      goto_sub_step(3);
      break;

  }

}
