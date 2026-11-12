/* @SymbolStart: 0x8014F4A4 */
/* @SymbolSize: 92 */
/* rpg_bat_act_sub_gate - RPG_BAT overlay, Address: 0x8014F4AC. */
#include "types.h"

extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_act_sub_next(void);

void rpg_bat_act_sub_gate(s32 a0)
{
    if (rpg_bat_msg_ctl_mode == 0) {
        rpg_bat_act_sub_next();
    }
    if (a0 == 0) {
        return;
    }
    if ((rpg_bat_msg_ctl - 3) < a0) {
        return;
    }
    rpg_bat_act_sub_next();
}
