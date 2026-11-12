/* @SymbolStart: 0x8014EE98 */
/* @SymbolSize: 92 */
/* rpg_bat_state_gate - RPG_BAT overlay, Address: 0x8014EEA0. */
#include "types.h"

extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_state_next(void);

void rpg_bat_state_gate(s32 a0)
{
    if (rpg_bat_msg_ctl_mode == 0) {
        rpg_bat_state_next();
    }
    if (a0 == 0) {
        return;
    }
    if ((rpg_bat_msg_ctl - 3) < a0) {
        return;
    }
    rpg_bat_state_next();
}
