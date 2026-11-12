/* @SymbolStart: 0x801596E8 */
/* @SymbolSize: 1112 */
/* @RodataBase: 0x8015E344 */
/* @PostProc: swap_instructions --offset1 0x1cc --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1d4 */
/* rpg_bat_win_seq - RPG_BAT overlay, Address: 0x801596F0, Size: 1104 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 date_name_set_tbl0[];
extern s32 p_sei[];
extern s32 rpg_bat_win_seq_str0[];
extern s32 rpg_bat_win_seq_str1[];
extern s32 rpg_bat_win_seq_str2[];
extern s32 rpg_bat_win_seq_str3[];
extern s32 rpg_bat_win_seq_str4[];
extern s32 rpg_bat_win_seq_str5[];
extern s32 rpg_bat_msg_buf4[];
extern s32 rpg_bat_msg4_line1[];
extern s32 rpg_bat_msg4_line2[];
extern s32 rpg_bat_msg_tbl4[];
extern s32 rpg_bat_girl;
extern s32 rpg_bat_result;
extern u8 cw155_disp;
extern s16 cw155_x;
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

extern void sub_step_up();
extern void device_port_init();
extern void text_sys_init();
extern void k_disp_start();
extern void k_disp_switch();
extern void k_sub_reset_point_set();
extern void strcpy();
extern void strcat();
extern void rpg_bat_anim_set();
extern s32 rpg_bat_kanji_set();
extern void rpg_bat_se_set();
extern void rpg_bat_msg_set();
extern void rpg_bat_seq_next();
extern void rpg_bat_seq_wait3();
extern void rpg_bat_seq_gate();
extern void banchou_defeat_talk();
extern void banchou_title_talk();
extern void banchou_bye_talk();
extern void rpg_bat_msg_build();

void rpg_bat_win_seq(void)
{
    switch ((u32)rpg_bat_unit3.step) {
    case 0:
        rpg_bat_unit0.flag = 4096;
        rpg_bat_seq_next();
        return;
    case 1:
        banchou_defeat_talk();
        return;
    case 2:
        strcpy(rpg_bat_msg_buf4, p_sei);
        strcat(rpg_bat_msg_buf4, rpg_bat_win_seq_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 90);
        rpg_bat_seq_next();
        return;
    case 3:
        rpg_bat_seq_gate(0);
        return;
    case 4:
        banchou_title_talk();
        return;
    case 5:
        text_sys_init();
        rpg_bat_unit0.disp = rpg_bat_kanji_set(177, 179, rpg_bat_win_seq_str1) - 1;
        rpg_bat_unit2.disp = rpg_bat_kanji_set(177, 195, date_name_set_tbl0[rpg_bat_girl]) - 1;
        rpg_bat_unit3.disp = rpg_bat_kanji_set(19, 187, rpg_bat_win_seq_str2) - 1;
        k_disp_start(rpg_bat_unit3.disp + 1);
        k_disp_switch(rpg_bat_unit0.disp, 1);
        k_disp_switch(rpg_bat_unit2.disp, 1);
        k_disp_switch(rpg_bat_unit3.disp, 1);
        k_sub_reset_point_set();
        rpg_bat_se_set(1301, 1, 0);
        rpg_bat_seq_next();
        return;
    case 6:
        banchou_bye_talk();
        return;
    case 7:
        cw155_disp = cw155_disp | 1;
        cw155_x = cw155_x - 16;
        rpg_bat_unit3.cnt = rpg_bat_unit3.cnt + 1;
        if (rpg_bat_unit3.cnt >= 33) {
            rpg_bat_seq_next();
        }
        return;
    case 8:
        rpg_bat_seq_wait3(30);
        return;
    case 9:
        rpg_bat_anim_set(48, 9, 1, 1);
        switch ((u32)rpg_bat_girl) {
        case 0:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 1:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 2:
            rpg_bat_anim_set(29, 2, 1, 1);
            break;
        case 3:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 4:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 5:
            rpg_bat_anim_set(29, 2, 1, 1);
            break;
        case 6:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 7:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 8:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 9:
            rpg_bat_anim_set(29, 7, 1, 1);
            break;
        case 10:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        case 13:
            rpg_bat_anim_set(29, 4, 1, 1);
            break;
        }
        rpg_bat_seq_next();
        return;
    case 10:
        strcpy(rpg_bat_msg_buf4, rpg_bat_win_seq_str3);
        strcpy(rpg_bat_msg4_line1, rpg_bat_win_seq_str4);
        strcpy(rpg_bat_msg4_line2, rpg_bat_win_seq_str5);
        rpg_bat_msg_build();
        rpg_bat_msg_set(rpg_bat_msg_tbl4, rpg_bat_seq_work[40], 90);
        device_port_init(1, 514);
        rpg_bat_seq_next();
        return;
    case 11:
        rpg_bat_seq_gate(0);
        return;
    case 12:
        rpg_bat_result = 0;
        sub_step_up();
        return;
    }
}
