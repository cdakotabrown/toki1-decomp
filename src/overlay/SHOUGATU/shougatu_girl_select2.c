/* @CompileEnv: IDO52_FORCE_WEB_RANGE_SPLIT=21 IDO52_SPLIT_BLOCKS=21#1:7-19 IDO52_FORCE_WEB_NOSPILL=21.0:5,21.1/-1:5 IDO52_UGEN_TEMP_REG=37:t8,38:t9,39:t6,40:t8,41:t9,42:t6 IDO52_AS1_TIE_TAKE=756 IDO52_AS1_TIE_REJECT=139,145,757 IDO52_FORCE_WEB_COLOR=21:t4,156:v1,146:t1,54:t2 */
/* @CompileFlags: -cckr -Wc,-pic0 */
#include "types.h"

typedef struct {
    s16 h[7];
} Tbl;

typedef struct {
    u32 b0 : 1;
    u32 b1 : 1;
    u32 rest : 30;
} Bits;

typedef struct {
    u8 p0[42];
    s16 f446;
    s16 f448;
    s16 f450;
    u8 p1[4];
    Bits fl;
} Chr;

typedef union {
    struct { u8 p0[404]; Chr c[63]; } r;
    struct { u8 p0[3935]; u8 sel; } s;
    struct { u8 p0[4344]; u32 v; } t;
    struct { u8 p0[458]; u8 v; } u;
} State;

extern void sub_sub_step_up();
extern void goto_sub_step();
extern void date_name_set();
extern void event_log_add();
extern void shougatu_girl_slot_set();

extern State screen_w;
extern s16 para_you;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern u8 now_step;
extern u8 now_sub_step;
extern s32 rec_res;
extern s16 shougatu_menu_now_serifu;
extern u8 shougatu_menu_ev_girl;
extern u8 shougatu_menu_girl1;
extern u8 shougatu_sel_girl_no;
extern Tbl shougatu_girl_select2_tbl0;
extern Tbl shougatu_girl_select2_tbl1;

#define SEL screen_w.s.sel
#define CHR(n) screen_w.r.c[n]

void shougatu_girl_select2()
{
    s16 i;
    s32 bit;
    Tbl hi;
    Tbl lo;
    s16 cnt;
    u8 v;
    u32 r;
    u8 s;

    hi = shougatu_girl_select2_tbl0;
    lo = shougatu_girl_select2_tbl1;
    cnt = 0;
    for (i = 0; i < 11; i++) {
        cnt += (CHR(i).fl.b1 >= 1);
    }
    i = 0;
    for (;;) {
        SEL = (*(u32 *)&screen_w.t.v + i) % 10 + 1;
        i++;
        bit = CHR(SEL).fl.b1;
        if (bit != 0 || SEL < 9) {
            if (5 == *(u8 *)&SEL && screen_w.u.v >> 4 == 8) continue;
            if (6 == *(u8 *)&SEL && screen_w.u.v >> 4 == 9) continue;
            if (shougatu_menu_girl1 == SEL) continue;
            if (bit != 0 || cnt < 3) break;
        }
    }
    v = shougatu_menu_ev_girl;
    if (v == 7 || v == 9) {
        if (shougatu_menu_girl1 != v) {
            SEL = v;
        }
    }
    shougatu_sel_girl_no = SEL;
    shougatu_menu_now_serifu = 0;
    r = get_g_zyotai_s(SEL);
    if (*(u8 *)&SEL == 6 && (r & 0x7F) >= 2) {
        shougatu_menu_now_serifu++;
    }
    if ((r & 0x7F) == 4) {
        shougatu_menu_now_serifu++;
    }
    shougatu_girl_slot_set();
    date_name_set(SEL);
    if (!CHR(SEL).fl.b1) {
        CHR(SEL).fl.b1 = 1;
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = 24;
        rec_res = 3;
        event_log_add(4, 0);
        goto_sub_step(5);
    } else {
        CHR(SEL).f450 += 4;
        for (i = 0; i < 7; i++) {
            if (para_you < hi.h[i]) {
                break;
            }
        }
        CHR(SEL).f446 += lo.h[i];
        event_log_add(4, 0);
        sub_sub_step_up();
    }
}
