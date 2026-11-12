/* @SymbolStart: 0x8015A11C */
/* @RodataBase: 0x8015E460 */
/* @SymbolSize: 200 */
/* Address: 0x8015A124, Size: 192 bytes (48 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern void rpg_bat_anim_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_act_next(void);
extern void rpg_bat_act_sub_next(void);
extern void rpg_bat_substep_wait2(s32 a0);
extern void kosiki_talk(void);

void kosiki_talk_step(void) {
    switch (rpg_bat_unit2.sub) {
    case 0:
        rpg_bat_anim_set(29, 1, 1, 1);
        rpg_bat_act_sub_next();
        break;
    case 1:
        rpg_bat_substep_wait2(20);
        break;
    case 2:
        rpg_bat_anim_set(29, 3, 1, 1);
        rpg_bat_act_sub_next();
        break;
    case 3:
        kosiki_talk();
        break;
    case 4:
        rpg_bat_anim_set(29, 1, 1, 1);
        rpg_bat_act_next();
        break;
    }
}
