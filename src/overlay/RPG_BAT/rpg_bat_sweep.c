/* @RodataBase: 0x8015AA30 */
/* @SymbolStart: 0x80132348 */
/* @SymbolSize: 1404 */
/* @PostProc: scheduler_normalizer --block-aware --fence 96-115 */
/* @PostProc: patch_insn_reg_field 0x520:rs:t5:t1 0x520:rt:t1:t5 */
/* rpg_bat_sweep - RPG_BAT overlay, Address: 0x80132348, Size: 1404 bytes. */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_move_dx;
extern s32 rpg_bat_move_dy;
extern s32 rpg_bat_girl_away_x;
extern s32 rpg_bat_girl_away_y;
extern s32 rpg_bat_girl_home_x;
extern s32 rpg_bat_girl_home_y;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_target_no;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rsin();
extern s32 rpg_bat_sprite_sway();
extern s32 rpg_bat_sprites_set();
extern s32 rpg_bat_pos_set();
extern s32 rpg_bat_row_disp();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_timer_sub();

void rpg_bat_sweep(void)
{
    s32 sel = rpg_bat_unit0.sub;
    s32 ang;

    switch ((u32)sel) {
    case 0:
        rpg_bat_move_dx = -48;
        rpg_bat_move_dy = -320;
        rpg_bat_anim_set2(52, 5, 5, 1);
        rpg_bat_se_set2(1285, 1, 0);
        rpg_bat_state_next();
        break;

    case 1:
        if (class_w[148].frame == 2) {
            rpg_bat_state_next();
        }
        break;

    case 2:
        rpg_bat_sprite_pos_set(52, rpg_bat_move_dx * rpg_bat_unit0.wait / 1000 + 272,
                      rpg_bat_move_dy * rpg_bat_unit0.wait / 1000 + 88);
        rpg_bat_unit0.wait += 20;
        if (rpg_bat_unit0.wait > 1000) {
            rpg_bat_anim_set2(52, 6, 1, 1);
            rpg_bat_pos_set();
            rpg_bat_move_dx = rpg_bat_girl_home_x - rpg_bat_girl_away_x;
            rpg_bat_move_dy = rpg_bat_girl_home_y - rpg_bat_girl_away_y;
            rpg_bat_state_next();
        }
        break;

    case 3:
        rpg_bat_sprite_pos_set(52, rpg_bat_move_dx * rpg_bat_unit0.wait / 1000 + rpg_bat_girl_away_x,
                      rpg_bat_move_dy * rpg_bat_unit0.wait / 1000 + rpg_bat_girl_away_y);
        rpg_bat_unit0.wait += 30;
        if (rpg_bat_tgt_y - 16 <
            rpg_bat_girl_away_y + rpg_bat_move_dy * rpg_bat_unit0.wait / 1000) {
            rpg_bat_se_set2(1284, 0, 1);
            rpg_bat_sprites_set(rpg_bat_tgt_x, rpg_bat_tgt_y);
            rpg_bat_sprite_sway(rpg_bat_tgt_x, rpg_bat_tgt_y);
            rpg_bat_anim_set2(52, 13, 1, 1);
        }
        if (rpg_bat_unit0.wait > 1000) {
            if (rpg_bat_target_no == 1) {
                class_w[155].prio = 4;
                class_w[157].prio = 4;
            }
            rpg_bat_anim_set2(52, 7, 5, 1);
            class_w[148].frame = 1;
            rpg_bat_state_next();
        }
        break;

    case 4:
        rpg_bat_sprite_sway(rpg_bat_tgt_x, rpg_bat_tgt_y);
        rpg_bat_timer_sub(30);
        break;

    case 5:
        rpg_bat_row_disp();
        rpg_bat_state_next();
        break;

    case 6:
        rpg_bat_sprite_sway(rpg_bat_tgt_x, rpg_bat_tgt_y);
        ang = rsin(rpg_bat_unit0.wait * 2048 / 1000);
        rpg_bat_sprite_pos_set(52,
                      (240 - rpg_bat_girl_home_x) * rpg_bat_unit0.wait / 1000 + rpg_bat_girl_home_x,
                      (88 - rpg_bat_girl_home_y) * rpg_bat_unit0.wait / 1000 + rpg_bat_girl_home_y -
                          (ang >> 7));
        rpg_bat_unit0.wait += 20;
        rpg_bat_anim_set2(52, 8, 1, 1);
        if (rpg_bat_unit0.wait > 1000) {
            rpg_bat_se_set2(1282, 1, 0);
            rpg_bat_step_next();
        }
        break;
    }
}
