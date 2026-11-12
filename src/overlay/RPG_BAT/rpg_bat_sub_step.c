/* @SymbolStart: 0x801420EC */
/* @RodataBase: 0x8015C274 */
/* @SymbolSize: 236 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern void rpg_bat_anim_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_msg_seq(void);
extern void rpg_bat_slide_out(s32 a0, s32 a1);
extern void rpg_bat_slide_in(s32 a0, s32 a1);
extern void rpg_bat_act_next(void);
extern void rpg_bat_reset(void);
extern void rpg_bat_act_wait(s32 a0);

void rpg_bat_sub_step(void) {
    switch (rpg_bat_unit2.step) {
    case 0:
        rpg_bat_slide_out(2, 0);
        break;
    case 1:
        rpg_bat_act_wait(20);
        break;
    case 2:
        rpg_bat_anim_set(29, 5, 1, 1);
        rpg_bat_act_next();
        break;
    case 3:
        rpg_bat_msg_seq();
        break;
    case 4:
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_act_wait(40);
        break;
    case 5:
        rpg_bat_slide_in(3, 0);
        break;
    case 6:
        rpg_bat_act_wait(20);
        break;
    case 7:
        rpg_bat_reset();
        break;
    }
}
