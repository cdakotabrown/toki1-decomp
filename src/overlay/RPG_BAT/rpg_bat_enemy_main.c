/* @RodataBase: 0x8015BC30 */
/* rpg_bat_enemy_main - RPG_BAT overlay, Address: 0x8013C890 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_act_delay;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

extern s32 get_rpg_bat_mode_limit(void);
extern s32 get_rpg_bat_roll2(s32, s32);
extern s32 get_rpg_bat_act_sel(void);
extern void monster_attack(void);
extern void mongoose_seq(void);
extern void komusou_seq(void);
extern void rpg_bat_intro(void);
extern void rpg_bat_intro_mode(void);
extern void furyou_punch_hit_seq(void);
extern void furyou_punch_miss_seq(void);
extern void furyou_punch_ally_seq(void);
extern void furyou_punch_stun_seq(void);
extern void rpg_bat_intro_step(void);
extern void rpg_bat_furyou_end_step(void);
extern void banchou_talk_seq(void);
extern void banchou_attack(void);
extern void banchou_power_seq(void);
extern void rpg_bat_shade_step(void);
extern void rpg_bat_lose_seq(void);
extern void robo_attack(void);
extern void robo_power_seq(void);
extern void utyuuzin_attack(void);
extern void utyuuzin_attack2(void);
extern void rpg_bat_escape_seq(void);
extern void utyuuzin_talk(void);

s32 rpg_bat_enemy_main()
{
    if (get_rpg_bat_mode_limit() < rpg_bat_unit1.aux08 &&
        (rpg_bat_unit0.flag & 0xFF) == 0 &&
        (rpg_bat_unit1.flag & 0xFF) == 0 &&
        (rpg_bat_unit3.flag & 0xFF) == 0 &&
        (rpg_bat_unit2.flag & 0xFF) == 0) {
        rpg_bat_act_delay = get_rpg_bat_roll2(20, 40);
        rpg_bat_unit1.aux08 = 0;
        rpg_bat_unit1.flag |= get_rpg_bat_act_sel();
        if ((rpg_bat_unit1.flag & 0xFF) == 0) {
            rpg_bat_unit1.aux08 = get_rpg_bat_roll2(60, 90);
        }
    }

    if ((rpg_bat_unit1.flag & 0xFF) == 0) {
        return;
    }

    switch ((s32)(rpg_bat_unit1.flag & 0xFFFF0000)) {
    case 0x00010000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 1: monster_attack(); break;
        case 2: mongoose_seq(); break;
        }
        break;
    case 0x00020000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 1: monster_attack(); break;
        case 2: komusou_seq(); break;
        }
        break;
    case 0x00040000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 1: monster_attack(); break;
        case 2: rpg_bat_intro(); break;
        }
        break;
    case 0x00100000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch ((u32)(rpg_bat_unit1.flag & 0xFFFF)) {
        case 1: rpg_bat_intro_mode(); break;
        case 2: furyou_punch_hit_seq(); break;
        case 3: furyou_punch_miss_seq(); break;
        case 4: furyou_punch_ally_seq(); break;
        case 5: furyou_punch_stun_seq(); break;
        case 6: rpg_bat_intro_step(); break;
        case 7: rpg_bat_furyou_end_step(); break;
        }
        break;
    case 0x00200000:
    case 0x00400000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch ((u32)(rpg_bat_unit1.flag & 0xFFFF)) {
        case 1: banchou_talk_seq(); break;
        case 2: banchou_attack(); break;
        case 3: banchou_power_seq(); break;
        case 4: rpg_bat_shade_step(); break;
        case 5: rpg_bat_lose_seq(); break;
        }
        break;
    case 0x08000000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 1: robo_attack(); break;
        case 2: robo_power_seq(); break;
        }
        break;
    case (s32)0x80000000:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 1: utyuuzin_attack(); break;
        case 2: utyuuzin_attack2(); break;
        case 3: rpg_bat_escape_seq(); break;
        case 4: utyuuzin_talk(); break;
        }
        break;
    default:
        rpg_bat_unit1.flag = rpg_bat_unit1.flag;
        break;
    }
}
