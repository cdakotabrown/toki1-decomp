/* @SymbolSize: 244 */
/* rpg_bat_shake_step - RPG_BAT overlay, Address: 0x80136BD8, Size: 244 bytes (61 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_sprite_nudge();
extern s32 rpg_bat_step_next();

extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_target_x;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_shake_step(void)
{
    rpg_bat_se_set2(1284, 0, 4);
    if (rpg_bat_unit0.cnt < 20) {
        if ((rpg_bat_unit0.cnt & 1) == 0) {
            rpg_bat_sprite_nudge(0, 4);
        } else {
            rpg_bat_sprite_nudge(0, -4);
        }
    }
    if (rpg_bat_unit0.cnt == 15) {
        rpg_bat_sprite_pos_set(42, rpg_bat_target_x, rpg_bat_tgt_y + 120);
        rpg_bat_anim_set2(42, 5, 5, 1);
    }
    rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 1;
    if ((class_w[138].anim_flag & 1) == 0) {
        rpg_bat_anim_set2(42, 255, 1, 0);
        rpg_bat_step_next();
    }
}
