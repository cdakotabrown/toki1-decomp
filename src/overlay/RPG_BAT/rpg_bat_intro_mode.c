/* @SymbolStart: 0x80138120 */
/* @SymbolSize: 80 */
/* rpg_bat_intro_mode - RPG_BAT overlay, Address: 0x80138128 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


extern void rpg_bat_furyou_talk_step();
extern s32 rpg_bat_effect_clear();

void rpg_bat_intro_mode(void)
{
    s32 mode = rpg_bat_unit1.step;

    switch (mode) {
    case 0:
        rpg_bat_furyou_talk_step();
        break;
    case 1:
        rpg_bat_effect_clear();
        break;
    }
}
