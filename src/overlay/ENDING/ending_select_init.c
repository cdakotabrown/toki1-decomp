/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:3:deny IDO52_UGEN_DEST_TEMP=31 IDO52_UGEN_TEMP_REG_INPLACE=occ:25 IDO52_AS1_TIE_TAKE=97,107,110,113 IDO52_UGEN_TEMP_REG=25:a0,26:t1,27:t2,28:t3,29:t4,30:t9,31:t8,32:t0,33:t7,36:t2,37:t3,38:t4,39:t5,40:t9,41:t8,42:t0,43:t7,44:t6 */
#include "types.h"

typedef struct {
    s16 h[18];
} Thresh;

typedef struct {
    unsigned b0 : 1;
    unsigned met : 1;
    unsigned rest : 30;
    u8 pad[52];
} Chr;

typedef struct {
    u8 pad[456];
    Chr chr[11];
    u8 pad2[3935 - 456 - 616];
    u8 ending;
} Save;

typedef struct {
    unsigned lo : 9;
    unsigned f : 3;
    unsigned hi : 20;
} Cfg;

extern Thresh ending_select_init_tbl;
extern Save screen_w;
extern u8  screen_bri;
extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern u8  now_youbi;
extern Cfg girl11_flag;
extern u8  denwa_cnt;
extern u8  ev_flag4c;
extern u8  ev_flag4d;
extern u8  ev_flag4e;
extern u8  ev_flag4f;
#define now_girl screen_w.ending
extern u8  vblank_flag;
extern s32 vb_ready;
extern u8  fade_bri;
extern u8  bg_change_tbl2;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  ba_string[];
extern u8  ba_string2[];
extern u8  ending_select_init_str0[];
extern u8  ending_select_init_str1[];
extern s32 ending_serifu_tbl[];
extern s32 ending_serifu_kind_tbl[];
extern s32 ending_sound_tbl[];

void ending_select_init()
{
    u8 buf[56]; s16 score[16]; s16 i; s16 pick; Thresh tbl; s32 best; Save *sv; s32 z0[2];

    tbl = ending_select_init_tbl;

    now_year = 98;
    now_month = 3;
    now_day = 1;
    now_youbi = 6;

    get_grp(buf, score);

    now_girl = 13;

    if (score[0] < 500 && denwa_cnt >= 70 && ev_flag4f == 2) {
        now_girl = 12;
    } else {
        sv = &screen_w;
        best = 499;
        for (i = 0; i < 11; i++) {
            if (sv->chr[i].met) {
                if (best < score[i]) {
                    sv->ending = i;
                    best = score[i];
                }
            }
        }
    }

    if (now_girl == 13) {
        pick = (s32)girl11_flag.f + (ev_flag4c >= 1) * 2 + (ev_flag4e >= 1) * 2;
        if (pick >= 18) {
            pick = 17;
        }
        if (girl11_flag.f != 0 && ev_flag4d == 0) {
            if (tbl.h[pick] > rand() % 256) {
                now_girl = 11;
            }
        }
    }

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(ba_string, ending_select_init_str0);
    strcpy(ba_string2, ending_select_init_str1);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    now_serifu = 0;
    now_serifu_in = 0;
    date_name_set(now_girl);
    ending_ptr_init();
    ss_serifu = ending_serifu_tbl[3];
    ss_serifu_kind = ending_serifu_kind_tbl[3];
    sss_sound = ending_sound_tbl[3];
    sub_sub_step_up();
}
