/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_NO_FALLTHRU_FILL=0 */
/* @SymbolSize: 212 */
/* rpg_bat_ready_set - RPG_BAT overlay, Address: 0x8013C1CC, Size: 212 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_enemy_hp[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_ready_set(void)
{
    s32 flags;

    if ((rpg_bat_unit0.flag & 0xFF) != 0) {
        return;
    }
    flags = rpg_bat_unit1.flag;
    if ((flags & 0xFF) != 0) {
        return;
    }
    if ((rpg_bat_unit2.flag & 0xFF) != 0) {
        return;
    }

    if ((flags << 11) < 0) {
        if (rpg_bat_enemy_hp[0] == 0 || rpg_bat_enemy_hp[1] == 0 || rpg_bat_enemy_hp[2] == 0) {
            rpg_bat_unit3.flag = 1;
        }
        return;
    }
    if ((flags & 0x00600000) != 0) {
        if (rpg_bat_unit1.hp == 0) {
            rpg_bat_unit3.flag = 1;
        }
        return;
    }
    if ((flags << 4) < 0) {
        if (rpg_bat_unit1.hp == 0) {
            rpg_bat_unit3.flag = 1;
        }
    }
}
