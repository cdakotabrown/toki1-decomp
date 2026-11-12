/* @SymbolStart: 0x8013C2A0 */
/* @SymbolSize: 140 */
/* rpg_bat_win_seq_sel - RPG_BAT overlay, Address: 0x8013C2A8, Size: 132 bytes (33 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 rpg_bat_phase();
extern s32 rpg_bat_box_step();
extern s32 rpg_bat_win_seq();

extern s32 rpg_bat_mode;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_win_seq_sel(void)
{
    s32 v0;

    if (rpg_bat_unit3.flag == 1) {
        v0 = rpg_bat_mode;
        switch (v0) {
        case 0x100000:
            if ((rpg_bat_unit1.flag & 0x100000) != 0) {
                rpg_bat_phase();
            } else {
                rpg_bat_win_seq();
            }
            break;
        case 0x8000000:
            rpg_bat_box_step();
            break;
        }
    }
}
