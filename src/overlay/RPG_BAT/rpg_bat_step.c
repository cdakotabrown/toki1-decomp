/* @SymbolStart: 0x80134D30 */
/* @RodataBase: 0x8015ADB8 */
/* @SymbolSize: 328 */
/* rpg_bat_step - RPG_BAT overlay, Address: 0x80134D38, Size: 320 bytes (80 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 strcpy();
extern s32 rpg_bat_layout_init();
extern s32 rpg_bat_hud_tween();
extern s32 rpg_bat_hameln_anim_step();
extern s32 rpg_bat_blink_step();
extern s32 rpg_bat_blink_step2();
extern s32 rpg_bat_rows_disp();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_pos1_set();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern char rpg_bat_step_str0[];
extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_tbl[];
extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step(void)
{
    switch (rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_step_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 2: rpg_bat_slide1(); break;
    case 3: rpg_bat_hameln_anim_step(); break;
    case 4: rpg_bat_blink_step(); break;
    case 5: rpg_bat_layout_init(); break;
    case 6: rpg_bat_hud_tween(); break;
    case 7:
        rpg_bat_rows_disp();
        rpg_bat_anim_set(52, 2, 1, 0);
        rpg_bat_step_next();
        break;
    case 8: rpg_bat_blink_step2(); break;
    case 9: rpg_bat_bukatu_exit(); break;
    case 10: rpg_bat_clear(); break;
    default: break;
    }
}
