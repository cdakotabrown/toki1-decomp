/* @SymbolStart: 0x80135B4C */
/* @RodataBase: 0x8015AE44 */
/* @SymbolSize: 580 */
/* rpg_bat_flash - RPG_BAT overlay, Address: 0x80135B54, Size: 572 bytes (143 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 strcpy();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_act_gate();
extern char rpg_bat_msg_buf3[];
extern char rpg_bat_msg_tbl3[];
extern char rpg_bat_flash_str0[];
extern RPG_BAT_UNIT rpg_bat_unit2;
void rpg_bat_flash(void)
{
  s32 v0;
  switch ((u32) rpg_bat_unit2.step)
  {
    case 0:
      rpg_bat_anim_set(29, 5, 1, 1);
      rpg_bat_se_set(1298, 1, 0);
      rpg_bat_act_next();
      break;

    case 1:
      rpg_bat_act_wait(20);
      break;

    case 2:
      rpg_bat_anim_set(29, 4, 1, 1);
      rpg_bat_se_set(1298, 1, 0);
      rpg_bat_act_next();
      break;

    case 3:
      rpg_bat_act_wait(20);
      break;

    case 4:
      rpg_bat_anim_set(29, 5, 1, 1);
      rpg_bat_se_set(1298, 1, 0);
      rpg_bat_act_next();
      break;

    case 5:
      rpg_bat_act_wait(20);
      break;

    case 6:
      rpg_bat_anim_set(29, 4, 1, 1);
      rpg_bat_se_set(1298, 1, 0);
      rpg_bat_act_next();
      break;

    case 7:
      rpg_bat_act_wait(20);
      break;

    case 8:
      strcpy((s32) rpg_bat_msg_buf3, (s32) rpg_bat_flash_str0);
      rpg_bat_koe_set(0x1F000087, 0x18000089, 0);
      rpg_bat_msg_set((s32) rpg_bat_msg_tbl3, 1, 60);
      rpg_bat_act_next();
      break;

    case 9:
      rpg_bat_anim_set(29, 6, 1, 1);
      rpg_bat_act_next();
      break;

    case 10:
      v0 = (rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 1);
      rpg_bat_sprite_pos_set(29, v0, 0);
      if (rpg_bat_unit2.cnt < 65)
    {
      return;
    }
      rpg_bat_act_next();
      break;

    case 11:
      rpg_bat_act_gate(0);
      break;

    case 12:
      rpg_bat_act_wait(20);
      break;

    case 13:
      rpg_bat_reset();
      rpg_bat_unit2.flag = 0x40000000;
      break;

  }

}
