/* @SymbolStart: 0x80156D78 */
/* @RodataBase: 0x8015DDBC */
/* @SymbolSize: 992 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: remap_stack_slots --from 44 --to 56 */
/* @PostProc: patch_insn_reg_field 0x17c:rt:t4:v0 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x4c:rt:v0:v1 0x50:rs:v0:v1 0x50:rt:v0:v1 0x58:rs:v0:v1 0x60:rs:v0:v1 0x88:rs:v0:v1 0x90:rt:v0:v1 0x160:rt:t4:v0 0x164:rs:t4:v0 0x164:rt:t4:v0 0x180:rt:t7:t6 0x184:rt:t5:t4 0x18c:rs:t7:t6 0x18c:rt:t7:t6 0x194:rt:t6:t5 0x198:rt:t8:t7 0x19c:rt:t5:t4 0x1a0:rt:t8:t7 0x1a4:rt:t6:t5 0x1a8:rt:t7:t6 0x200:rt:t9:v0 0x204:rs:t9:v0 0x204:rt:t9:v0 0x208:rt:v0:v1 0x20c:rs:t9:v0 0x21c:rt:v0:v1 0x244:rt:t0:t8 0x248:rs:t0:t8 0x248:rt:t0:t8 0x250:rs:t0:t8 0x2a4:rt:t1:t9 0x2ac:rs:t1:t9 0x2ac:rt:t1:t9 0x2b4:rs:t1:t9 0x2c0:rt:t2:t0 0x2c8:rt:t2:t0 0x2d0:rt:t3:t1 0x2d4:rs:t3:t1 0x2d4:rt:t3:t1 0x2f0:rt:t3:t1 0x354:rt:t4:t2 0x358:rt:at:v0 0x35c:rt:t5:at 0x360:rs:t5:v0 0x360:rt:t5:v0 0x364:rt:t4:t2 0x36c:rd:t6:t3 0x36c:rs:t5:v0 0x370:rs:t6:t3 0x370:rt:t7:t4 0x378:rt:t7:t4 */
/* utyuuzin_attack - RPG_BAT overlay, Address: 0x80156D80, Size: 984 bytes (246 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 dtd_on_tpage();
extern s32 _sprite_set_box_shade();
extern s32 strcpy();
extern s32 rpg_bat_player_pose_set();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_damage_set();
extern s32 rpg_bat_msg_set();
extern s32 suuzi_str();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_effect_clear();
extern s32 rpg_bat_seq_wait();
extern s32 rpg_bat_flag_seq();
extern s32 rpg_bat_seq_step_gate();

extern u8 p_sei[];
extern u8 utyuuzin_attack_str0[];
extern u8 utyuuzin_attack_str1[];
extern u8 utyuuzin_attack_str2[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_tbl2[];
extern s32 rpg_bat_dmg;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void utyuuzin_attack(void)
{
    s32 v1;
    s32 v0;
    s32 sp56;

    v1 = rpg_bat_unit1.step;
    if (v1 < 80) {
        if (rpg_bat_unit0.flag == 0x02000000) {
            rpg_bat_unit0.flag = 256;
        }
        rpg_bat_player_pose_set();
        v1 = rpg_bat_unit1.step;
    }

    switch (v1) {
    case 0:
        strcpy(rpg_bat_msg_buf2, utyuuzin_attack_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_unit0.flag = rpg_bat_unit0.flag | 0x1000;
        rpg_bat_se_set(1300, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_seq_step_gate(0);
        break;
    case 2:
        rpg_bat_anim_set(59, 1, 5, 1);
        rpg_bat_unit0.flag = rpg_bat_unit0.flag & ~0x1000;
        rpg_bat_seq_step_next();
        break;
    case 3:
        rpg_bat_flag_seq(59);
        break;
    case 4:
        sp56 = rpg_bat_unit1.cnt;
        rpg_bat_anim_set(59, 0, 1, 1);
        class_w[144].prio = 6;
        _sprite_set_box_shade(-38, -26, sp56, 2, 5, 0x00BFBF00, 129);
        dtd_on_tpage(0, 0, 5, 1, 0);
        v0 = rpg_bat_unit1.cnt + 2;
        rpg_bat_unit1.cnt = v0;
        if (v0 < 114) {
            rpg_bat_se_set(1284, 36, 1);
        }
        if (rpg_bat_unit1.cnt >= 151) {
            rpg_bat_unit1.step = 80;
        }
        break;
    case 80:
        rpg_bat_sprite_pos_set(40, 272, 88);
        rpg_bat_anim_set2(40, 5, 5, 1);
        if (rpg_bat_unit0.flag != 256) {
            rpg_bat_anim_set(48, 5, 1, 1);
        }
        rpg_bat_se_set(1288, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 81:
        rpg_bat_flag_seq(40);
        break;
    case 82:
        rpg_bat_damage_set();
        if (rpg_bat_dmg >= rpg_bat_unit0.hp) {
            rpg_bat_dmg = rpg_bat_unit0.hp - 1;
        }
        suuzi_str(rpg_bat_msg_buf2, p_sei, utyuuzin_attack_str1, rpg_bat_dmg,
                      utyuuzin_attack_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_anim_set2(40, 255, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 83:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        if (rpg_bat_unit0.hp == 1) {
            rpg_bat_unit0.flag = 4096;
            rpg_bat_unit1.flag = (rpg_bat_unit1.flag & 0xFFFF0000) | 3;
            rpg_bat_unit1.step = 0;
            rpg_bat_anim_set(48, 4, 1, 1);
            rpg_bat_seq_step_next();
            break;
        }
        rpg_bat_seq_step_next();
        break;
    case 84:
        rpg_bat_seq_step_gate(0);
        break;
    case 85:
        rpg_bat_seq_wait(20);
        break;
    case 86:
        rpg_bat_effect_clear();
        break;
    }
}
