/* @SymbolStart: 0x8013B534 */
/* @SymbolSize: 116 */
/* rpg_bat_miss_step - RPG_BAT overlay, Address: 0x8013B53C, Size: 108 bytes (27 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit0;


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();

void rpg_bat_miss_step(void)
{
    s32 pad[2];
    (void)&pad;

    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(48, 6, 5, 1);
        rpg_bat_state_next();
        break;
    case 1:
        if (rpg_bat_rec_flag_check(48) != 0) {
            rpg_bat_step_next();
        }
        break;
    }
}
