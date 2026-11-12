/* @SymbolStart: 0x80140CC8 */
/* @RodataBase: 0x8015C148 */
/* robo_power_seq -- RPG_BAT overlay, Address: 0x80140CD0, Size: 416 bytes */
/* @SymbolSize: 424 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_talk1a(void);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void k_disp_switch(s32, s32);
extern void rpgbat_palette_anim(void);
extern void rpg_bat_effect_clear(void);

extern u8  rpg_bat_msg_buf2[];
extern u8  p_sei[];
extern u8  robo_power_seq_str0[];
extern u8  rpg_bat_msg_tbl2[];
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void robo_power_seq(void)
{
    s32 n;

    switch (rpg_bat_unit1.step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, robo_power_seq_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_seq_step_gate(0);
        break;
    case 2:
        rpg_bat_anim_set2(59, 1, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 3:
        rpg_bat_talk1a();
        break;
    case 4:
        rpg_bat_anim_set2(59, 0, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 5:
        rpg_bat_seq_wait(30);
        break;
    case 6:
        rpg_bat_sprite_pos_set(59, 72 - rpg_bat_unit1.cnt, 112);
        n = rpg_bat_unit1.cnt + 16;
        rpg_bat_unit1.cnt = n;
        if (n >= 97) {
            k_disp_switch(rpg_bat_unit2.disp, 0);
            rpg_bat_seq_step_next();
        }
        break;
    case 7:
        rpg_bat_seq_wait(30);
        break;
    case 8:
        rpgbat_palette_anim();
        break;
    case 9:
        rpg_bat_effect_clear();
        k_disp_switch(rpg_bat_unit1.disp, 1);
        rpg_bat_unit1.flag = 0x08000001;
        break;
    }
}
