/* @SymbolStart: 0x801516E4 */
/* @SymbolSize: 672 */
/* rpg_bat_girl_move_out - RPG_BAT overlay, Address: 0x801516EC, Size: 664 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern void rpg_bat_pose_set(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_step_next(void);
extern void rpg_bat_state_next(void);

extern s32 rpg_bat_girl_away_x;
extern s32 rpg_bat_girl_away_y;
extern s32 rpg_bat_girl_home_x;
extern s32 rpg_bat_girl_home_y;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_girl_move_out(void)
{
    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_pose_set();
        rpg_bat_sprite_pos_set(29, 280, 136);
        rpg_bat_sprite_pos_set(50, rpg_bat_tgt_x + 280, rpg_bat_tgt_y + 136);
        rpg_bat_anim_set2(29, 0, 1, 1);
        rpg_bat_anim_set2(50, 7, 5, 1);
        (*(u16 *)&class_w[146].frame) = 1;
        class_w[125].prio = 4;
        class_w[146].prio = 3;
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_sprite_pos_set(29,
                      (rpg_bat_girl_home_x - rpg_bat_girl_away_x) * rpg_bat_unit0.wait / 100 +
                          rpg_bat_girl_away_x,
                      (rpg_bat_girl_home_y - rpg_bat_girl_away_y) * rpg_bat_unit0.wait / 100 +
                          rpg_bat_girl_away_y);
        rpg_bat_sprite_pos_set(50,
                      (rpg_bat_girl_home_x - rpg_bat_girl_away_x) * rpg_bat_unit0.wait / 100 +
                          rpg_bat_girl_away_x + rpg_bat_tgt_x,
                      (rpg_bat_girl_home_y - rpg_bat_girl_away_y) * rpg_bat_unit0.wait / 100 +
                          rpg_bat_girl_away_y + rpg_bat_tgt_y);
        rpg_bat_unit0.wait = rpg_bat_unit0.wait + 4;
        if (rpg_bat_unit0.wait >= 101) {
            rpg_bat_state_next();
        }
        break;
    case 2:
        rpg_bat_step_next();
        break;
    }
}
