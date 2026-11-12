/* @SymbolStart: 0x8013D350 */
/* @SymbolSize: 148 */
/* rpg_bat_rows_disp - RPG_BAT overlay, Address: 0x8013D358, Size: 140 bytes (35 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_slot_adjust();

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg[];
extern RPG_BAT_UNIT rpg_bat_unit1;


void rpg_bat_rows_disp(void)
{
    if ((rpg_bat_unit1.flag << 11) < 0) {
        rpg_bat_slot_adjust(0, 0, rpg_bat_dmg[0], 1);
        rpg_bat_slot_adjust(1, 1, rpg_bat_dmg[1], 1);
        rpg_bat_slot_adjust(2, 2, rpg_bat_dmg[2], 1);
    } else {
        rpg_bat_slot_adjust(0, rpg_bat_target_no, rpg_bat_dmg[0], 1);
    }
}
