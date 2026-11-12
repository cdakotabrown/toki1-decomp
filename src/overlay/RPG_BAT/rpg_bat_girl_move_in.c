/* @SymbolStart: 0x80151984 */
/* @SymbolSize: 644 */
/* rpg_bat_girl_move_in - RPG_BAT overlay, Address: 0x8015198C, Size: 636 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_step_next(void);

extern s32 rpg_bat_girl_away_x;
extern s32 rpg_bat_girl_away_y;
extern s32 rpg_bat_girl_home_x;
extern s32 rpg_bat_girl_home_y;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_girl;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_girl_move_in(void)
{
    if (rpg_bat_unit0.wait == 0) {
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_anim_set(51, 8, 1, 0);
    }

    rpg_bat_sprite_pos_set(29,
                  (rpg_bat_girl_away_x - rpg_bat_girl_home_x) * rpg_bat_unit0.wait / 100 + rpg_bat_girl_home_x,
                  (rpg_bat_girl_away_y - rpg_bat_girl_home_y) * rpg_bat_unit0.wait / 100 + rpg_bat_girl_home_y);

    rpg_bat_sprite_pos_set(50,
                  (rpg_bat_girl_away_x - rpg_bat_girl_home_x) * rpg_bat_unit0.wait / 100 + rpg_bat_girl_home_x +
                      rpg_bat_tgt_x,
                  (rpg_bat_girl_away_y - rpg_bat_girl_home_y) * rpg_bat_unit0.wait / 100 + rpg_bat_girl_home_y +
                      rpg_bat_tgt_y);

    rpg_bat_unit0.wait = rpg_bat_unit0.wait + 4;
    if (rpg_bat_unit0.wait >= 101) {
        rpg_bat_sprite_pos_set(29, 0, 0);
        if (rpg_bat_girl == 1 && rpg_bat_unit2.flag == 4096) {
            rpg_bat_anim_set(29, 7, 1, 1);
        } else {
            rpg_bat_anim_set(29, 0, 1, 1);
        }
        rpg_bat_anim_set2(50, 255, 1, 0);
        class_w[146].prio = 5;
        rpg_bat_step_next();
    }
}
