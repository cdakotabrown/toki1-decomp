/* @SymbolStart: 0x80143CE8 */
/* @SymbolSize: 192 */
/* rpg_bat_slide_in - RPG_BAT overlay, Address: 0x80143CF0, Size: 184 bytes (46 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_act_sub_next();
void rpg_bat_slide_in(s32 a0, s32 a1)
{
  switch (rpg_bat_unit2.sub)
  {
    case 0:
      rpg_bat_anim_set(29, a0, 1, 1);
      rpg_bat_act_sub_next();
      break;

    case 1:
      rpg_bat_sprite_pos_set(29, rpg_bat_unit2.wait - 32, 0);
      rpg_bat_unit2.wait = rpg_bat_unit2.wait + 2;
      if (rpg_bat_unit2.wait >= 33)
    {
      rpg_bat_anim_set(29, a1, 1, 1);
      rpg_bat_sprite_pos_set(29, 0, 0);
      rpg_bat_act_next();
    }
      break;

  }

}
