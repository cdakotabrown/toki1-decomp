/* @SymbolSize: 160 */
/* rpg_bat_banner0f - RPG_BAT overlay, Address: 0x8013ACA4, Size: 160 bytes (40 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern void rpg_bat_se_set2();
extern s32 rpg_bat_step_next();

void rpg_bat_banner0f(void)
{
    rpg_bat_se_set2(1287, 0, 2);
    rpg_bat_anim_set(46, 10, 1, 1);
    rpg_bat_sprite_pos_set(46, (-rpg_bat_unit0.cnt) << 3, 0);

    rpg_bat_unit0.cnt++;
    if (rpg_bat_unit0.cnt >= 39) {
        class_w[157].prio = 5;
        rpg_bat_anim_set(46, 10, 1, 0);
        rpg_bat_step_next();
    }
}
