/* @SymbolStart: 0x80159E28 */
/* @RodataBase: 0x8015E420 */
/* @SymbolSize: 756 */
/* kosiki_act - RPG_BAT overlay, Address: 0x80159E30, Size: 748 bytes (187 insns) */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 strcpy();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_pos2_set();
extern s32 rpg_bat_param_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_act_gate();
extern s32 rpg_bat_slide_out();
extern s32 kosiki_talk_step();
extern s32 rpg_bat_blink2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_move();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_blink3();
extern s32 rpg_bat_slide_in();
extern s32 rpg_bat_reset();

extern u8 kosiki_act_str0[];
extern u8 rpg_bat_msg_buf3[];
extern u8 rpg_bat_msg_tbl3[];
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void kosiki_act(void)
{
    s32 v0;
    s32 f;

    switch ((u32)rpg_bat_unit2.step) {
    case 0:
        strcpy(rpg_bat_msg_buf3, kosiki_act_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos2_set(rpg_bat_target_no);
        rpg_bat_param_set();
        rpg_bat_act_next();
        return;
    case 1:
        rpg_bat_act_gate(1);
        return;
    case 2:
        rpg_bat_slide_out(0, 0);
        return;
    case 3:
        kosiki_talk_step();
        return;
    case 4:
        rpg_bat_blink2();
        return;
    case 5:
        rpg_bat_sprite_pos_set(29, 352, 104);
        rpg_bat_anim_set2(29, 4, 1, 1);
        rpg_bat_act_next();
        return;
    case 6:
        if ((rpg_bat_unit1.flag << 11) < 0) {
            rpg_bat_sprite_move(29, 352, 104, 168, 104, 150);
        } else {
            rpg_bat_sprite_move(29, 352, 104, 144, 104, 150);
        }
        return;
    case 7:
        rpg_bat_sprite_pos_set(30, rpg_bat_target_x, rpg_bat_target_y);
        rpg_bat_anim_set2(30, 5, 1, 1);
        rpg_bat_act_next();
        return;
    case 8:
        rpg_bat_se_set(1298, 15, 1);
        rpg_bat_act_wait(55);
        return;
    case 9:
        rpg_bat_anim_set2(30, 255, 1, 0);
        rpg_bat_slot_adjust(0, rpg_bat_target_no, rpg_bat_dmg, 1);
        rpg_bat_act_next();
        return;
    case 10:
        rpg_bat_act_wait(30);
        return;
    case 11:
        f = rpg_bat_unit2.cnt;
        if ((f & 1) == 0) {
            rpg_bat_anim_set2(29, 255, 1, 1);
        } else {
            rpg_bat_anim_set2(29, 255, 1, 0);
        }
        v0 = rpg_bat_unit2.cnt + 1;
        rpg_bat_unit2.cnt = v0;
        if (v0 < 60) {
            return;
        }
        rpg_bat_sprite_pos_set(29, -32, 0);
        rpg_bat_anim_set(29, 1, 1, 0);
        rpg_bat_act_next();
        return;
    case 12:
        rpg_bat_blink3();
        return;
    case 13:
        rpg_bat_slide_in(1, 0);
        return;
    case 14:
        rpg_bat_act_wait(20);
        return;
    case 15:
        rpg_bat_reset();
        return;
    }
}
