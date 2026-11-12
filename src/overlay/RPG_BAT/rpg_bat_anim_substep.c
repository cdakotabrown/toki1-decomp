/* @SymbolStart: 0x80140A58 */
/* @RodataBase: 0x8015C128 */
/* @SymbolSize: 624 */
/* rpg_bat_anim_substep - RPG_BAT overlay, Address: 0x80140A60, Size: 616 bytes (154 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_substep_next();
extern s32 rpg_bat_flag_substep();

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_anim_substep(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit1.sub) {
    case 0:
        rpg_bat_sprite_pos_set(58, -8, 0);
        rpg_bat_anim_set(58, 15, 5, 1);
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_substep_next();
        break;
    case 1:
        if ((class_w[154].anim_flag & 1) != 0)
            return;
        rpg_bat_anim_set(58, 16, 5, 1);
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_substep_next();
        break;
    case 2:
        if ((class_w[154].anim_flag & 1) != 0)
            return;
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_anim_set(58, 16, 5, 1);
        v0 = rpg_bat_unit1.wait + 1;
        rpg_bat_unit1.wait = v0;
        if (v0 < 4)
            return;
        rpg_bat_anim_set(58, 16, 1, 1);
        rpg_bat_anim_set2(59, 18, 5, 1);
        rpg_bat_substep_next();
        break;
    case 3:
        rpg_bat_flag_substep(59);
        break;
    case 4:
        rpg_bat_anim_set(58, 17, 5, 1);
        if ((rpg_bat_unit0.flag & 0x100) == 0)
            rpg_bat_anim_set(48, 10, 1, 1);
        rpg_bat_se_set(1291, 1, 0);
        rpg_bat_substep_next();
        break;
    case 5:
        if ((class_w[154].anim_flag & 1) != 0)
            return;
        rpg_bat_se_set(1291, 1, 0);
        rpg_bat_anim_set(58, 17, 5, 1);
        v0 = rpg_bat_unit1.wait + 1;
        rpg_bat_unit1.wait = v0;
        if (v0 < 9)
            return;
        rpg_bat_anim_set(58, 255, 1, 0);
        rpg_bat_anim_set2(59, 19, 5, 1);
        rpg_bat_substep_next();
        break;
    case 6:
        rpg_bat_flag_substep(59);
        break;
    case 7:
        rpg_bat_anim_set2(59, 2, 1, 1);
        rpg_bat_seq_step_next();
        break;
    }
}
