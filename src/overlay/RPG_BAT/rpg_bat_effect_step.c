/* @SymbolStart: 0x80135AE0 */
/* @SymbolSize: 108 */
/* rpg_bat_effect_step - RPG_BAT overlay, Address: 0x80135AE8, Size: 100 bytes (25 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern void rpg_bat_flash();
extern void rpg_bat_slide_in_step();
extern void rpg_bat_slide_seq();

void rpg_bat_effect_step(void)
{
    switch (rpg_bat_unit2.flag) {
    case 1:
        rpg_bat_flash();
        break;
    case 2:
        rpg_bat_slide_in_step();
        break;
    case 3:
        rpg_bat_slide_seq();
        break;
    }
}
