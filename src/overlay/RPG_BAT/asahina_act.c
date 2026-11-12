/* @SymbolStart: 0x8015675C */
/* @RodataBase: 0x8015DC2C */
/* @SymbolSize: 264 */
/* asahina_act - RPG_BAT overlay, Address: 0x80156764, Size: 256 bytes (64 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_slide_out();
extern s32 rpg_bat_slide_in();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_flag_act();
extern s32 asahina_talk();
extern s32 bat_blink_step();
extern s16 cw125_frame;
extern RPG_BAT_UNIT rpg_bat_unit2;

void asahina_act(void)
{
    s32 t7;

    switch (rpg_bat_unit2.step) {
    case 0:
        rpg_bat_anim_set(29, 5, 5, 1);
        rpg_bat_act_next();
        break;
    case 1:
        t7 = cw125_frame;
        if (t7 == 2) {
            rpg_bat_se_set(1299, 0, 1);
        }
        rpg_bat_flag_act(29);
        break;
    case 2:
        rpg_bat_slide_out(6, 0);
        break;
    case 3:
        asahina_talk();
        break;
    case 4:
        bat_blink_step();
        break;
    case 5:
        rpg_bat_act_wait(20);
        break;
    case 6:
        rpg_bat_slide_in(3, 0);
        break;
    case 7:
        rpg_bat_act_wait(20);
        break;
    case 8:
        rpg_bat_reset();
        break;
    default:
        break;
    }
}
