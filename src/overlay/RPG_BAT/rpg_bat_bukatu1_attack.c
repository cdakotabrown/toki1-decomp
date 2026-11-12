/* @SymbolStart: 0x801505B8 */
/* @SymbolSize: 1260 */
/* @RodataBase: 0x8015D54C */
/* rpg_bat_bukatu1_attack - RPG_BAT overlay, Address: 0x801505B8, Size: 1260 bytes. */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 pad0[3];
    u8 flags3;
    u8 pad1[64];
} BatRec;

extern BatRec class_w[];

extern volatile s16 cw146_x;
extern volatile s16 cw152_x;

extern u8 rpg_bat_bukatu1_attack_str0[];
extern u8 rpg_bat_bukatu1_attack_str1[];
extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_tbl[];
extern s32 rpg_bat_mode;
extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 strcpy();
extern s32 rpg_bat_rows_disp();
extern s32 rpg_bat_row_disp();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_pos1_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_timer_step();
extern s32 rpg_bat_step_gate();
extern s32 himo_manzai_talk();
extern s32 rpg_bat_manzai_girl_sel();
extern s32 rpg_bat_round();
extern s32 rpg_bat_fade_set();
extern s32 rpg_bat_layout_set();
extern s32 rpg_bat_enemy_pose_set();

void rpg_bat_bukatu1_attack(void)
{
    s32 sel = rpg_bat_unit0.step;
    s32 i;

    switch (sel) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_bukatu1_attack_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_layout_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;

    case 2:
        rpg_bat_slide1();
        break;

    case 3:
        rpg_bat_round();
        break;

    case 4:
        if (rpg_bat_mode == 0x8000000) {
            rpg_bat_unit0.step = 200;
            return;
        }
        rpg_bat_fade_set(rpg_bat_unit0.cnt);
        rpg_bat_unit0.cnt += 4;
        if (rpg_bat_unit0.cnt >= 225) {
            rpg_bat_step_next();
        }
        break;

    case 5:
        rpg_bat_rows_disp();
        if ((rpg_bat_unit1.flag << 11) < 0) {
            for (i = 0; i < 3; i++) {
                class_w[i + 149].flags3 &= ~1;
            }
        }
        rpg_bat_step_next();
        break;

    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        rpg_bat_manzai_girl_sel();
        break;

    default:
        rpg_bat_unit0.step = 100;

    case 100:
        rpg_bat_fade_set(224 - rpg_bat_unit0.cnt);
        rpg_bat_unit0.cnt += 4;
        if (rpg_bat_unit0.cnt >= 225) {
            rpg_bat_enemy_pose_set();
            rpg_bat_step_next();
        }
        break;

    case 101:
        rpg_bat_timer_step(20);
        break;

    case 102:
        rpg_bat_bukatu_exit();
        break;

    case 103:
        rpg_bat_clear();
        break;

    case 200:
        rpg_bat_timer_step(60);
        break;

    case 201:
        strcpy(rpg_bat_msg_buf, rpg_bat_bukatu1_attack_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_step_next();
        break;

    case 202:
        rpg_bat_step_gate(0);
        break;

    case 203:
        rpg_bat_row_disp();
        rpg_bat_step_next();
        break;

    case 204:
        rpg_bat_timer_step(60);
        break;

    case 205:
        himo_manzai_talk();
        break;

    case 206:
        rpg_bat_sprite_pos_set(56, 320, 94);
        rpg_bat_anim_set2(56, 1, 1, 1);
        rpg_bat_sprite_pos_set(50, 320, 96);
        rpg_bat_anim_set2(50, 6, 5, 1);
        ((CLASS_W *)class_w)[152].disp_flag |= 1;
        ((CLASS_W *)class_w)[152].prio = 4;
        ((CLASS_W *)class_w)[146].prio = 4;
        rpg_bat_step_next();
        break;

    case 207:
        cw152_x = cw152_x - 16;
        cw146_x = cw146_x - 16;
        rpg_bat_unit0.cnt++;
        if (rpg_bat_unit0.cnt == 2) {
            rpg_bat_se_set2(1299, 0, 5);
            rpg_bat_anim_set(52, 12, 1, 1);
            rpg_bat_anim_set(51, 8, 1, 1);
            ((CLASS_W *)class_w)[146].frame = 2;
        }
        if (rpg_bat_unit0.cnt == 10) {
            ((CLASS_W *)class_w)[152].prio = 6;
            ((CLASS_W *)class_w)[146].prio = 6;
        }
        if (rpg_bat_unit0.cnt >= 22) {
            rpg_bat_step_next();
        }
        break;

    case 208:
        if (rpg_bat_unit0.wait == 20) {
            rpg_bat_anim_set(51, 255, 1, 0);
        }
        rpg_bat_timer_step(60);
        break;

    case 209:
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_anim_set(51, 255, 1, 0);
        rpg_bat_unit0.step = 101;
        break;
    }
}
