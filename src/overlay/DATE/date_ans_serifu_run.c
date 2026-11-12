/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    s16 a;
    s16 pad02;
    s16 b;
    s16 pad06;
    s16 c;
    u8  pad0A[46];
} GirlRec;

typedef struct {
    u8      pad000[446];
    GirlRec rec[24];
    u8      pad6FE[46];
    u8      date[16];
    u8      flag[16];
    u8      pad74C[16];
    u8      evt;
    u8      pad75D[3935 - 1885];
    u8      girl;
} SaveBlk;

extern SaveBlk screen_w;
extern u8  step_mode;
extern s16 now_serifue;
extern s32 serifu_no;
extern s16 ans[];
extern s32 now_date_spot;
extern u8  now_step;
extern u8  date_aug_flag;
extern u8  now_basyo;
extern s32 now_s;
extern s32 now_s_in;
extern s16 now_serifue_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void param_clamp();
void serifu_main(u8, s32, s32);

date_ans_serifu_run()
{
    s32 r;
    s16 val;
    u8 lv;

    r = get_g_zyotai_s(screen_w.girl);
    lv = r & 0x7F;
    if (step_mode >= 11) {
        step_mode = 10;
    }
    if (step_mode++ == 0) {
        now_serifue = ((lv >= 2) + (lv >= 3) + (lv >= 4)) * 3;
        if (lv == 4) {
            bustup_face_change(4);
            ans[serifu_no] = 0;
        }
    }
    val = ans[serifu_no] + now_serifue;
    if (screen_w.evt) {
        val = 10;
    }
    if (screen_w.evt && lv < 2 && screen_w.girl == 6) {
        val = 18;
    }
    if (now_date_spot >= 0 && now_date_spot == screen_w.date[screen_w.girl] &&
        screen_w.flag[screen_w.girl] != 0 && now_step == 80 &&
        date_aug_flag == 0 && screen_w.evt == 0 && now_basyo != 0 &&
        now_basyo != 18) {
        val = 11;
        if (lv < 2 && screen_w.girl == 6) {
            val = 19;
        }
        if (step_mode == 1) {
            bustup_face_change(3);
            screen_w.rec[screen_w.girl].a -= 1;
            screen_w.rec[screen_w.girl].b -= 2;
            screen_w.rec[screen_w.girl].c += 5;
            param_clamp();
        }
    }
    now_s = (s32)&val;
    now_s_in = (s32)&now_serifue_in;
    r = get_g_zyotai_s(screen_w.girl);
    lv = r & 0x7F;
    if ((screen_w.girl == 1 || screen_w.girl == 5) && lv < 2 &&
        now_serifue_in == 3) {
        bustup_face_change(1);
    }
    ss_s = date_serifu_tbl[22];
    ss_s_kind = date_serifu_kind_tbl[22];
    sss_so = date_sound_tbl[22];
    serifu_main(serifu_no, 1, 0);
}
