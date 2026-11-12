/* @SymbolStart: 0x80145CC0 */
/* @RodataBase: 0x8015C704 */
/* @PostProc: patch_insn_reg_field 0x124:rt:v0:v1 0x128:rs:v0:v1 0x128:rt:v0:v1 0x130:rs:v0:v1 0x130:rt:v0:v1 0x134:rt:v0:v1 0x138:rt:v0:v1 0x148:rt:v0:v1 0x160:rt:v0:v1 0x164:rs:v0:v1 0x164:rt:v0:v1 0x16c:rs:v0:v1 0x360:rt:v0:v1 0x364:rs:v0:v1 0x364:rt:v0:v1 0x36c:rs:v0:v1 0x36c:rt:v0:v1 0x370:rt:v0:v1 0x374:rs:v0:v1 0x37c:rs:v0:v1 0x388:rt:v0:v1 0x414:rt:v0:v1 0x418:rs:v0:v1 0x418:rt:v0:v1 0x420:rs:v0:v1 0x428:rs:v0:v1 0x434:rt:v0:v1 0x468:rt:v0:v1 0x46c:rs:v0:v1 0x46c:rt:v0:v1 0x474:rs:v0:v1 0x574:rt:v0:v1 0x578:rs:v0:v1 0x578:rt:v0:v1 0x580:rs:v0:v1 0x580:rt:v0:v1 0x584:rt:v0:v1 0x590:rt:v0:v1 0x5a0:rt:v0:v1 0x5a4:rs:v0:v1 0x5a4:rt:v0:v1 0x5ac:rs:v0:v1 0x5b4:rt:v0:v1 0x5bc:rs:v0:v1 0x5c4:rt:v0:v1 0x5cc:rs:v0:v1 0x5d4:rt:v0:v1 0x5dc:rs:v0:v1 0x5fc:rt:v0:v1 0x600:rs:v0:v1 0x600:rt:v0:v1 0x608:rs:v0:v1 0x678:rt:v0:v1 0x67c:rs:v0:v1 0x67c:rt:v0:v1 0x684:rs:v0:v1 0x684:rt:v0:v1 0x688:rt:v0:v1 0x690:rt:v0:v1 0x6a0:rt:v0:v1 0x6a4:rs:v0:v1 0x6a4:rt:v0:v1 0x6ac:rs:v0:v1 0x6b4:rt:v0:v1 0x6bc:rs:v0:v1 0x6c4:rt:v0:v1 0x6cc:rs:v0:v1 0x6d4:rt:v0:v1 0x6dc:rs:v0:v1 0x6fc:rt:v0:v1 0x700:rs:v0:v1 0x700:rt:v0:v1 0x708:rs:v0:v1 */
/* @SymbolSize: 2232 */
/* mongoose_seq -- RPG_BAT overlay, Address: 0x80145CC8, */
/* Size: 2224 bytes (556 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 sub_step_up(void);
extern void device_port_init(s32, s32);
extern void k_disp_switch(s32, s32);
extern void strcpy(char *, char *);
extern void strcat(char *, char *);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern s32 rpg_bat_rec_flag_check(s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_sprite_bri_set(s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_sprite_spread(void);
extern void rpg_bat_msg_set(char *, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_effect_clear(void);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_pick_list_set(void);

extern s32 rpg_bat_girl;

extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_buf2_1[];
extern char rpg_bat_msg_tbl2[];
extern char rpg_bat_msg_buf4[];
extern char rpg_bat_msg4_line1[];
extern char rpg_bat_msg4_line2[];
extern char rpg_bat_msg4_line3[];
extern char rpg_bat_msg_tbl4[];
extern char mongoose_seq_str0[];
extern char mongoose_seq_str1[];
extern char mongoose_seq_str2[];
extern char mongoose_seq_str3[];
extern char mongoose_seq_str4[];
extern char mongoose_seq_str5[];
extern char mongoose_seq_str6[];
extern char mongoose_seq_str7[];
extern char mongoose_seq_str8[];
extern char mongoose_seq_str9[];
extern char p_sei[];
extern CLASS_W class_w[];
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit3;

void mongoose_seq(void)
{
    switch (rpg_bat_unit1.step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, mongoose_seq_str0);
        rpg_bat_sprite_pos_set(56, 0, 0);
        class_w[152].prio = 4;
        rpg_bat_anim_set(56, 1, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_seq_wait(60);
        break;
    case 2:
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_anim_set(56, 255, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 3:
        rpg_bat_seq_wait(20);
        break;
    case 4:
        rpg_bat_anim_set(56, 2, 5, 1);
        rpg_bat_se_set(1287, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 5:
        rpg_bat_unit1.cnt += 3;
        rpg_bat_sprite_pos_set(56, (s32)((u32)rpg_bat_unit1.cnt * 144U) / 100, 0);
        if (rpg_bat_unit1.cnt < 101) {
            break;
        }
        rpg_bat_se_set(1286, 1, 0);
        rpg_bat_sprite_pos_set(56, 144, 0);
        rpg_bat_anim_set(56, 3, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 6:
        rpg_bat_seq_wait(10);
        break;
    case 7:
        rpg_bat_slot_adjust(0, 33, rpg_bat_unit0.hp - 3, 1);
        rpg_bat_anim_set(56, 255, 1, 0);
        rpg_bat_anim_set(48, 4, 1, 1);
        rpg_bat_unit0.flag = 0x1000;
        rpg_bat_seq_step_next();
        break;
    case 8:
        rpg_bat_seq_wait(150);
        break;
    case 9:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, mongoose_seq_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 120);
        rpg_bat_seq_step_next();
        break;
    case 10:
        rpg_bat_seq_step_gate(1);
        break;
    case 11:
        strcpy(rpg_bat_msg_buf2, mongoose_seq_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_seq_step_next();
        break;
    case 12:
        rpg_bat_seq_wait(10);
        break;
    case 13:
        rpg_bat_se_set(1291, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 14:
        rpg_bat_seq_step_gate(1);
        break;
    case 15:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, mongoose_seq_str3);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_seq_step_next();
        break;
    case 16:
        rpg_bat_seq_step_gate(1);
        break;
    case 17:
        rpg_bat_unit1.cnt += 1;
        if (rpg_bat_unit1.cnt < 61) {
            rpg_bat_unit1.cnt = rpg_bat_unit1.cnt;
            if ((rpg_bat_unit1.cnt & 1) == 0) {
                rpg_bat_anim_set(48, 255, 1, 0);
                if (rpg_bat_girl != 14) {
                    rpg_bat_anim_set(29, 255, 1, 0);
                }
            } else {
                rpg_bat_anim_set(48, 255, 1, 1);
                if (rpg_bat_girl != 14) {
                    rpg_bat_anim_set(29, 255, 1, 1);
                }
            }
        }
        rpg_bat_sprite_pos_set(40, 0, 0);
        if (rpg_bat_unit1.cnt >= 61) {
            rpg_bat_unit1.cnt = rpg_bat_unit1.cnt;
            if ((rpg_bat_unit1.cnt & 1) == 0) {
                rpg_bat_anim_set(40, 0, 1, 1);
            } else {
                rpg_bat_anim_set(40, 0, 1, 0);
            }
        }
        if (rpg_bat_unit1.cnt < 120) {
            break;
        }
        rpg_bat_seq_step_next();
        break;
    case 18:
        strcpy(rpg_bat_msg_buf2, mongoose_seq_str4);
        strcpy(rpg_bat_msg_buf2_1, mongoose_seq_str5);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 2, 90);
        rpg_bat_unit3.hp ^= 0x10000000;
        rpg_bat_seq_step_next();
        break;
    case 19:
        rpg_bat_seq_step_gate(1);
        break;
    case 20:
        rpg_bat_anim_set(40, 1, 5, 1);
        rpg_bat_se_set(1289, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 21:
        if (rpg_bat_rec_flag_check(40) == 0) {
            break;
        }
        rpg_bat_anim_set(40, 2, 1, 1);
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 22:
        rpg_bat_unit1.cnt += 1;
        rpg_bat_sprite_pos_set(40, -rpg_bat_unit1.cnt * 8, 0);
        if (rpg_bat_unit1.cnt == 19 ||
            (rpg_bat_unit1.cnt = rpg_bat_unit1.cnt, rpg_bat_unit1.cnt == 22) ||
            (rpg_bat_unit1.cnt = rpg_bat_unit1.cnt, rpg_bat_unit1.cnt == 25) ||
            (rpg_bat_unit1.cnt = rpg_bat_unit1.cnt, rpg_bat_unit1.cnt == 28)) {
            rpg_bat_sprite_bri_set(255);
        } else {
            rpg_bat_sprite_bri_set(128);
        }
        if (rpg_bat_unit1.cnt < 38) {
            break;
        }
        rpg_bat_seq_step_next();
        break;
    case 23:
        rpg_bat_seq_wait(30);
        break;
    case 24:
        rpg_bat_sprite_pos_set(40, -192, -160);
        rpg_bat_anim_set(40, 3, 1, 1);
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 25:
        rpg_bat_unit1.cnt += 1;
        rpg_bat_sprite_pos_set(40, -192, rpg_bat_unit1.cnt * 8 - 160);
        if (rpg_bat_unit1.cnt == 13 ||
            (rpg_bat_unit1.cnt = rpg_bat_unit1.cnt, rpg_bat_unit1.cnt == 16) ||
            (rpg_bat_unit1.cnt = rpg_bat_unit1.cnt, rpg_bat_unit1.cnt == 19) ||
            (rpg_bat_unit1.cnt = rpg_bat_unit1.cnt, rpg_bat_unit1.cnt == 22)) {
            rpg_bat_sprite_bri_set(255);
        } else {
            rpg_bat_sprite_bri_set(128);
        }
        if (rpg_bat_unit1.cnt < 31) {
            break;
        }
        rpg_bat_seq_step_next();
        break;
    case 26:
        rpg_bat_seq_wait(60);
        break;
    case 27:
        rpg_bat_sprite_spread();
        break;
    case 28:
        rpg_bat_seq_wait(30);
        break;
    case 29:
        rpg_bat_anim_set(48, 0, 1, 1);
        if (rpg_bat_girl != 14) {
            rpg_bat_anim_set(29, 0, 1, 1);
        }
        k_disp_switch(rpg_bat_unit1.disp, 0);
        strcpy(rpg_bat_msg_buf4, mongoose_seq_str6);
        strcpy(rpg_bat_msg4_line1, mongoose_seq_str7);
        strcpy(rpg_bat_msg4_line2, mongoose_seq_str8);
        strcpy(rpg_bat_msg4_line3, mongoose_seq_str9);
        rpg_bat_pick_list_set();
        rpg_bat_seq_step_next();
        break;
    case 30:
        rpg_bat_seq_wait(60);
        break;
    case 31:
        device_port_init(1, 514);
        rpg_bat_seq_step_next();
        break;
    case 32:
        rpg_bat_msg_set(rpg_bat_msg_tbl4, rpg_bat_seq_work[40], 90);
        rpg_bat_anim_set(48, 9, 1, 1);
        if (rpg_bat_girl != 14) {
            rpg_bat_anim_set(29, 4, 1, 1);
        }
        rpg_bat_seq_step_next();
        break;
    case 33:
        rpg_bat_seq_step_gate(0);
        break;
    case 34:
        sub_step_up();
        break;
    default:
        rpg_bat_effect_clear();
        break;
    }
}
