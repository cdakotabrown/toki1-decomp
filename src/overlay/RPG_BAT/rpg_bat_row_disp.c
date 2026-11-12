/* rpg_bat_row_disp - RPG_BAT overlay, Address: 0x8013D3E4, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;

extern void rpg_bat_slot_adjust();

void rpg_bat_row_disp(void)
{
    rpg_bat_slot_adjust(0, rpg_bat_target_no, rpg_bat_dmg, 1);
}
