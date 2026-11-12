/* @SymbolStart: 0x80154430 */
/* @RodataBase: 0x8015DAA8 */
/* @SymbolSize: 492 */
/* @PostProc: patch_insn_reg_field 0xd8:rt:v0:v1 0xdc:rs:v0:v1 0xdc:rt:v0:v1 0xe4:rs:v0:v1 0xf0:rs:v0:v1 0xfc:rs:v0:v1 0x108:rs:v0:v1 */
/* rpg_bat_bukatu4_attack - RPG_BAT overlay, Address: 0x80154438, Size: 484 bytes (121 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 strcpy();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_pos1_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_timer_step();
extern s32 rpg_bat_rec_step();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_pair_sway_step();
extern s32 rpg_bat_sanshoku_step();
extern s32 rpg_bat_fade_step();

extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_tbl[];
extern u8 rpg_bat_bukatu4_attack_str0[];
extern s32 rpg_bat_target_no;
extern s16 cw148_frame;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_bukatu4_attack(void)
{
    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_bukatu4_attack_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_step_next();
        break;
    case 4:
        switch (cw148_frame) {
        case 1:
            rpg_bat_se_set2(1289, 0, 2);
            break;
        case 3:
            rpg_bat_se_set2(1289, 0, 3);
            break;
        case 5:
            rpg_bat_se_set2(1289, 0, 4);
            break;
        case 7:
            rpg_bat_se_set2(1289, 0, 5);
            break;
        }
        rpg_bat_rec_step(52);
        break;
    case 5:
        rpg_bat_anim_set(52, 4, 1, 1);
        rpg_bat_step_next();
        break;
    case 6:
        rpg_bat_sanshoku_step();
        break;
    case 7:
        rpg_bat_pair_sway_step();
        break;
    case 8:
        rpg_bat_fade_step();
        break;
    case 9:
        rpg_bat_timer_step(20);
        break;
    case 10:
        rpg_bat_bukatu_exit();
        break;
    case 11:
        rpg_bat_clear();
        break;
    }
}
