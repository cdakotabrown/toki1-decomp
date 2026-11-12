/* @SymbolStart: 0x8013415C */
/* @SymbolSize: 1044 */
/* @RodataBase: 0x8015ACAC */
/* rpg_bat_seq1 - RPG_BAT overlay, Address: 0x80134164, Size: 1036 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void strcpy(u8 *, u8 *);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_score_disp(s32, s32, s32, s32, s32);
extern void rpg_bat_act_next(void);
extern void rpg_bat_reset(void);
extern void rpg_bat_act_wait(s32);
extern void rpg_bat_act_gate(s32);

extern u8  rpg_bat_seq1_str0[];
extern u8  rpg_bat_seq1_str1[];
extern u8  rpg_bat_msg_buf3[];
extern u8  rpg_bat_msg_buf3_1[];
extern u8  rpg_bat_msg_tbl3[];
extern s32 rpg_bat_msg_ctl_mode;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_seq1(void)
{
    switch (rpg_bat_unit2.step) {
    case 0:
        strcpy(rpg_bat_msg_buf3, rpg_bat_seq1_str0);
        strcpy(rpg_bat_msg_buf3_1, rpg_bat_seq1_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl3, 2, 60);
        rpg_bat_anim_set(29, 5, 5, 1);
        rpg_bat_koe_set(0x1F0000A1, 0x1D0000A4, 0);
        rpg_bat_act_next();
        break;
    case 1:
        rpg_bat_act_gate(1);
        break;
    case 2:
        rpg_bat_koe_set(0x0700001F, 0x0600002B, 1);
        rpg_bat_msg_ctl_mode = 3;
        rpg_bat_act_wait(30);
        break;
    case 3:
        rpg_bat_anim_set(29, 7, 1, 1);
        rpg_bat_act_next();
        break;
    case 4:
        rpg_bat_sprite_pos_set(29, rpg_bat_unit2.cnt * -168 / 1000,
                      rpg_bat_unit2.cnt * -48 / 1000);
        rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 5;
        if (rpg_bat_unit2.cnt < 1001) {
            break;
        }
        rpg_bat_sprite_pos_set(29, -168, -48);
        rpg_bat_sprite_pos_set(30, -168, -48);
        rpg_bat_anim_set(29, 8, 1, 1);
        rpg_bat_anim_set(30, 12, 1, 1);
        rpg_bat_score_disp(0, 72, 80, 0, 1);
        rpg_bat_se_set(1283, 1, 0);
        rpg_bat_msg_ctl_mode = 1;
        rpg_bat_act_next();
        break;
    case 5:
        rpg_bat_act_wait(60);
        break;
    case 6:
        rpg_bat_score_disp(1, 100, 96, rpg_bat_unit2.hp - 7, 1);
        rpg_bat_unit2.hp = 7;
        rpg_bat_act_next();
        break;
    case 7:
        rpg_bat_act_wait(60);
        break;
    case 8:
        rpg_bat_anim_set(29, 3, 1, 1);
        rpg_bat_anim_set(30, 255, 1, 0);
        rpg_bat_act_next();
        break;
    case 9:
        rpg_bat_sprite_pos_set(29, rpg_bat_unit2.cnt * 168 / 100 - 168,
                      rpg_bat_unit2.cnt * 48 / 100 - 48);
        rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 2;
        if (rpg_bat_unit2.cnt < 101) {
            break;
        }
        rpg_bat_sprite_pos_set(29, 0, 0);
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_act_next();
        break;
    case 10:
        rpg_bat_act_gate(0);
        break;
    case 11:
        rpg_bat_act_wait(20);
        break;
    case 12:
        rpg_bat_reset();
        break;
    }
}
