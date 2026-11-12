/* @SymbolStart: 0x801359FC */
/* @RodataBase: 0x8015ADF8 */
/* @SymbolSize: 228 */
/* rpg_bat_hameln_anim_step - RPG_BAT overlay, Address: 0x80135A04, Size: 220 bytes (55 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit0;


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_timer_sub();
extern s32 rpg_bat_rec_sub();

void rpg_bat_hameln_anim_step(void)
{
    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_timer_sub(15);
        break;
    case 2:
        rpg_bat_state_next();
        break;
    case 3:
        rpg_bat_rec_sub(52);
        break;
    case 4:
        rpg_bat_anim_set(52, 7, 1, 1);
        rpg_bat_se_set2(1293, 1, 0);
        rpg_bat_state_next();
        break;
    case 5:
        rpg_bat_timer_sub(75);
        break;
    case 6:
        rpg_bat_step_next();
        break;
    default:
        break;
    }
}
