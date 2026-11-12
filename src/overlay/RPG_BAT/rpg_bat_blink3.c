/* @SymbolStart: 0x80143F68 */
/* @SymbolSize: 156 */
/* rpg_bat_blink3 - RPG_BAT overlay, Address: 0x80143F70, Size: 148 bytes (37 insns) */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit2;

extern s32 rpg_bat_act_next(void);

void rpg_bat_blink3(void)
{
    if ((rpg_bat_unit2.cnt & 1) == 0) {
        class_w[144].disp_flag |= 0x80;
        class_w[125].disp_flag |= 0x80;
    } else {
        class_w[144].disp_flag &= 0xFF7F;
        class_w[125].disp_flag &= 0xFF7F;
    }
    rpg_bat_unit2.cnt++;
    rpg_bat_unit2.cnt = rpg_bat_unit2.cnt;
    if (rpg_bat_unit2.cnt >= 61) {
        rpg_bat_act_next();
    }
}
