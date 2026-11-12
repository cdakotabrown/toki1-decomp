/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=3:v0,21:v1 */
/* @SymbolSize: 952 */
/* @RodataBase: 0x8015D110 */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* rpg_bat_slide_disp - RPG_BAT overlay, Address: 0x80150120, Size: 952 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32  get_rand_mod(s32);
extern void strcpy(u8 *, u8 *);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern void k_disp_switch(s32, s32);
extern void rpg_bat_act_next(void);
extern void rpg_bat_act_sub_next(void);
extern void rpg_bat_act_sub_gate(s32);

extern u8  rpg_bat_slide_disp_str0[];
extern u8  rpg_bat_slide_disp_str1[];
extern u8  rpg_bat_msg_buf2[];
extern u8  rpg_bat_msg_buf2_1[];
extern u8  rpg_bat_msg_tbl2[];
extern s32 rpg_bat_bg_wave_amp;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_slide_disp(void)
{
    s32 color;

    switch (get_rand_mod(8)) {
    case 0:
        color = 0x9F9F9F;
        break;
    case 1:
        color = 0x6F6F6F;
        break;
    case 2:
        color = 0xBF0000;
        break;
    case 3:
        color = 0xBF00;
        break;
    case 4:
        color = 191;
        break;
    case 5:
        color = 0xAFAF00;
        break;
    case 6:
        color = 0xAF00AF;
        break;
    case 7:
        color = 0xAFAF;
        break;
    }

    rpg_bat_unit0.flag = 4096;

    switch (rpg_bat_unit2.sub) {
    case 0:
        strcpy(rpg_bat_msg_buf2, rpg_bat_slide_disp_str0);
        strcpy(rpg_bat_msg_buf2_1, rpg_bat_slide_disp_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 2, 90);
        rpg_bat_se_set(1299, 1, 0);
        rpg_bat_act_sub_next();
        break;
    case 1:
        rpg_bat_act_sub_gate(1);
        break;
    case 2:
        rpg_bat_se_set(1298, 1, 0);
        rpg_bat_act_sub_next();
        break;
    case 3:
        rpg_bat_act_sub_gate(0);
        break;
    case 4:
        rpg_bat_bg_wave_amp = 24 - rpg_bat_unit2.wait;
        rpg_bat_unit2.wait = rpg_bat_unit2.wait + 2;
        if (rpg_bat_unit2.wait < 121) {
            break;
        }
        rpg_bat_sprite_pos_set(56, 0, 0);
        rpg_bat_anim_set(56, 9, 1, 1);
        class_w[152].prio = 2;
        rpg_bat_anim_set(59, 8, 1, 1);
        rpg_bat_act_sub_next();
        break;
    case 5:
        _sprite_set_box_shade(-92 - rpg_bat_unit2.wait, -120, rpg_bat_unit2.wait * 2, 168, 3,
                      color, 129);
        dtd_on_tpage(0, 0, 3, 1, 0);
        rpg_bat_unit2.wait = rpg_bat_unit2.wait + 2;
        if (rpg_bat_unit2.wait < 33) {
            break;
        }
        rpg_bat_act_sub_next();
        break;
    case 6:
        _sprite_set_box_shade(-124, -120, 64, 168, 3, color, 129);
        dtd_on_tpage(0, 0, 3, 1, 0);
        rpg_bat_se_set(1297, 0, 6);
        rpg_bat_sprite_pos_set(59, 72, 104 - rpg_bat_unit2.wait);
        rpg_bat_anim_set2(59, 8, 1, 1);
        rpg_bat_unit2.wait = rpg_bat_unit2.wait + 2;
        if (rpg_bat_unit2.wait < 233) {
            break;
        }
        k_disp_switch(rpg_bat_unit1.disp, 0);
        rpg_bat_act_sub_next();
        break;
    case 7:
        _sprite_set_box_shade(rpg_bat_unit2.wait - 124, -120, 64 - rpg_bat_unit2.wait * 2, 168, 3,
                      color, 129);
        dtd_on_tpage(0, 0, 3, 1, 0);
        rpg_bat_unit2.wait = rpg_bat_unit2.wait + 2;
        if (rpg_bat_unit2.wait < 33) {
            break;
        }
        rpg_bat_act_next();
        break;
    }
}
