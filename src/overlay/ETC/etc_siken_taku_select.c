/* @SymbolSize: 188 */
/* Address: 0x80149988 */
/* Size: 188 bytes (47 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void menu_check(s32, s32, s32);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern s32 pad_trig;
extern s8 now_icon;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 etc_siken_sel;
volatile unsigned int etc_siken_taku_select(void)
{
  s16 new_var;
  new_var = cursor_cw_x;
  menu_check(0, new_var, cursor_cw_y);
  menu_bar_show(0);
  menu_cursor_move(0);
  if (!(pad_trig & 0x20))
  {
    return;
  }
  switch (now_icon)
  {
    case 0:
      etc_siken_sel = 0;
      sub_sub_step_up();
      break;

    case 1:
      etc_siken_sel = 1;
      sub_sub_step_up();
      break;

    case 2:
      etc_siken_sel = 2;
      sub_sub_step_up();
      break;

  }

}
