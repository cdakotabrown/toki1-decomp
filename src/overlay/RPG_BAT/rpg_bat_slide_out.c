/* @SymbolStart: 0x80143C28 */
/* Address: 0x80143C30, Size: 184 bytes (46 insns) */
/* @SymbolSize: 192 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;



extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_act_sub_next(void);
extern void rpg_bat_act_next(void);

void rpg_bat_slide_out(s32 arg0, s32 arg1)
{
    switch (rpg_bat_unit2.sub) {
    case 0:
        rpg_bat_anim_set(29, arg0, 1, 1);
        rpg_bat_act_sub_next();
        break;
    case 1:
        rpg_bat_sprite_pos_set(29, -rpg_bat_unit2.wait, 0);
        rpg_bat_unit2.wait += 2;
        if (rpg_bat_unit2.wait >= 33) {
            rpg_bat_anim_set(29, arg1, 1, 1);
            rpg_bat_sprite_pos_set(29, -32, 0);
            rpg_bat_act_next();
        }
        break;
    }
}
