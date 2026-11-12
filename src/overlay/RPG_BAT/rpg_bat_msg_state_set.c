/* @SymbolSize: 220 */
/* @RodataBase: 0x8015CE30 */
#include "types.h"

extern s32 rpg_bat_msg_ctl_mode;

void rpg_bat_msg_state_set(s32 cmd)
{
    s32 def;

    def = 1;
    switch ((u32)(cmd - 1)) {
    case 0:
        rpg_bat_msg_ctl_mode = 128;
        return;
    case 1:
        rpg_bat_msg_ctl_mode = 129;
        return;
    case 2:
        rpg_bat_msg_ctl_mode = 130;
        return;
    case 3:
        rpg_bat_msg_ctl_mode = 131;
        return;
    case 4:
        rpg_bat_msg_ctl_mode = 132;
        return;
    case 5:
        rpg_bat_msg_ctl_mode = 133;
        return;
    case 6:
        rpg_bat_msg_ctl_mode = 134;
        return;
    case 7:
        rpg_bat_msg_ctl_mode = 135;
        return;
    case 8:
        rpg_bat_msg_ctl_mode = 136;
        return;
    case 9:
        rpg_bat_msg_ctl_mode = 137;
        return;
    }
    rpg_bat_msg_ctl_mode = def;
}
