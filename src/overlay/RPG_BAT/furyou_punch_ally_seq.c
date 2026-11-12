/* @SymbolStart: 0x80138324 */
/* @RodataBase: 0x8015B86C */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --from 36 --to 48 */
/* @PostProc: patch_insn_reg_field 0x230:rt:t3:t1 0x238:rd:t1:t9 0x23c:rd:t2:t9 0x23c:rt:t1:t9 0x240:rs:t3:t1 0x240:rt:t3:t1 0x244:rs:t2:t9 0x244:rt:t3:t1 0x248:rt:t4:t2 0x250:rs:t4:t2 0x250:rt:t5:t3 0x258:rt:t5:t3 0x2a0:rt:t9:t5 0x2a4:rd:t6:t4 0x2a8:rd:t7:t4 0x2a8:rs:t6:t4 0x2ac:rd:t8:t4 0x2ac:rt:t7:t4 0x2b0:rs:t9:t5 0x2b0:rt:t9:t5 0x2b4:rs:t8:t4 0x2b4:rt:t9:t5 0x2b8:rt:t1:t6 0x2c0:rs:t1:t6 0x2c0:rt:t2:t7 0x2c8:rt:t2:t7 0x30c:rt:t3:t8 0x314:rt:t3:t8 0x334:rt:t4:v1 0x33c:rt:t4:v1 0x354:rt:t6:t1 0x358:rt:t7:t2 0x35c:rs:t7:t2 0x35c:rt:t7:t2 0x360:rs:t6:t1 0x360:rt:t6:t1 0x364:rt:t5:t9 0x368:rt:t7:t2 0x36c:rs:t5:t9 0x36c:rt:t6:t1 0x3d0:rt:t9:t4 0x3d4:rt:t1:t5 0x3d8:rs:t1:t5 0x3d8:rt:t1:t5 0x3dc:rs:t9:t4 0x3dc:rt:t9:t4 0x3e0:rt:t8:t3 0x3e4:rt:t1:t5 0x3e8:rs:t8:t3 0x3e8:rt:t9:t4 */
/* @SymbolSize: 1228 */
/* furyou_punch_ally_seq - RPG_BAT overlay, Address: 0x8013832C, Size: 1220 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 strcpy();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_pos1_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_msg_set();
extern s32 suuzi_str();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_effect_clear();
extern s32 rpg_bat_seq_wait();
extern s32 rpg_bat_flag_seq();
extern s32 rpg_bat_seq_step_gate();

extern u8 furyou_attack4_str0[];
extern u8 furyou_attack4_str1[];
extern u8 furyou_attack4_str2[];
extern u8 furyou_attack4_str3[];
extern u8 furyou_attack4_str4[];
extern u8 furyou_attack4_str5[];
extern u8 furyou_attack4_str6[];
extern u8 furyou_attack4_str7[];
extern u8 furyou_attack4_str8[];
extern u8 furyou_attack4_str9[];
extern u8 furyou_attack4_str10[];
extern u8 furyou_attack4_str11[];
extern u8 furyou_attack4_str12[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_buf2_1[];
extern u8 rpg_bat_msg_buf2_2[];
extern u8 rpg_bat_msg_tbl2[];
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_seq_wait_cnt;
extern u8 class_w[];
extern RPG_BAT_UNIT rpg_bat_unit1;

void furyou_punch_ally_seq(void)
{
    s32 v1;
    s32 i;
    s32 sp48;

    switch ((u32)rpg_bat_unit1.step) {
    case 0:
        switch ((u32)rpg_bat_target_no) {
        case 0:
            strcpy(rpg_bat_msg_buf2, furyou_attack4_str0);
            rpg_bat_seq_wait_cnt = get_rand_mod(2) + rpg_bat_target_no + 1;
            break;
        case 1:
            strcpy(rpg_bat_msg_buf2, furyou_attack4_str1);
            rpg_bat_seq_wait_cnt = (get_rand_mod(2) << 1) + rpg_bat_target_no - 1;
            break;
        case 2:
            strcpy(rpg_bat_msg_buf2, furyou_attack4_str2);
            rpg_bat_seq_wait_cnt = rpg_bat_target_no - get_rand_mod(2) - 1;
            break;
        }
        strcpy(rpg_bat_msg_buf2_1, furyou_attack4_str3);
        switch ((u32)rpg_bat_seq_wait_cnt) {
        case 0:
            suuzi_str(rpg_bat_msg_buf2_2, furyou_attack4_str4, furyou_attack4_str5, rpg_bat_dmg,
                          furyou_attack4_str6);
            break;
        case 1:
            suuzi_str(rpg_bat_msg_buf2_2, furyou_attack4_str7, furyou_attack4_str8, rpg_bat_dmg,
                          furyou_attack4_str9);
            break;
        case 2:
            suuzi_str(rpg_bat_msg_buf2_2, furyou_attack4_str10, furyou_attack4_str11, rpg_bat_dmg,
                          furyou_attack4_str12);
            break;
        }
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 3, 60);
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 1, 5, 1);
        rpg_bat_se_set(1285, 1, 0);
        i = ((rpg_bat_target_no << 4) + rpg_bat_target_no) << 2;
        class_w[i + 10543] |= 1;
        rpg_bat_seq_step_next();
        return;
    case 1:
        rpg_bat_flag_seq(rpg_bat_target_no + 59);
        return;
    case 2:
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 0, 1, 1);
        i = ((rpg_bat_target_no << 4) + rpg_bat_target_no) << 2;
        class_w[i + 10543] |= 1;
        rpg_bat_seq_step_next();
        return;
    case 3:
        rpg_bat_seq_step_gate(1);
        return;
    case 4:
        rpg_bat_pos1_set(rpg_bat_seq_wait_cnt);
        rpg_bat_sprite_pos_set(56, rpg_bat_target_x, rpg_bat_target_y);
        ((CLASS_W *)class_w)[152].prio = 3;
        rpg_bat_anim_set2(56, 12, 5, 1);
        rpg_bat_seq_work = get_rand_mod(4) + 1;
        sp48 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, sp48 + rpg_bat_target_x - 10,
                      get_rand_mod(20) + rpg_bat_target_y - 10);
        rpg_bat_se_set(1286, 1, 0);
        rpg_bat_seq_step_next();
        return;
    case 5:
        if (rpg_bat_rec_flag_check(56) == 0) {
            return;
        }
        rpg_bat_se_set(1286, 1, 0);
        sp48 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, sp48 + rpg_bat_target_x - 10,
                      get_rand_mod(20) + rpg_bat_target_y - 10);
        rpg_bat_anim_set2(56, 12, 5, 1);
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
        rpg_bat_anim_set2(56, 255, 5, 0);
        rpg_bat_seq_step_next();
        return;
    case 7:
        rpg_bat_seq_step_gate(2);
        return;
    case 8:
        rpg_bat_slot_adjust(0, rpg_bat_seq_wait_cnt, rpg_bat_dmg, 1);
        rpg_bat_seq_step_next();
        return;
    case 9:
        rpg_bat_seq_step_gate(0);
        return;
    case 10:
        rpg_bat_seq_wait(20);
        return;
    case 11:
        rpg_bat_effect_clear();
        return;
    }
}
