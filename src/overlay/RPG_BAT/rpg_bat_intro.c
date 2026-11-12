/* @RodataBase: 0x8015ABB8 */
/* Address: 0x80133108 */
/* Size: 4068 bytes (1017 insns) */
/* @SymbolSize: 4068 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x5f4 --offset2 0x5fc */
/* @PostProc: swap_instructions --offset1 0x614 --offset2 0x618 */
/* @PostProc: patch_insn_reg_field 0x6b4:rt:v0:v1 0x6bc:rt:v0:v1 0x6c4:rs:v0:v1 0x6d4:rs:v0:v1 0x6ec:rd:v1:a0 0x6f0:rs:v1:a0 0x700:rt:v0:v1 0x710:rt:v0:v1 0x71c:rs:v0:v1 0x72c:rs:v0:v1 0x77c:rs:v1:s2 0x77c:rt:s2:v1 0x790:rs:v1:v0 0x790:rt:v0:v1 0x830:rt:v0:v1 0x838:rt:v0:v1 0x840:rs:v0:v1 0x850:rs:v0:v1 0x868:rd:v1:a0 0x86c:rs:v1:a0 0x87c:rt:v0:v1 0x88c:rt:v0:v1 0x898:rs:v0:v1 0x8a8:rs:v0:v1 0x8f0:rt:a3:t0 0x8f4:rs:a3:t0 0x8f8:rs:v1:s2 0x8f8:rt:s2:v1 0x8fc:rt:t0:a3 0x910:rt:t0:a3 0x920:rt:a3:t0 0x924:rs:a3:t0 0x934:rs:a3:t0 0x954:rs:v1:v0 0x954:rt:v0:v1 0x964:rt:t0:a3 0x974:rt:a3:t0 0x978:rs:a3:t0 0x988:rs:a3:t0 0xa38:rt:v0:v1 0xa40:rt:v0:v1 0xa48:rs:v0:v1 0xa58:rs:v0:v1 0xa70:rd:v1:a0 0xa74:rs:v1:a0 0xa84:rt:v0:v1 0xa94:rt:v0:v1 0xaa0:rs:v0:v1 0xab0:rs:v0:v1 0xb00:rs:v1:s2 0xb00:rt:s2:v1 0xb14:rs:v1:v0 0xb14:rt:v0:v1 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"
extern u8 class_w[];
extern u8 cw32_rec[];

extern s32 sub_step_up(void);
extern void device_port_init(s32, s32);
extern void k_disp_switch(s32, s32);
extern u32 rsin(s32);
extern u32 rcos(s32);
extern void strcpy(char *, char *);
extern void strcat(char *, char *);
extern void rpg_bat_matagi_talk_step(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern s32 rpg_bat_rec_flag_check(s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern s32 get_rand_mod(s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_msg_set(char *, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_effect_clear(void);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_pick_list_set(void);

extern s32 rpg_bat_girl;

extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_buf2_1[];
extern char rpg_bat_msg_buf2_2[];
extern char rpg_bat_msg_tbl2[];
extern char rpg_bat_msg_buf4[];
extern char rpg_bat_msg4_line1[];
extern char rpg_bat_msg4_line2[];
extern char rpg_bat_msg4_line3[];
extern char rpg_bat_msg_tbl4[];
extern char rpg_bat_intro_str0[];
extern char rpg_bat_intro_str1[];
extern char rpg_bat_intro_str2[];
extern char rpg_bat_intro_str3[];
extern char rpg_bat_intro_str4[];
extern char rpg_bat_intro_str5[];
extern char rpg_bat_intro_str6[];
extern char rpg_bat_intro_str7[];
extern char rpg_bat_intro_str8[];
extern char rpg_bat_intro_str9[];
extern char p_sei[];
extern s32 rpg_bat_seq_work[];
extern s32 rpg_bat_res_hdr[];
extern s32 rpg_bat_res_img[];
extern s32 rpg_bat_res_anim[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_intro(void)
{
    s32 frame;
    s32 i;
    s32 id;
    s32 base;
    u32 dx;
    u32 dy;
    s32 rem;
    u8 *rec;
    s32 w0;
    s32 w1;
    s32 w2;

    switch (rpg_bat_unit1.step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, rpg_bat_intro_str0);
        rpg_bat_sprite_pos_set(56, 0, 0);
        rpg_bat_anim_set(56, 1, 5, 1);
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 1:
        if (rpg_bat_rec_flag_check(56) != 0) {
            rpg_bat_anim_set(56, 1, 1, 0);
            rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
            rpg_bat_seq_step_next();
        }
        break;
    case 2:
        rpg_bat_seq_step_gate(1);
        break;
    case 3:
        ((CLASS_W *)class_w)[152].prio = 4;
        rpg_bat_sprite_pos_set(56, 0, 0);
        rpg_bat_anim_set(56, 2, 1, 1);
        rpg_bat_anim_set2(59, 4, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 4:
        rpg_bat_unit1.cnt += 2;
        rpg_bat_sprite_pos_set(56, (s32)((u32)rpg_bat_unit1.cnt * 224U) / 100,
                      (rpg_bat_unit1.cnt * -64) / 100);
        if (((CLASS_W *)class_w)[152].frame == 0) {
            rpg_bat_se_set(1286, 1, 0);
        }
        if (rpg_bat_unit1.cnt >= 101) {
            rpg_bat_sprite_pos_set(56, 224, 64);
            rpg_bat_seq_step_next();
        }
        if (rpg_bat_unit1.cnt == 70) {
            rpg_bat_se_set(1291, 1, 6);
            rpg_bat_unit0.flag = 0x1000;
            rpg_bat_anim_set(48, 10, 1, 1);
        }
        break;
    case 5:
        rpg_bat_unit1.cnt += 3;
        rpg_bat_sprite_pos_set(56, 224 - ((s32)((u32)rpg_bat_unit1.cnt * 224U) / 100),
                      ((rpg_bat_unit1.cnt << 6) / 100) - 64);
        frame = rpg_bat_unit1.cnt;
        if (frame < 80) {
            rpg_bat_unit1.cnt = frame;
            if (((CLASS_W *)class_w)[152].frame == 0) {
                rpg_bat_se_set(1286, 1, 0);
            }
        }
        if (rpg_bat_unit1.cnt >= 101) {
            rpg_bat_sprite_pos_set(56, 0, 0);
            rpg_bat_anim_set(56, 2, 1, 0);
            rpg_bat_anim_set2(59, 0, 1, 1);
            rpg_bat_seq_step_next();
        }
        if (rpg_bat_unit1.cnt == 51) {
            rpg_bat_anim_set(48, 4, 1, 1);
            rpg_bat_slot_adjust(0, 33, rpg_bat_unit0.hp - 3, 1);
        }
        break;
    case 6:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, rpg_bat_intro_str1);
        strcpy(rpg_bat_msg_buf2_1, rpg_bat_intro_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 2, 120);
        rpg_bat_seq_step_next();
        break;
    case 7:
        rpg_bat_seq_step_gate(0);
        break;
    case 8:
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
        rpg_bat_unit3.hp ^= 0x10000000;
        strcpy(rpg_bat_msg_buf2, rpg_bat_intro_str3);
        strcpy(rpg_bat_msg_buf2_1, rpg_bat_intro_str4);
        strcpy(rpg_bat_msg_buf2_2, rpg_bat_intro_str5);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 3, 120);
        rpg_bat_seq_step_next();
        break;
    case 9:
        w0 = rpg_bat_res_img[2];
        w1 = rpg_bat_res_anim[2];
        w2 = rpg_bat_res_hdr[2];
        rec = (void *)class_w;
        do {
            rec += 272;
            *(s32 *)(rec + 4160) = w0;
            *(s32 *)(rec + 4164) = w1;
            *(s32 *)(rec + 4200) = w2;
            *(s32 *)(rec + 4204) = 0x41000000;
            *(u8 *)(rec + 4215) = 24;
            *(s32 *)(rec + 4228) = w0;
            *(s32 *)(rec + 4232) = w1;
            *(s32 *)(rec + 4268) = w2;
            *(s32 *)(rec + 4272) = 0x41000000;
            *(u8 *)(rec + 4283) = 24;
            *(s32 *)(rec + 4296) = w0;
            *(s32 *)(rec + 4300) = w1;
            *(s32 *)(rec + 4336) = w2;
            *(s32 *)(rec + 4340) = 0x41000000;
            *(u8 *)(rec + 4351) = 24;
            *(s32 *)(rec + 4092) = w0;
            *(s32 *)(rec + 4096) = w1;
            *(s32 *)(rec + 4132) = w2;
            *(s32 *)(rec + 4136) = 0x41000000;
            *(u8 *)(rec + 4147) = 24;
        } while (rec != (void *)cw32_rec);
        rpg_bat_seq_step_next();
        break;
    case 10:
        rpg_bat_seq_step_gate(2);
        break;
    case 11:
        rpg_bat_se_set(1287, 0, 1);
        base = (rpg_bat_unit1.cnt << 10) / 60;
        if (base) {}
        i = 0;
        do {
            id = i - 32;
            rpg_bat_anim_set(id, ((rpg_bat_unit1.cnt * 6) / 60) + 1, 1, 1);
            dx = rsin(base);
            dy = rcos(base);
            rem = 60 - rpg_bat_unit1.cnt;
            rpg_bat_sprite_pos_set(id, ((s32)(rem * dx) >> 13) - 16,
                          (s32)(rem * dy) >> 13);
            i += 1;
            base += 512;
        } while (i != 8);
        rpg_bat_unit1.cnt += 1;
        if (rpg_bat_unit1.cnt == 60) {
            rpg_bat_seq_step_next();
        }
        break;
    case 12:
        i = 0;
        do {
            rpg_bat_anim_set(i - 32, 255, 1, 0);
            i += 1;
        } while (i != 8);
        rpg_bat_seq_wait(15);
        break;
    case 13:
        rpg_bat_se_set(1292, 0, 2);
        base = (rpg_bat_unit1.cnt << 10) / 45;
        if (base) {}
        i = 0;
        do {
            id = i - 32;
            rpg_bat_anim_set(id, ((rpg_bat_unit1.cnt * 6) / 45) + 1, 1, 1);
            dx = rsin(base);
            dy = rcos(base);
            rem = 45 - rpg_bat_unit1.cnt;
            rpg_bat_sprite_pos_set(id,
                          ((s32)((u32)(rem * dx) * 60U) / 45 >> 13) - 16,
                          (s32)((u32)(rem * dy) * 60U) / 45 >> 13);
            i += 1;
            base += 512;
        } while (i != 8);
        rpg_bat_unit1.cnt += 1;
        if (rpg_bat_unit1.cnt == 45) {
            rpg_bat_seq_step_next();
        }
        break;
    case 14:
        i = 0;
        do {
            rpg_bat_anim_set(i - 32, 255, 1, 0);
            i += 1;
        } while (i != 8);
        rpg_bat_seq_wait(10);
        break;
    case 15:
        rpg_bat_se_set(1293, 0, 3);
        base = (rpg_bat_unit1.cnt << 10) / 30;
        if (base) {}
        i = 0;
        do {
            id = i - 32;
            rpg_bat_anim_set(id, ((rpg_bat_unit1.cnt * 6) / 30) + 1, 1, 1);
            dx = rsin(base);
            dy = rcos(base);
            rem = 30 - rpg_bat_unit1.cnt;
            rpg_bat_sprite_pos_set(id, (((s32)(rem * dx) * 2) >> 13) - 16,
                          ((s32)(rem * dy) * 2) >> 13);
            i += 1;
            base += 512;
        } while (i != 8);
        rpg_bat_unit1.cnt += 1;
        if (rpg_bat_unit1.cnt == 30) {
            rpg_bat_seq_step_next();
        }
        break;
    case 16:
        i = 0;
        do {
            rpg_bat_anim_set(i - 32, 255, 1, 0);
            i += 1;
        } while (i != 8);
        rpg_bat_seq_wait(5);
        break;
    case 17:
        i = 0;
        do {
            rpg_bat_anim_set(i - 32, 255, 1, 0);
            i += 1;
        } while (i != 8);
        rpg_bat_anim_set(40, 7, 5, 1);
        rpg_bat_se_set(1288, 1, 4);
        rpg_bat_seq_step_next();
        break;
    case 18:
        rpg_bat_seq_wait(20);
        break;
    case 19:
        if ((rpg_bat_unit1.cnt & 3) == 0) {
            dx = get_rand_mod(64);
            dy = get_rand_mod(96);
            rpg_bat_sprite_pos_set((rpg_bat_unit1.cnt / 4) - 32, (s32)dx - 224,
                          (s32)dy - 64);
            rpg_bat_anim_set((rpg_bat_unit1.cnt / 4) - 32, 8, 5, 1);
        }
        if (rpg_bat_unit1.cnt >= 17 && rpg_bat_unit1.cnt < 96) {
            rpg_bat_se_set(1289, 15, 5);
        }
        rpg_bat_unit1.cnt += 1;
        if (rpg_bat_unit1.cnt >= 129) {
            rpg_bat_seq_step_next();
        }
        break;
    case 20:
        rpg_bat_unit1.cnt += 1;
        if ((rpg_bat_unit1.cnt & 1) == 0) {
            rpg_bat_sprite_pos_set(59, 0, 0);
            rpg_bat_anim_set(59, 255, 1, 1);
            i = 0;
            do {
                rpg_bat_anim_set(i - 32, 255, 33, 1);
                i += 1;
            } while (i != 32);
        } else {
            rpg_bat_sprite_pos_set(59, 0, 0);
            rpg_bat_anim_set(59, 255, 1, 0);
            i = 0;
            do {
                rpg_bat_anim_set(i - 32, 255, 33, 0);
                i += 1;
            } while (i != 32);
        }
        if (rpg_bat_unit1.cnt >= 61) {
            k_disp_switch(rpg_bat_unit1.disp, 0);
            rpg_bat_seq_step_next();
        }
        break;
    case 21:
        rpg_bat_matagi_talk_step();
        break;
    case 22:
        rpg_bat_unit1.cnt += 1;
        if ((rpg_bat_unit1.cnt & 1) == 0) {
            rpg_bat_anim_set(40, 0, 1, 1);
        } else {
            rpg_bat_anim_set(40, 0, 1, 0);
        }
        if (rpg_bat_unit1.cnt < 91) {
            break;
        }
        rpg_bat_unit3.hp ^= 0x10000000;
        rpg_bat_anim_set(48, 0, 1, 1);
        if (rpg_bat_girl != 14) {
            rpg_bat_anim_set(29, 0, 1, 1);
        }
        rpg_bat_seq_step_next();
        break;
    case 23:
        rpg_bat_seq_wait(30);
        break;
    case 24:
        strcpy(rpg_bat_msg_buf4, rpg_bat_intro_str6);
        strcpy(rpg_bat_msg4_line1, rpg_bat_intro_str7);
        strcpy(rpg_bat_msg4_line2, rpg_bat_intro_str8);
        strcpy(rpg_bat_msg4_line3, rpg_bat_intro_str9);
        rpg_bat_pick_list_set();
        rpg_bat_seq_step_next();
        break;
    case 25:
        rpg_bat_seq_wait(60);
        break;
    case 26:
        device_port_init(1, 514);
        rpg_bat_seq_step_next();
        break;
    case 27:
        rpg_bat_msg_set(rpg_bat_msg_tbl4, rpg_bat_seq_work[40], 90);
        rpg_bat_anim_set(48, 9, 1, 1);
        if (rpg_bat_girl != 14) {
            rpg_bat_anim_set(29, 4, 1, 1);
        }
        rpg_bat_seq_step_next();
        break;
    case 28:
        rpg_bat_seq_step_gate(0);
        break;
    case 29:
        sub_step_up();
        break;
    default:
        rpg_bat_effect_clear();
        break;
    }
}
