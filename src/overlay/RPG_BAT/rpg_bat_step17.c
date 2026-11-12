/* @SymbolStart: 0x80142DF4 */
/* @SymbolSize: 372 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step17()
{
    s32 q;

    if (rpg_bat_unit0.cnt == 0) {
        rpg_bat_anim_set2(43, 21, 1, 1);
        rpg_bat_anim_set2(44, 19, 1, 1);
        rpg_bat_sprite_pos_set(44, 272, 144);
        rpg_bat_se_set2(1287, 1, 7);
    }
    rpg_bat_sprite_pos_set(43, 272, 100 - rpg_bat_unit0.cnt * 4);
    q = (292 - rpg_bat_unit0.cnt * 4) * 4096 / 292;
    class_w[140].scale_x = q;
    class_w[140].scale_y = q;
    if (100 - rpg_bat_unit0.cnt * 4 < 84 && 100 - rpg_bat_unit0.cnt * 4 >= 77) {
        rpg_bat_anim_set2(43, 4, 1, 1);
    }
    rpg_bat_unit0.cnt++;
    if (100 - rpg_bat_unit0.cnt * 4 < -192) {
        rpg_bat_anim_set(43, 6, 1, 0);
        rpg_bat_anim_set(44, 255, 1, 0);
        rpg_bat_step_next();
    }
}
