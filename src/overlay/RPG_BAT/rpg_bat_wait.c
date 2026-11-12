/* @SymbolStart: 0x80148654 */
/* @SymbolSize: 192 */
/* rpg_bat_wait - RPG_BAT overlay, Address: 0x8014865C, Size: 184 bytes (46 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_wait(void)
{
    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(52, 4, 5, 1);
        rpg_bat_se_set2(1286, 1, 0);
        rpg_bat_state_next();
        break;
    case 1:
        if ((class_w[148].anim_flag & 1) == 0) {
            rpg_bat_anim_set(52, 4, 5, 1);
            rpg_bat_unit0.wait = rpg_bat_unit0.wait + 1;
        }
        if (rpg_bat_unit0.wait >= 14) {
            rpg_bat_step_next();
        }
        break;
    }
}
