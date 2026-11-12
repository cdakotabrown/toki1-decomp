/* @RodataBase: 0x8015B804 */
/* @SymbolSize: 560 */
/* rpg_bat_attack_step - RPG_BAT overlay, Address: 0x80137C44, Size: 560 bytes (140 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set();
extern s32 strcpy();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_talk_wait();
extern s32 rpg_bat_talk_clear();
extern s32 rpg_bat_seq_next();

extern u8 rpg_bat_attack_step_str0[];
extern u8 rpg_bat_attack_step_str1[];
extern u8 rpg_bat_attack_step_str2[];
extern u8 rpg_bat_msg_buf4[];
extern u8 rpg_bat_msg4_line1[];
extern u8 rpg_bat_msg4_line2[];
extern u8 rpg_bat_msg_tbl4[];
extern s32 rpg_bat_talk_work_step;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_attack_step(void)
{
    s32 v0;

    if (rpg_bat_rec_flag_check(48) != 0) {
        rpg_bat_anim_set(48, 0, 1, 1);
    }

    if (rpg_bat_talk_work_step >= 4) {
        if (rpg_bat_unit0.hp != rpg_bat_unit0.hp_max) {
            rpg_bat_se_set(1284, 2, 5);
        }
        v0 = rpg_bat_unit0.hp_max;
        rpg_bat_unit0.hp = rpg_bat_unit0.hp + 9;
        if (v0 < rpg_bat_unit0.hp) {
            rpg_bat_unit0.hp = v0;
        }
    }

    v0 = rpg_bat_unit0.hp_max;

    switch ((u32)rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf4, rpg_bat_attack_step_str0);
        strcpy(rpg_bat_msg4_line1, rpg_bat_attack_step_str1);
        strcpy(rpg_bat_msg4_line2, rpg_bat_attack_step_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 3, 240);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        return;
    case 1:
        rpg_bat_koe_set(0x05000028, 0x05000035, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        return;
    case 2:
        rpg_bat_talk_wait(1);
        return;
    case 3:
        rpg_bat_koe_set(0x06000003, 0x0400000C, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        return;
    case 4:
        rpg_bat_talk_wait(2);
        return;
    case 5:
        rpg_bat_koe_set(0x0600000D, 0x05000017, 2);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        return;
    case 6:
        rpg_bat_talk_wait(0);
        return;
    case 7:
        if (rpg_bat_unit0.hp != v0) {
            return;
        }
        rpg_bat_talk_clear();
        rpg_bat_anim_set(48, 0, 1, 1);
        rpg_bat_seq_next();
        return;
    }
}
