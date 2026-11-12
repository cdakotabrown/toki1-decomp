/* @SymbolStart: 0x80142CB8 */
/* @SymbolSize: 316 */
/* rpg_bat_step14 - RPG_BAT overlay, Address: 0x80142CC0, Size: 308 bytes (77 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();

void rpg_bat_step14(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_sprite_pos_set(0x2a, 0, 0);
        rpg_bat_anim_set(0x2a, 0xf, 5, 1);
        rpg_bat_state_next();
        break;
    case 1:
        if ((class_w[138].anim_flag & 1) == 0) {
            rpg_bat_se_set2(0x505, 0xf, 5);
            rpg_bat_anim_set(0x2a, 0x10, 5, 1);
            v0 = rpg_bat_unit0.cnt;
            rpg_bat_unit0.cnt = v0 + 1;
            rpg_bat_state_next();
        }
        break;
    case 2:
        if (rpg_bat_unit0.cnt < 50) {
            rpg_bat_se_set2(0x505, 0xf, 5);
            if ((class_w[138].anim_flag & 1) == 0) {
                rpg_bat_anim_set(0x2a, 0x10, 5, 1);
                v0 = rpg_bat_unit0.cnt;
                rpg_bat_unit0.cnt = v0 + 1;
            }
        } else {
            rpg_bat_anim_set(0x2a, 0xff, 1, 0);
            rpg_bat_step_next();
        }
        break;
    }
}
