/* @SymbolStart: 0x8014A16C */
/* @RodataBase: 0x8015CCF0 */
/* @SymbolSize: 624 */
/* rpg_bat_bukatu11_lineup_step - RPG_BAT overlay, Address: 0x8014A174, Size: 616 bytes (154 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit0;


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_timer_sub();


void rpg_bat_bukatu11_lineup_step(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(42, 255, 1, 0);
        rpg_bat_anim_set(43, 255, 1, 0);
        rpg_bat_anim_set(44, 255, 1, 0);
        rpg_bat_anim_set(45, 255, 1, 0);
        rpg_bat_anim_set(46, 255, 1, 0);
        rpg_bat_anim_set(47, 255, 1, 0);
        rpg_bat_anim_set(52, 5, 5, 1);
        rpg_bat_sprite_pos_set(41, 251, 61);
        rpg_bat_sprite_pos_set(40, 251, 61);
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_timer_sub(3);
        break;
    case 2:
        rpg_bat_sprite_pos_set(41, 258, 69);
        rpg_bat_sprite_pos_set(40, 258, 69);
        rpg_bat_state_next();
        break;
    case 3:
        rpg_bat_timer_sub(3);
        break;
    case 4:
        rpg_bat_sprite_pos_set(41, 263, 76);
        rpg_bat_sprite_pos_set(40, 263, 76);
        rpg_bat_state_next();
        break;
    case 5:
        rpg_bat_timer_sub(5);
        break;
    case 6:
        rpg_bat_sprite_pos_set(41, 265, 83);
        rpg_bat_sprite_pos_set(40, 265, 83);
        rpg_bat_state_next();
        break;
    case 7:
        rpg_bat_timer_sub(5);
        break;
    case 8:
        rpg_bat_sprite_pos_set(41, 264, 88);
        rpg_bat_sprite_pos_set(40, 264, 88);
        rpg_bat_state_next();
        break;
    case 9:
        rpg_bat_timer_sub(7);
        break;
    case 10:
        rpg_bat_sprite_pos_set(41, 264 - rpg_bat_unit0.wait, 88);
        rpg_bat_sprite_pos_set(40, 264 - rpg_bat_unit0.wait, 88);
        v0 = rpg_bat_unit0.wait + 2;
        rpg_bat_unit0.wait = v0;
        if (v0 < 40) {
            break;
        }
        rpg_bat_step_next();
        break;
    }
}
