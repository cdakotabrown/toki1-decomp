/* @SymbolStart: 0x80132EE0 */
/* @SymbolSize: 252 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();

void rpg_bat_pair_slide_in(void)
{
    if (rpg_bat_unit0.cnt == 0) {
        rpg_bat_sprite_pos_set(52, 368 - rpg_bat_unit0.cnt, 88);
        rpg_bat_sprite_pos_set(51, 368 - rpg_bat_unit0.cnt, 88);
        rpg_bat_anim_set2(52, 3, 1, 1);
        rpg_bat_anim_set2(51, 4, 1, 1);
    }

    rpg_bat_sprite_pos_set(52, 368 - rpg_bat_unit0.cnt, 88);
    rpg_bat_sprite_pos_set(51, 368 - rpg_bat_unit0.cnt, 88);

    rpg_bat_unit0.cnt += 6;
    if (rpg_bat_unit0.cnt < 97) {
        return;
    }

    class_w[148].rate = 1;
    rpg_bat_anim_set2(51, 4, 1, 0);
    rpg_bat_step_next();
}
