/* @PostProc: rename_register_chain --chain v1:v0 */
/* @RodataBase: 0x8015E4A8 */
/* @SymbolStart: 0x8015A1F0 */
/* @SymbolSize: 1252 */
/* rpg_bat_bukatu6_attack - RPG_BAT overlay, Address: 0x8015A1F8, Size: 1244 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern u8 rpg_bat_bukatu6_attack_str0[];
extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_tbl[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern void strcpy();
extern void rpg_bat_rows_disp();
extern void rpg_bat_slide1();
extern void rpg_bat_bukatu_exit();
extern void rpg_bat_anim_set();
extern void rpg_bat_sprite_pos_set();
extern void rpg_bat_se_set2();
extern void rpg_bat_target_set();
extern void rpg_bat_reward_set();
extern void rpg_bat_msg_set();
extern void rpg_bat_step_next();
extern void rpg_bat_clear();
extern void rpg_bat_timer_step();
extern void rpg_bat_rec_step();
extern void rpg_bat_sprites_init();
extern void rpg_bat_bukatu6_ball_step();
extern void rpg_bat_rec_rescale();

void rpg_bat_bukatu6_attack(void)
{
    s32 t;

    if (rpg_bat_unit0.step >= 6 && rpg_bat_unit0.step < 9) {
        rpg_bat_se_set2(1280, 14, 1);
    }

    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_bukatu6_attack_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_reward_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_rec_step(52);
        break;
    case 5:
        rpg_bat_anim_set(51, 6, 1, 1);
        rpg_bat_anim_set(52, 4, 1, 1);
        rpg_bat_anim_set(53, 5, 1, 1);
        rpg_bat_step_next();
        break;
    case 6:
        rpg_bat_sprite_pos_set(51, 0, rpg_bat_unit0.cnt * -48 * rpg_bat_unit0.cnt / 3600);
        rpg_bat_sprite_pos_set(52, 0, rpg_bat_unit0.cnt * -48 * rpg_bat_unit0.cnt / 3600);
        rpg_bat_sprite_pos_set(53, 0, rpg_bat_unit0.cnt * -48 * rpg_bat_unit0.cnt / 3600);
        rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 1;
        if (rpg_bat_unit0.cnt >= 61) {
            rpg_bat_sprite_pos_set(51, 0, -48);
            rpg_bat_sprite_pos_set(52, 0, -48);
            rpg_bat_sprite_pos_set(53, 0, -48);
            rpg_bat_step_next();
        }
        break;
    case 7:
        rpg_bat_sprites_init();
        break;
    case 8:
        rpg_bat_bukatu6_ball_step();
        rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 1;
        if (rpg_bat_unit0.cnt >= 301) {
            rpg_bat_step_next();
        }
        break;
    case 9:
        rpg_bat_rec_rescale();
        t = rpg_bat_unit0.cnt;
        if (t == 0) {
            t = 60;
        }
        rpg_bat_unit0.cnt = t;
        rpg_bat_sprite_pos_set(51, 0, t * -48 * t / 3600);
        rpg_bat_sprite_pos_set(52, 0, rpg_bat_unit0.cnt * -48 * rpg_bat_unit0.cnt / 3600);
        rpg_bat_sprite_pos_set(53, 0, rpg_bat_unit0.cnt * -48 * rpg_bat_unit0.cnt / 3600);
        rpg_bat_unit0.cnt = rpg_bat_unit0.cnt - 1;
        if (rpg_bat_unit0.cnt >= 15) {
            rpg_bat_se_set2(1280, 14, 1);
        }
        if (rpg_bat_unit0.cnt > 0) {
            break;
        }
        rpg_bat_se_set2(1282, 1, 0);
        rpg_bat_rows_disp();
        rpg_bat_sprite_pos_set(51, 0, 0);
        rpg_bat_sprite_pos_set(52, 0, 0);
        rpg_bat_sprite_pos_set(53, 0, 0);
        rpg_bat_step_next();
        break;
    case 10:
        rpg_bat_anim_set(51, 255, 1, 0);
        rpg_bat_anim_set(52, 8, 5, 1);
        rpg_bat_anim_set(53, 255, 1, 0);
        rpg_bat_step_next();
        break;
    case 11:
        rpg_bat_rec_step(52);
        break;
    case 12:
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_step_next();
        break;
    case 13:
        rpg_bat_timer_step(20);
        break;
    case 14:
        rpg_bat_bukatu_exit();
        break;
    case 15:
        rpg_bat_clear();
        break;
    }
}
