/* @SymbolStart: 0x80149AA8 */
/* @RodataBase: 0x8015CCD8 */
/* @SymbolSize: 436 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern void rpg_bat_anim_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_anim_set2(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_sprite_bri_set(s32 a0);
extern void rpg_bat_se_set2(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_state_next(void);
extern void rpg_bat_timer_sub(s32 a0);
extern void rpg_bat_step_next(void);

extern s16 cw137_frame;
extern s32 rpg_bat_hit_cnt;
extern s32 rpg_bat_seq_work;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_effect_step6(void)
{
    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(40, 255, 1, 0);
        rpg_bat_anim_set2(41, 14, 5, 1);
        rpg_bat_state_next();
        break;
    case 1:
        if (cw137_frame + 3 == rpg_bat_seq_work) {
            rpg_bat_sprite_bri_set(255);
            switch ((u32)rpg_bat_hit_cnt) {
            default:
            case 0:
                rpg_bat_se_set2(1285, 1, 0);
                break;
            case 1:
                rpg_bat_se_set2(1285, 1, 0);
                break;
            case 2:
                rpg_bat_se_set2(1286, 1, 0);
                break;
            case 3:
                rpg_bat_se_set2(1287, 1, 0);
                break;
            case 4:
                rpg_bat_se_set2(1288, 1, 0);
                break;
            case 5:
                rpg_bat_se_set2(1289, 1, 0);
                break;
            }
            rpg_bat_state_next();
        }
        break;
    case 2:
        cw137_frame = rpg_bat_seq_work - 3;
        rpg_bat_timer_sub(10);
        break;
    case 3:
        rpg_bat_sprite_bri_set(128);
        rpg_bat_anim_set2(41, 255, 1, 0);
        rpg_bat_step_next();
        break;
    }
}
