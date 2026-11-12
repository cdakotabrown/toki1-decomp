/* @SymbolStart: 0x80155CC0 */
/* @SymbolSize: 448 */
/* @RodataBase: 0x8015DB48 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern void strcpy(void *, void *);
extern void rpg_bat_msg_set(void *, s32, s32);
extern void rpg_bat_target_set(void);
extern void rpg_bat_pos1_set(s32);
extern void rpg_bat_reward_set(void);
extern void rpg_bat_effect_rec_set(s32);
extern void rpg_bat_step_next(void);
extern void rpg_bat_slide1(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_timer_step(s32);
extern void rpg_bat_bukatu8_fly_step(void);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_se_set2(s32, s32, s32);
extern void rpg_bat_rec_step(s32);
extern void bat_spark_step(void);
extern void rpg_bat_row_disp(void);
extern void rpg_bat_bukatu_exit(void);
extern void rpg_bat_clear(void);

extern u8  rpg_bat_msg_buf[];
extern u8  rpg_bat_bukatu8_attack_str0[];
extern u8  rpg_bat_msg_tbl[];
extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_bukatu8_attack(void)
{
    switch (rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_bukatu8_attack_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_effect_rec_set(3);
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 1:
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_timer_step(41);
        break;
    case 5:
        rpg_bat_bukatu8_fly_step();
        break;
    case 6:
        rpg_bat_anim_set2(51, 6, 5, 1);
        rpg_bat_se_set2(1296, 0, 2);
        rpg_bat_step_next();
        break;
    case 7:
        rpg_bat_rec_step(51);
        break;
    case 8:
        bat_spark_step();
        break;
    case 9:
        rpg_bat_effect_rec_set(255);
        rpg_bat_row_disp();
        rpg_bat_step_next();
        break;
    case 10:
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_step_next();
        break;
    case 11:
        rpg_bat_timer_step(20);
        break;
    case 12:
        rpg_bat_bukatu_exit();
        break;
    case 13:
        rpg_bat_clear();
        break;
    }
}
