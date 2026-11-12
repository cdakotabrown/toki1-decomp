/* @SymbolStart: 0x80138B9C */
/* @RodataBase: 0x8015B8CC */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1e0 */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1dc --offset2 0x1e8 */
/* @PostProc: patch_insn_reg_field 0x1dc:rd:t1:t0 0x1e4:rt:t0:t1 0x1e8:rt:t0:t1 0x1f0:rt:t1:t0 0x21c:rt:t3:v1 0x224:rt:t3:v1 0x348:rt:t4:t3 0x34c:rs:t4:t3 0x34c:rt:t4:t3 0x354:rs:t4:t3 0x364:rt:t5:t4 0x368:rs:t5:t4 0x368:rt:t5:t4 0x374:rt:t5:t4 0x3a8:rt:t6:v1 0x3ac:rs:t6:v1 0x3ac:rt:t6:v1 0x3b4:rd:t7:t5 0x3b4:rs:t6:v1 0x3c0:rd:t8:t6 0x3c0:rs:t7:t5 0x3cc:rt:t8:t6 */
/* @SymbolSize: 976 */
/* furyou_punch_stun_seq - RPG_BAT overlay, Address: 0x80138BA4, Size: 968 bytes (242 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 strcpy();
extern s32 suuzi_str();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_flag_seq();
extern s32 rpg_bat_seq_step_gate();
extern s32 rpg_bat_anim_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_effect_clear();
extern s32 rpg_bat_seq_wait();

extern u8 p_sei[];
extern u8 furyou_attack5_str0[];
extern u8 furyou_attack5_str1[];
extern u8 furyou_attack5_str2[];
extern u8 furyou_attack5_str3[];
extern u8 furyou_attack5_str4[];
extern u8 furyou_attack5_str5[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_buf2_1[];
extern u8 rpg_bat_msg_buf2_2[];
extern u8 rpg_bat_msg_tbl2[];
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_menu_ctl;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void furyou_punch_stun_seq(void)
{
    s32 v1;
    s32 t8;
    s32 sp52;

    switch ((u32)rpg_bat_unit1.step) {
    case 0:
        switch ((u32)rpg_bat_target_no) {
        case 0:
            strcpy(rpg_bat_msg_buf2, furyou_attack5_str0);
            break;
        case 1:
            strcpy(rpg_bat_msg_buf2, furyou_attack5_str1);
            break;
        case 2:
            strcpy(rpg_bat_msg_buf2, furyou_attack5_str2);
            break;
        }
        strcpy(rpg_bat_msg_buf2_1, furyou_attack5_str3);
        suuzi_str(rpg_bat_msg_buf2_2, p_sei, furyou_attack5_str4, rpg_bat_dmg,
                      furyou_attack5_str5);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 3, 60);
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 1, 5, 1);
        rpg_bat_se_set(1285, 1, 0);
        rpg_bat_seq_step_next();
        return;
    case 1:
        rpg_bat_flag_seq(rpg_bat_target_no + 59);
        return;
    case 2:
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 0, 1, 1);
        rpg_bat_seq_step_next();
        return;
    case 3:
        rpg_bat_seq_step_gate(1);
        return;
    case 4:
        rpg_bat_anim_set(48, 4, 1, 1);
        t8 = rpg_bat_unit0.flag | 0x1000;
        rpg_bat_unit0.flag = t8;
        if ((t8 << 15) < 0) {
            v1 = t8 & ~0x10000;
            rpg_bat_unit0.aux08 = 0;
            rpg_bat_menu_ctl = 4;
            rpg_bat_unit0.flag = v1;
        }
        class_w[152].prio = 3;
        rpg_bat_anim_set(56, 12, 5, 1);
        rpg_bat_seq_work = get_rand_mod(4) + 1;
        sp52 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, sp52 - 5, get_rand_mod(20) - 5);
        rpg_bat_se_set(1286, 1, 0);
        rpg_bat_seq_step_next();
        return;
    case 5:
        if (rpg_bat_rec_flag_check(56) == 0) {
            return;
        }
        rpg_bat_se_set(1286, 1, 0);
        sp52 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, sp52 - 5, get_rand_mod(20) - 5);
        rpg_bat_anim_set(56, 12, 5, 1);
        v1 = rpg_bat_seq_work - 1;
        rpg_bat_seq_work = v1;
        if (v1 != 0) {
            return;
        }
        rpg_bat_seq_step_next();
        return;
    case 6:
        if (rpg_bat_rec_flag_check(56) == 0) {
            return;
        }
        rpg_bat_anim_set(56, 255, 5, 0);
        rpg_bat_seq_step_next();
        return;
    case 7:
        rpg_bat_seq_step_gate(2);
        return;
    case 8:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        if (rpg_bat_unit0.hp == 0) {
            rpg_bat_effect_clear();
            rpg_bat_unit1.flag = 0x100007;
            return;
        }
        rpg_bat_seq_step_next();
        return;
    case 9:
        rpg_bat_seq_step_gate(0);
        return;
    case 10:
        rpg_bat_seq_wait(20);
        return;
    case 11:
        v1 = rpg_bat_unit0.flag & ~0x1000;
        rpg_bat_unit0.flag = v1 & 0x00FFFFFF;
        rpg_bat_effect_clear();
        return;
    }
}
