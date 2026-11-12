/* @SymbolSize: 124 */
/* /* rpg_bat_msg_pick - RPG_BAT overlay, Address: 0x80151F38, Size: 124 bytes (31 insns) */
#include "types.h"

extern s32 get_rand_mod();
extern void rpg_bat_se_set();

void rpg_bat_msg_pick(void)
{
    s32 v0;
    s32 pad[2];
    (void)pad;

    v0 = get_rand_mod(3);
    switch (v0) {
    case 0:
        rpg_bat_se_set(1294, 1, 0);
        break;
    case 1:
        rpg_bat_se_set(1295, 1, 0);
        break;
    case 2:
        rpg_bat_se_set(1296, 1, 0);
        break;
    }
}
