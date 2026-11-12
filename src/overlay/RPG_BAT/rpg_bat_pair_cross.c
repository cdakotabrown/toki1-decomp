/* @SymbolStart: 0x80132DE8 */
/* @SymbolSize: 248 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();

void rpg_bat_pair_cross(void)
{
    if (rpg_bat_unit0.cnt == 0) {
        class_w[147].prio = 4;
        class_w[148].prio = 4;
        rpg_bat_sprite_pos_set(52, rpg_bat_unit0.cnt - 16, 138);
        rpg_bat_sprite_pos_set(51, rpg_bat_unit0.cnt - 16, 138);
        rpg_bat_anim_set2(52, 11, 1, 1);
        rpg_bat_anim_set2(51, 12, 1, 1);
    }

    rpg_bat_sprite_pos_set(52, rpg_bat_unit0.cnt - 16, 138);
    rpg_bat_sprite_pos_set(51, rpg_bat_unit0.cnt - 16, 138);
    rpg_bat_unit0.cnt += 6;
    if (rpg_bat_unit0.cnt >= 390) {
        class_w[147].prio = 5;
        class_w[148].prio = 5;
        rpg_bat_step_next();
    }
}
