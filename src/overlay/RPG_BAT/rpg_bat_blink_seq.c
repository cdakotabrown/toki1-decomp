/* @SymbolStart: 0x80136374 */
/* @RodataBase: 0x8015AF78 */
/* @SymbolSize: 444 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_talk();
extern s32 rpg_bat_foe_cry_talk();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_flag_act();
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit2;
void rpg_bat_blink_seq(void)
{
  s32 v0;
  switch ((u32) rpg_bat_unit2.step)
  {
    case 0:
      v0 = (rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 1);
      if ((v0 % 20) < 10)
    {
      rpg_bat_anim_set(29, 5, 1, 1);
    }
    else
    {
      rpg_bat_anim_set(29, 0, 1, 1);
    }
      if (rpg_bat_unit2.cnt < 59)
    {
      return;
    }
      rpg_bat_act_next();
      break;

    case 1:
      rpg_bat_anim_set(29, 6, 1, 1);
      rpg_bat_act_next();
      break;

    case 2:
      rpg_bat_talk();
      break;

    case 3:
      rpg_bat_flag_act(30);
      break;

    case 4:
      rpg_bat_foe_cry_talk();
      break;

    case 5:
      rpg_bat_anim_set(30, 7, 33, 1);
      class_w[126].frame = 7;
      class_w[126].rate = 131;
      rpg_bat_anim_set(29, 0, 1, 1);
      rpg_bat_act_next();
      break;

    case 6:
      if (rpg_bat_rec_flag_check(30) != 0)
    {
      rpg_bat_anim_set(30, 255, 1, 0);
      class_w[126].rate = 1;
      rpg_bat_act_next();
    }
      break;

    case 7:
      rpg_bat_act_wait(20);
      break;

    case 8:
      rpg_bat_reset();
      break;

  }

}
