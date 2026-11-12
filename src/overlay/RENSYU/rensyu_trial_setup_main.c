/* @CompileFlags: -cckr */
/* @DataBase: 0x80134338 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=3:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u16 v[11];
} Tbl11;

typedef struct {
    u16 a;
    u16 b;
} Pair;

typedef struct {
    u8 pad0[244];
    u16 date;
    u8 pad_f6[1644 - 246];
    Pair pair[16];
} State;

extern State screen_w;
extern u8 now_sub_sub_step;
extern u8 rensyu_trial_no;
extern u8 rensyu_succ_cnt;
u8 rensyu_trial_hit_flag[5] = {0};
extern u8 cal_cw_prio;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;
extern s32 rensyu_cal_cw_res[];
extern CLASS_W class_w[];

extern void sub_sub_step_up();
extern void common_disp();
extern void game_reset();
extern void sub_step_up();
extern void class_clear_n();

void rensyu_trial_setup_main()
{
    s32 i;
    s32 j;
    s32 rnd;
    s32 k;
    u32 sel;
    u8 a;
    u8 b;
    s16 v;
    Tbl11 tbl = { {
        30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330
    } };

    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        v = screen_w.pair[screen_w.date >> 12].a -
            tbl.v[screen_w.pair[screen_w.date >> 12].b & 15];
        if (v < -30) {
            rensyu_succ_cnt = 0;
        } else if (v < -20) {
            rensyu_succ_cnt = 1;
        } else if (v < -10) {
            rensyu_succ_cnt = 1;
        } else if (v < 0) {
            rensyu_succ_cnt = 2;
        } else if (v >= 30) {
            rensyu_succ_cnt = 5;
        } else if (v >= 20) {
            rensyu_succ_cnt = 4;
        } else if (v >= 10) {
            rensyu_succ_cnt = 4;
        } else if (v >= 0) {
            rensyu_succ_cnt = 3;
        }
        sub_sub_step_up();
        break;
    case 1:
        for (i = 0; i < 5; i++) {
            rensyu_trial_hit_flag[i] = 0;
        }
        for (i = 0; i < rensyu_succ_cnt; i++) {
            rensyu_trial_hit_flag[i] = 1;
        }
        i = 0;
        rnd = rand() & 15;
        if (rensyu_succ_cnt >= 3) {
            a = rensyu_trial_hit_flag[0];
            b = rensyu_trial_hit_flag[4];
            rensyu_trial_hit_flag[4] = a;
            rensyu_trial_hit_flag[0] = b;
        }
        for (i = 0; i < rnd; i++) {
            j = rand() & 3;
            k = rand() & 3;
            a = rensyu_trial_hit_flag[j];
            b = rensyu_trial_hit_flag[k];
            rensyu_trial_hit_flag[j] = b;
            rensyu_trial_hit_flag[k] = a;
        }
        if ((screen_w.date >> 12) == 9) {
            if (rensyu_succ_cnt >= 3) {
                rensyu_trial_hit_flag[0] = 1;
            } else {
                rensyu_trial_hit_flag[0] = 0;
            }
        }
        sub_sub_step_up();
        break;
    case 2:
        class_clear_n(96);
        cal_cw_prio = 3;
        cal_cw_attr = 0x1000000;
        cal_cw_bright = 128;
        cal_cw_sel = 128;
        (*(u16 *)&class_w[96].scale_x) = 4096;
        (*(u16 *)&class_w[96].scale_y) = 4096;
        cal_cw_rate = 1;
        cal_cw_clut = (screen_w.pair[screen_w.date >> 12].b & 15) % 5 + 16;
        cal_cw_x = -96;
        cal_cw_y = 32;
        cal_cw_img = rensyu_cal_cw_res[1];
        cal_cw_anim_tbl = rensyu_cal_cw_res[2];
        cal_cw_hdr = rensyu_cal_cw_res[0];
        cal_cw_aux14 = (*(s16 *)&rensyu_cal_cw_res[3]);
        rensyu_trial_no = 0;
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
