/* @RodataBase: 0x8015C460 */
/* @SymbolSize: 584 */
/* rpg_bat - RPG_BAT overlay, Address: 0x801439E0, Size: 584 bytes (146 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rpg_bat_wait();
extern s32 rpg_bat_player_pose_set();
extern s32 rpg_bat_judge();
extern s32 get_rpg_bat_roll2();
extern s32 rpg_bat_progress();
extern s32 rpg_bat_substep();
extern s32 rpg_bat_sub_step();
extern s32 bat_result_step();
extern s32 himo_act();
extern s32 katagiri_act();
extern s32 rpg_bat_sprite_ent_step();
extern s32 kosiki_act();
extern s32 kiyokawa_act();
extern s32 rpg_bat_sub_step2();
extern s32 asahina_act();
extern s32 rpg_bat_effect_step();
extern s32 rpg_bat_blink_seq();
extern s32 rpg_bat_yoshio_attack_step();

extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat(void)
{
    s32 v0;

    if (get_rpg_bat_wait() < rpg_bat_unit2.aux08) {
        if ((rpg_bat_unit0.flag & 0xFF) == 0 &&
            (rpg_bat_unit1.flag & 0xFF) == 0 &&
            (rpg_bat_unit3.flag & 0xFF) == 0 &&
            (rpg_bat_unit2.flag & 0xFF) == 0) {
            rpg_bat_player_pose_set();
            rpg_bat_unit2.aux08 = 0;
            v0 = rpg_bat_judge();
            rpg_bat_unit2.flag = v0;
            if ((v0 & 0xFF) == 0) {
                rpg_bat_unit2.aux08 = get_rpg_bat_roll2(100, 50);
            }
        }
    }

    if (rpg_bat_unit2.flag == 0x10001000) {
        rpg_bat_progress();
    }

    if (rpg_bat_unit2.flag == 0x10001001) {
        if (rpg_bat_unit2.step < 15) {
            rpg_bat_player_pose_set();
        }
        rpg_bat_substep();
    }

    if ((rpg_bat_unit3.flag & 0xFF) != 0) {
        return;
    }
    if ((rpg_bat_unit2.flag & 0xFF) == 0 && (rpg_bat_unit2.flag & (1 << 31)) == 0) {
        return;
    }

    switch ((u32)rpg_bat_girl) {
    case 0:
        rpg_bat_sub_step();
        return;
    case 1:
        bat_result_step();
        return;
    case 2:
        if (rpg_bat_mode == 0x80000000) {
            return;
        }
        himo_act();
        return;
    case 3:
        katagiri_act();
        return;
    case 4:
        rpg_bat_sprite_ent_step();
        return;
    case 5:
        kosiki_act();
        return;
    case 6:
        kiyokawa_act();
        return;
    case 7:
        rpg_bat_sub_step2();
        return;
    case 8:
        asahina_act();
        return;
    case 9:
        rpg_bat_effect_step();
        return;
    case 10:
        rpg_bat_blink_seq();
        return;
    case 13:
        rpg_bat_yoshio_attack_step();
        return;
    case 14:
        return;
    }
}
