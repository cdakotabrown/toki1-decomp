/* @SymbolStart: 0x80139510 */
/* Address: 0x80139518, 604 insns, frame -80 (ra at sp+20). */
/* @SymbolSize: 2424 */
/* @RodataBase: 0x8015B934 */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --from 28 --to 40 --from 30 --to 42 --from 32 --to 44 --from 34 --to 46 --from 36 --to 48 --from 38 --to 50 --from 40 --to 52 --from 42 --to 54 --from 44 --to 56 --from 46 --to 58 --from 48 --to 60 --from 50 --to 62 --from 52 --to 64 --from 54 --to 66 --from 56 --to 68 --from 58 --to 70 */
/* @PostProc: swap_instructions --offset1 0x774 --offset2 0x778 */
/* @PostProc: swap_instructions --offset1 0x77c --offset2 0x780 */
/* @PostProc: swap_instructions --offset1 0x7a4 --offset2 0x7a8 */
/* @PostProc: swap_instructions --offset1 0x7ac --offset2 0x7b0 */
/* @PostProc: swap_instructions --offset1 0x7d4 --offset2 0x7d8 */
/* @PostProc: swap_instructions --offset1 0x7dc --offset2 0x7e0 */
/* @PostProc: swap_instructions --offset1 0x804 --offset2 0x808 */
/* @PostProc: swap_instructions --offset1 0x80c --offset2 0x810 */
/* @PostProc: patch_insn_reg_field 0x6c:rt:t0:v1 0x70:rs:t0:v1 0x88:rt:t0:v1 0x8c:rs:t0:v1 0x98:rt:t0:v1 0xa4:rt:t0:v1 0xc4:rt:t1:t0 0xc8:rs:t1:t0 0xc8:rt:t1:t0 0xd0:rs:t1:t0 0xd0:rt:t2:t1 0xd4:rt:t2:t1 0xd8:rt:t3:t2 0xdc:rs:t3:t2 0xdc:rt:t3:t2 0xe0:rt:t0:v1 0xe4:rs:t0:v1 0xe4:rt:t0:v1 0xec:rs:t3:t2 0xec:rt:t4:t3 0xf0:rt:t4:t3 0xf4:rs:t0:v1 0x134:rt:t5:t4 0x138:rs:t5:t4 0x138:rt:t5:t4 0x140:rs:t5:t4 0x148:rd:t5:t4 0x148:rt:t5:t4 0x150:rt:t5:t4 0x154:rt:t5:t4 0x15c:rs:t5:t4 0x378:rt:t6:t5 0x380:rt:t6:t5 0x428:rt:t0:v1 0x42c:rs:t0:v1 0x42c:rt:t0:v1 0x43c:rs:t0:v1 0x454:rt:t0:v1 0x458:rs:t0:v1 0x458:rt:t0:v1 0x460:rs:t0:v1 0x460:rt:v0:v1 0x464:rt:v0:v1 0x468:rs:v0:v1 0x554:rt:t0:v1 0x558:rs:t0:v1 0x558:rt:t0:v1 0x560:rs:t0:v1 0x560:rt:v0:v1 0x564:rt:v0:v1 0x568:rs:v0:v1 0x58c:rt:t0:v1 0x590:rs:t0:v1 0x590:rt:t0:v1 0x594:rt:t7:t6 0x59c:rd:t8:t7 0x59c:rs:t7:t6 0x59c:rt:t0:v1 0x5a0:rt:t8:t7 0x5a8:rs:t0:v1 0x5a8:rt:v0:v1 0x5ac:rt:v0:v1 0x5b0:rs:v0:v1 0x5dc:rt:t9:t8 0x5e4:rt:t9:t8 0x684:rt:a0:v1 0x688:rs:a0:v1 0x688:rt:a0:v1 0x690:rt:a0:v1 0x698:rs:a0:v1 0x698:rt:v0:v1 0x69c:rt:v0:v1 0x6a0:rs:v0:v1 0x6a8:rt:a0:v1 0x6b0:rt:a0:v1 0x764:rt:t1:t9 0x768:rt:t2:t0 0x76c:rt:t3:t1 0x770:rt:t4:t2 0x774:rt:t2:t0 0x778:rt:t1:t9 0x77c:rt:t4:t2 0x780:rt:t3:t1 0x794:rt:t5:t3 0x798:rt:t6:t4 0x79c:rt:t7:t5 0x7a0:rt:t8:t6 0x7a4:rt:t6:t4 0x7a8:rt:t5:t3 0x7ac:rt:t8:t6 0x7b0:rt:t7:t5 0x7c4:rt:t9:t7 0x7c8:rt:t1:t8 0x7cc:rt:t2:t9 0x7d0:rt:t3:t0 0x7d4:rt:t1:t8 0x7d8:rt:t9:t7 0x7dc:rt:t3:t0 0x7e0:rt:t2:t9 0x7f4:rt:t4:t1 0x7f8:rt:t5:t2 0x7fc:rt:t6:t3 0x800:rt:t7:t4 0x804:rt:t5:t2 0x808:rt:t4:t1 0x80c:rt:t7:t4 0x810:rt:t6:t3 0x830:rt:t8:t5 0x834:rt:t8:t5 0x83c:rt:t9:t6 0x840:rt:t9:t6 0x848:rt:t1:t7 0x84c:rt:t1:t7 0x854:rt:t2:t8 0x858:rt:t2:t8 0x860:rt:t3:t9 0x868:rt:t3:t9 0x874:rt:t0:v1 0x878:rs:t0:v1 0x878:rt:t0:v1 0x894:rt:t0:v1 0x898:rt:t0:v1 0x89c:rs:t0:v1 0x89c:rt:t0:v1 0x8a4:rs:t0:v1 0x8a4:rt:v0:v1 0x8a8:rt:v0:v1 0x8ac:rs:v0:v1 0x8e4:rt:t4:t0 0x8e8:rs:t4:t0 0x8e8:rt:t4:t0 0x8f0:rs:t4:t0 0x8f4:rt:t5:t1 0x90c:rt:t5:t1 0x954:rt:t6:t2 0x958:rt:t6:t2 0x970:rt:t7:t3 0x974:rt:t7:t3 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_seq_next(void);
extern void rpg_bat_furyou_talk_step3(void);
extern void rpg_bat_se_set(s32, s32, s32);
extern void k_disp_switch(s32, s32);
extern void rpg_bat_seq_wait3(s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void device_port_init(s32, s32);
extern void strcpy(void *, void *);
extern void strcat(void *, void *);
extern void rpg_bat_random_quad_pick(void);
extern void rpg_bat_msg_set(void *, s32, s32);
extern void rpg_bat_seq_gate(s32);
extern s32  rpg_bat_flag_check(void);
extern void sub_step_up(void);
extern void matte_talk(void);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void banchou_intro_talk(void);
extern s32  rpg_bat_rec_flag_check(s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_sprite_bri_set(s32);
extern void banchou_taunt_talk(void);
extern void rpg_bat_load(void);
extern void tenshi_koe_talk(void);
extern void rpg_bat_blink(void);
extern void rpg_bat_attack_step(void);
extern void banchou_shock_talk(void);
extern void banchou_power_talk(void);
extern void MoveImage(void *, s32, s32);

extern u8  p_sei[];
extern volatile s16 cw155_x;
extern volatile s16 cw156_x;
extern volatile s16 cw157_x;
extern u8  rpg_bat_phase_str0[];
extern u8  rpg_bat_phase_str1[];
extern u8  rpg_bat_phase_str2[];
extern u8  rpg_bat_phase_str3[];
extern u8  rpg_bat_phase_str4[];
extern u8  rpg_bat_phase_str5[];
extern u8  rpg_bat_phase_str6[];
extern u8  rpg_bat_msg_buf4[];
extern u8  rpg_bat_msg4_line1[];
extern u8  rpg_bat_msg4_line2[];
extern u8  rpg_bat_msg_tbl4[];
extern s32 rpg_bat_girl;
extern s32 rpg_bat_dmg_rate;
extern s32 rpg_bat_act_delay;
extern s32 rpg_bat_act_wt[];
extern s32 rpg_bat_result;
extern s32 rpg_bat_pause_cnt;
extern s32 rpg_bat_bg_wave_phase;
extern s32 rpg_bat_bg_wave_amp;
extern s32 rpg_bat_bg_wave_period;
extern s32 rpg_bat_bg_wave_len;
extern s32 rpg_bat_bg_wave_step;
extern s32 rpg_bat_bg_wave_on;
extern CLASS_W class_w[];
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

void rpg_bat_phase(void)
{
    s32 v1;
    Rect rect[4];

    switch (rpg_bat_unit3.step) {
    case 0:
        rpg_bat_unit0.flag = 4096;
        rpg_bat_seq_next();
        break;
    case 1:
        rpg_bat_furyou_talk_step3();
        break;
    case 2:
        v1 = rpg_bat_unit3.cnt;
        if (v1 == 0) {
            rpg_bat_se_set(1287, 1, 0);
            v1 = rpg_bat_unit3.cnt;
        }
        rpg_bat_unit3.cnt = v1 + 8;
        k_disp_switch(rpg_bat_unit1.disp, 0);
        cw155_x -= 8;
        cw156_x -= 8;
        cw157_x -= 8;
        if (rpg_bat_unit3.cnt >= 145)
            rpg_bat_seq_next();
        break;
    case 3:
        rpg_bat_seq_wait3(30);
        break;
    case 4:
        rpg_bat_anim_set(48, 9, 1, 1);
        switch (rpg_bat_girl) {
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
        default:
            break;
        }
        device_port_init(1, 514);
        strcpy(rpg_bat_msg_buf4, rpg_bat_phase_str0);
        strcpy(rpg_bat_msg4_line1, rpg_bat_phase_str1);
        strcpy(rpg_bat_msg4_line2, rpg_bat_phase_str2);
        rpg_bat_random_quad_pick();
        rpg_bat_msg_set(rpg_bat_msg_tbl4, rpg_bat_seq_work[40], 90);
        rpg_bat_seq_next();
        break;
    case 5:
        rpg_bat_seq_gate(0);
        break;
    case 6:
        rpg_bat_result = 0;
        if (rpg_bat_flag_check() != 0)
            rpg_bat_seq_next();
        else
            sub_step_up();
        break;
    case 7:
        rpg_bat_seq_wait3(60);
        break;
    case 8:
        class_w[127].bright = 64;
        rpg_bat_anim_set(48, 0, 1, 1);
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_seq_next();
        break;
    case 9:
        matte_talk();
        break;
    case 10:
        strcpy(rpg_bat_msg_buf4, p_sei);
        strcat(rpg_bat_msg_buf4, rpg_bat_phase_str3);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 60);
        rpg_bat_seq_next();
        break;
    case 11:
        rpg_bat_seq_gate(0);
        break;
    case 12:
        v1 = rpg_bat_unit3.cnt;
        rpg_bat_sprite_pos_set(59, v1 - 128, 0);
        rpg_bat_anim_set(59, 2, 1, 1);
        v1 = rpg_bat_unit3.cnt;
        v1 = v1 + 8;
        rpg_bat_unit3.cnt = v1;
        if (v1 >= 129) {
            device_port_init(1, 513);
            k_disp_switch(rpg_bat_unit3.disp, 1);
            rpg_bat_seq_next();
        }
        break;
    case 13:
        banchou_intro_talk();
        break;
    case 14:
        rpg_bat_anim_set(59, 3, 5, 1);
        rpg_bat_se_set(1288, 1, 0);
        rpg_bat_seq_next();
        break;
    case 15:
        if (rpg_bat_rec_flag_check(59) != 0) {
            rpg_bat_slot_adjust(0, 33, rpg_bat_unit0.hp, 1);
            rpg_bat_anim_set(59, 2, 1, 1);
            rpg_bat_anim_set(48, 7, 1, 1);
            rpg_bat_unit0.hp = 0;
            rpg_bat_seq_next();
        }
        break;
    case 16:
        device_port_init(1, 515);
        rpg_bat_sprite_bri_set(255);
        v1 = rpg_bat_unit3.cnt;
        v1 = v1 + 1;
        rpg_bat_unit3.cnt = v1;
        if (v1 >= 5)
            rpg_bat_seq_next();
        break;
    case 17:
        rpg_bat_sprite_bri_set(128);
        v1 = rpg_bat_unit3.cnt;
        class_w[127].bright = (u8)(64 - v1);
        v1 = v1 + 1;
        rpg_bat_unit3.cnt = v1;
        if (v1 >= 65)
            rpg_bat_seq_next();
        break;
    case 18:
        banchou_taunt_talk();
        break;
    case 19:
        rpg_bat_pause_cnt = 4095;
        rpg_bat_seq_wait3(60);
        break;
    case 20:
        rpg_bat_load();
        break;
    case 21:
        strcpy(rpg_bat_msg_buf4, rpg_bat_phase_str4);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 90);
        rpg_bat_seq_next();
        break;
    case 22:
        rpg_bat_seq_gate(0);
        break;
    case 23:
        tenshi_koe_talk();
        break;
    case 24:
        rpg_bat_blink();
        break;
    case 25:
        rpg_bat_anim_set(48, 8, 5, 1);
        v1 = rpg_bat_unit0.hp_max;
        rpg_bat_unit0.hp = v1;
        v1 += 2000;
        rpg_bat_unit0.hp_max = v1;
        if (v1 >= 10000) {
            v1 = 9999;
            rpg_bat_unit0.hp_max = v1;
        }
        rpg_bat_seq_next();
        break;
    case 26:
        rpg_bat_attack_step();
        break;
    case 27:
        banchou_shock_talk();
        break;
    case 28:
        strcpy(rpg_bat_msg_buf4, p_sei);
        strcat(rpg_bat_msg_buf4, rpg_bat_phase_str5);
        strcpy(rpg_bat_msg4_line1, rpg_bat_phase_str6);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 2, 90);
        rpg_bat_seq_next();
        break;
    case 29:
        rpg_bat_seq_gate(2);
        break;
    case 30:
        banchou_power_talk();
        break;
    case 31:
        rect[3].x = 576;
        rect[3].y = 256;
        rect[3].w = 64;
        rect[3].h = 40;
        MoveImage(&rect[3], 384, 384);
        rect[2].x = 576;
        rect[2].y = 296;
        rect[2].w = 64;
        rect[2].h = 40;
        MoveImage(&rect[2], 448, 384);
        rect[1].x = 544;
        rect[1].y = 256;
        rect[1].w = 64;
        rect[1].h = 40;
        MoveImage(&rect[1], 512, 384);
        rect[0].x = 543;
        rect[0].y = 256;
        rect[0].w = 1;
        rect[0].h = 128;
        MoveImage(&rect[0], 544, 256);
        rpg_bat_bg_wave_phase = 0;
        rpg_bat_bg_wave_amp = 32;
        rpg_bat_bg_wave_period = 32;
        rpg_bat_bg_wave_len = 64;
        rpg_bat_bg_wave_step = 1;
        rpg_bat_bg_wave_on = 1;
        rpg_bat_seq_next();
        break;
    case 32:
        v1 = rpg_bat_unit3.cnt;
        class_w[127].bright = (u8)v1;
        rpg_bat_anim_set(31, 0, 1, 0);
        v1 = rpg_bat_unit3.cnt;
        v1 = v1 + 1;
        rpg_bat_unit3.cnt = v1;
        if (v1 >= 129)
            rpg_bat_seq_next();
        break;
    case 33:
        rpg_bat_unit3.flag = 0;
        rpg_bat_unit3.step = 0;
        device_port_init(1, 516);
        if (rpg_bat_unit2.flag != 0x40000000)
            rpg_bat_unit2.flag = 0;
        rpg_bat_act_delay = 0;
        rpg_bat_dmg_rate = 1000;
        rpg_bat_act_wt[0] = 0;
        rpg_bat_act_wt[1] = 0;
        rpg_bat_act_wt[2] = 0;
        rpg_bat_act_wt[3] = 0;
        rpg_bat_unit2.step = 0;
        rpg_bat_unit2.aux08 = 0;
        rpg_bat_unit0.flag = 0;
        rpg_bat_unit0.step = 0;
        rpg_bat_unit0.aux08 = 0xFFFF;
        rpg_bat_unit1.step = 0;
        rpg_bat_unit1.aux08 = 0;
        rpg_bat_unit1.flag = 0x200000;
        break;
    default:
        break;
    }
}
