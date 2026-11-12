/* @SymbolStart: 0x8014F6D4 */
/* @SymbolSize: 92 */
/* rpg_bat_seq_gate - RPG_BAT overlay, Address: 0x8014F6DC. */
#include "types.h"

extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_seq_next(void);

void rpg_bat_seq_gate(s32 a0)
{
    if (rpg_bat_msg_ctl_mode == 0) {
        rpg_bat_seq_next();
    }
    if (a0 == 0) {
        return;
    }
    if ((rpg_bat_msg_ctl - 3) < a0) {
        return;
    }
    rpg_bat_seq_next();
}
