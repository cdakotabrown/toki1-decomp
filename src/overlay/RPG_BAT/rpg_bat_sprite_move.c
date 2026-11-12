/* @SymbolStart: 0x80143DA8 */
/* @SymbolSize: 292 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_move_dx;
extern s32 rpg_bat_move_dy;
extern RPG_BAT_UNIT rpg_bat_unit2;

extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_act_next(void);

void rpg_bat_sprite_move(s32 id, s32 x0, s32 y0, s32 x1, s32 y1, s32 step)
{
    rpg_bat_move_dx = x1 - x0;
    rpg_bat_move_dy = y1 - y0;
    rpg_bat_sprite_pos_set(id, rpg_bat_move_dx * rpg_bat_unit2.wait / 1000 + x0,
                      rpg_bat_move_dy * rpg_bat_unit2.wait / 1000 + y0);
    rpg_bat_unit2.wait += step;
    if (rpg_bat_unit2.wait > 1000) {
        rpg_bat_sprite_pos_set(id, x1, y1);
        rpg_bat_act_next();
    }
}
