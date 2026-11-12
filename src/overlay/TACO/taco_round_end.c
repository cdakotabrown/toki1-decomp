/* @PostProc: patch_insn_reg_field 0x60:rt:a3:v0 0x64:rt:v0:v1 0x68:rs:a3:v1 0x7c:rt:t0:a3 0x80:rt:t0:a3 0x88:rs:t0:a3 0x88:rt:t0:a3 0x94:rt:t0:a3 0xf4:rs:t0:a3 0xf4:rt:t0:a3 0xfc:rs:t0:a3 0x118:rt:t0:a3 */
/* Address: 0x801391DC */
/* Size: 392 bytes (98 insns) */
#include "types.h"

extern s8 back_clear_r;
extern s8 back_clear_g;
extern s8 back_clear_b;
extern s32 taco_view_flag;
extern volatile s32 taco_score;
extern s32 taco_rank_score;
extern s32 taco_rank_score_1;
extern s32 taco_rank_score_2;
extern s32 taco_name_row;
extern u8 taco_rank_name[];
extern u8 taco_rank_name_1_0[];
extern u8 taco_rank_name_2_0[];
extern u8 taco_round_end_str0[];
extern u8 taco_round_end_str1[];
extern u8 taco_round_end_str2[];

extern void sub_sub_step_up(void);
extern void back_clear_switch(s32 a0);
extern void device_port_init2(s32 a0, s32 a1);
extern void all_object_disp_off(void);
extern void strncpy(u8 *dst, u8 *src, s32 n);
extern void taco_wheel_clear(void);
extern void taco_drop_init(void);

void taco_round_end(void)
{
    s32 first;
    s32 score;
    s32 second;

    taco_wheel_clear();
    taco_drop_init();
    device_port_init2(0, 514);
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    back_clear_switch(1);
    taco_view_flag = 0;
    all_object_disp_off();
    first = taco_rank_score;
    score = taco_score;
    if (first < score) {
        second = taco_rank_score_1;
        taco_name_row = 0;
        taco_rank_score_2 = second;
        taco_rank_score = score;
        taco_rank_score_1 = first;
        strncpy(taco_rank_name_2_0, taco_rank_name_1_0, 4);
        strncpy(taco_rank_name_1_0, taco_rank_name, 4);
        strncpy(taco_rank_name, taco_round_end_str0, 4);
    } else {
        second = taco_rank_score_1;
        if (second < score) {
            taco_name_row = 1;
            taco_rank_score_2 = second;
            taco_rank_score_1 = score;
            strncpy(taco_rank_name_2_0, taco_rank_name_1_0, 4);
            strncpy(taco_rank_name_1_0, taco_round_end_str1, 4);
        } else {
            taco_name_row = 2;
            taco_rank_score_2 = score;
            strncpy(taco_rank_name_2_0, taco_round_end_str2, 4);
        }
    }
    sub_sub_step_up();
}
