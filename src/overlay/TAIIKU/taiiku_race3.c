/* Address: 0x80134868 */
/* Size: 1688 bytes (422 insns) */
/* @PostProc: swap_instructions --offset1 0x364 --offset2 0x368 */
/* @PostProc: swap_instructions --offset1 0x36c --offset2 0x370 */
/* @PostProc: swap_instructions --offset1 0x370 --offset2 0x37c */
/* @PostProc: swap_instructions --offset1 0x374 --offset2 0x37c */
/* @PostProc: swap_instructions --offset1 0x378 --offset2 0x37c */
/* @PostProc: swap_instructions --offset1 0x368 --offset2 0x36c */
/* @PostProc: patch_insn_reg_field 0x24:rt:v0:v1 0x28:rs:v0:v1 0x28:rt:v0:v1 0x168:rs:v0:v1 0x174:rs:v0:v1 0x180:rs:v0:v1 0x3e0:rt:v1:v0 0x3e4:rs:v1:v0 0x3e4:rt:v1:v0 0x3ec:rs:v1:v0 0x5f0:rt:v1:v0 0x5f8:rd:v1:v0 0x5f8:rs:v1:v0 0x5fc:rs:v1:v0 0x5fc:rt:v1:v0 0x604:rd:v0:t2 0x604:rt:v1:v0 0x608:rd:t2:t1 0x608:rt:v0:t2 0x60c:rs:t2:t1 0x60c:rt:t2:t1 0x618:rd:t1:t3 0x618:rs:t2:t5 0x618:rt:t5:t1 0x61c:rt:t1:t3 0x620:rd:t3:t8 0x620:rt:v0:t2 0x62c:rs:t3:t8 0x62c:rt:t3:t8 0x634:rd:t8:t0 0x634:rs:t3:t4 0x634:rt:t4:t8 0x63c:rt:t8:t0 0x648:rd:t0:t9 0x64c:rd:t9:t7 0x64c:rt:t0:t9 0x650:rs:t9:t7 0x650:rt:t9:t7 0x65c:rd:t7:t2 0x65c:rs:t9:t6 0x65c:rt:t6:t7 0x660:rt:t7:t2 0x664:rd:t2:t1 0x664:rt:t0:t9 0x670:rs:t2:t1 0x670:rt:t2:t1 0x678:rd:t1:t3 0x678:rs:t2:t5 0x678:rt:t5:t1 0x67c:rt:t1:t3 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern s32 game_reset();
extern s32 set_kanji_string();
extern s32 k_disp_start();
extern void *get_k_work();
extern s32 scroll_poly_disp();
extern s32 load_done_flag_set();
extern void taiiku_seat_set();
extern void taiiku_seat_plate_set();
extern s32 taiiku_runner_count_inc();
extern void taiiku_run_frame_set();
extern void taiiku_racers_step();
extern s32 func_8013753C();
extern void taiiku_ouen_cue();
extern s32 taiiku_seat_walk();
extern void taiiku_racer_pos_set();
extern s32 taiiku_goal_check2();

typedef struct { s16 v[11]; } S24;
typedef struct { s32 v[11]; } S44;

extern S24 taiiku_race3_tbl0;
extern S44 taiiku_race3_tbl1;
extern S44 taiiku_race3_tbl2;
extern S44 taiiku_race3_tbl3;
extern S44 taiiku_race3_tbl4;

extern u8 now_sub_sub_step;
extern s32 load_done_flag;
extern s32 taiiku_konami_mode;
extern u8  taiiku_kari_cue;
extern s32 taiiku_kari_vmax;
extern s32 taiiku_kari_gain;
extern s32 taiiku_kari_run_0_speed;
extern s32 taiiku_kari_run_0_accel;
extern s16 taiiku_kari_scrl_hi;
extern s32 taiiku_kari_scrl;
extern s16 taiiku_kari_card;
u8 taiiku_kari_run[2] = {0};
extern u8  taiiku_kari_girl;
extern s16 taiiku_kari_gl_card[];
extern s16 cal_cw_x;
extern s16 cal_cw_frame;
extern CLASS_W class_w[];

void taiiku_race3(void) {
    u8 sel;
    s32 idx;
    S24 hdr;
    S44 team0, team1, team2, team3;

    hdr = taiiku_race3_tbl0;
    team0 = taiiku_race3_tbl1;
    team1 = taiiku_race3_tbl2;
    team2 = taiiku_race3_tbl3;
    team3 = taiiku_race3_tbl4;
    sel = now_sub_sub_step;

    switch ((u32)sel) {
    case 0:
        load_done_flag_set();
        if (load_done_flag == 1 && taiiku_kari_cue == 0) {
            taiiku_kari_cue = 1;
            device_cmd_send(5637);
            device_cmd_send(1792);
        }
        taiiku_runner_count_inc();
        taiiku_kari_run_0_speed = (taiiku_kari_vmax >> 1) + 1;
        taiiku_kari_run_0_accel = 1;
        taiiku_run_frame_set();
        taiiku_racers_step();
        taiiku_racer_pos_set();
        scroll_poly_disp((taiiku_kari_scrl_hi >> 1) % 640, 15, 0, 112, 640);
        scroll_poly_disp((taiiku_kari_scrl_hi >> 1) & 0x7F, 15, 112, 128, 128);
        taiiku_seat_set(taiiku_kari_scrl_hi, 3);
        taiiku_goal_check2();
        func_8013753C();
        taiiku_seat_plate_set();
        taiiku_kari_scrl = taiiku_kari_scrl + 0x18000;
        cal_cw_x = cal_cw_x - 1;
        if (cal_cw_x < -175) {
            cal_cw_x = -176;
        }
        if (taiiku_kari_scrl_hi < 640) {
            return;
        }
        device_cmd_send(1542);
        cal_cw_frame = 135;
        class_w[106].disp_flag = 164;
        class_w[106].anim_flag = 64;
        class_w[104].anim_flag = 64;
        class_w[104].disp_flag = 164;
        class_w[105].anim_flag = 64;
        class_w[105].disp_flag = 164;
        idx = taiiku_kari_card;
        class_w[105].frame = hdr.v[idx];
        set_kanji_string((s16)(cal_cw_x + 64), 64, 0, team0.v[idx], 0);
        set_kanji_string((s16)(cal_cw_x + 72), 80, 0, team1.v[taiiku_kari_card], 0);
        k_disp_start(2);
        class_w[104].x.half[1] = cal_cw_x + 48;
        class_w[105].x.half[1] = class_w[104].x.half[1] + 80;
        *(s16 *)get_k_work(0) = cal_cw_x + 64;
        *(s16 *)get_k_work(1) = cal_cw_x + 72;
        sub_sub_step_up();
        return;

    case 1:
        if (load_done_flag != 0 && taiiku_kari_cue == 0) {
            taiiku_kari_cue = 1;
            device_cmd_send(5637);
            device_cmd_send(1792);
        }
        taiiku_runner_count_inc();
        taiiku_racers_step();
        taiiku_racer_pos_set();
        taiiku_ouen_cue();
        taiiku_seat_plate_set();
        scroll_poly_disp((taiiku_kari_scrl_hi >> 1) % 640, 15, 0, 112, 640);
        scroll_poly_disp(taiiku_kari_scrl_hi & 0x7F, 15, 112, 128, 128);
        taiiku_seat_set(taiiku_kari_scrl_hi, 3);
        taiiku_goal_check2();
        if (taiiku_kari_run[1] == 0) {
            return;
        }
        sub_sub_step_up();
        return;

    case 2:
        taiiku_runner_count_inc();
        taiiku_racers_step();
        taiiku_racer_pos_set();
        taiiku_seat_plate_set();
        scroll_poly_disp((taiiku_kari_scrl_hi >> 1) % 640, 15, 0, 112, 640);
        scroll_poly_disp(taiiku_kari_scrl_hi & 0x7F, 15, 112, 128, 128);
        taiiku_goal_check2();
        if (taiiku_seat_walk(0) == 0) {
            return;
        }
        taiiku_kari_run[1] = 0;
        taiiku_kari_run[0] = 1;
        if (taiiku_konami_mode == 1) {
            idx = taiiku_kari_gl_card[taiiku_kari_girl];
            taiiku_kari_vmax = taiiku_kari_vmax + team2.v[idx];
            taiiku_kari_gain = taiiku_kari_gain + team3.v[idx];
        } else {
            idx = taiiku_kari_card;
            taiiku_kari_vmax = taiiku_kari_vmax + team2.v[idx];
            taiiku_kari_gain = taiiku_kari_gain + team3.v[idx];
        }
        sub_step_up();
        return;

    default:
        game_reset();
        return;
    }
}
