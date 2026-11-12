/* @SymbolStart: 0x8013D59C */
/* @RodataBase: 0x8015BC90 */
/* @SymbolSize: 476 */
/* rpg_bat_bukatu_exit - RPG_BAT overlay, Address: 0x8013D5A4, Size: 468 bytes (117 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_timer_sub();
extern s32 rpg_bat_rec_sub();

extern volatile u8 cw148_rate;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_bukatu_exit(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        if ((rpg_bat_unit1.flag & 0x600000) != 0) {
            rpg_bat_anim_set(52, 1, 33, 1);
            cw148_rate = 130;
            class_w[148].frame = 15;
        } else {
            rpg_bat_anim_set(52, 0, 33, 1);
            cw148_rate = 130;
            class_w[148].frame = 15;
        }
        rpg_bat_sprite_pos_set(52, 0, 0);
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_rec_sub(52);
        break;
    case 2:
        cw148_rate = 1;
        rpg_bat_anim_set(52, 0, 1, 0);
        rpg_bat_anim_set(48, 0, 1, 1);
        rpg_bat_state_next();
        break;
    case 3:
        rpg_bat_timer_sub(20);
        break;
    case 4:
        rpg_bat_anim_set(48, 2, 1, 1);
        rpg_bat_state_next();
        break;
    case 5:
        rpg_bat_sprite_pos_set(48, rpg_bat_unit0.cnt - 32, 0);
        v0 = rpg_bat_unit0.cnt + 2;
        rpg_bat_unit0.cnt = v0;
        if (v0 >= 33) {
            rpg_bat_sprite_pos_set(48, 0, 0);
            rpg_bat_anim_set(48, 0, 1, 1);
            rpg_bat_state_next();
        }
        break;
    case 6:
        rpg_bat_timer_sub(20);
        break;
    case 7:
        rpg_bat_step_next();
        break;
    }
}
