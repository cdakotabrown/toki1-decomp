/* @CompileEnv: IDO52_CONST_INREG_VALUES=0 IDO52_COPYPROP_SITE=lod:1:deny,lod:3:deny IDO52_FORM_MERGE=const:0:19 IDO52_FORCE_WEB_COLOR=84:a2,51:a2 IDO52_AS1_TIE_TAKE=145 */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* Measured: `@CompileEnv: IDO52_CONST_INREG_VALUES=0` (all zero queries) */
#include "types.h"

typedef struct {
    s16 h[7];
} Tbl;

typedef struct {
    u8 p0[46];
    s16 f446;
    u8 p1[2];
    s16 f450;
    u8 p2[4];
} Chr;

typedef union {
    struct { u8 p0[400]; Chr c[63]; } r;
    struct { u8 p0[3935]; u8 sel; } s;
} State;

extern void sub_sub_step_up();
extern s32 get_g_zyotai_s();
extern void param_clamp();
extern void event_log_add();

extern State screen_w;
extern s16 para_you;
extern s16 girl9_tokimeki;
typedef struct { u32 b0 : 1; u32 b1 : 1; u32 rest : 30; } Bits;
extern Bits girl9_flag;
extern Bits frame_counter;
extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;
extern s16 shougatu_menu_now_serifu;
extern u8 shougatu_girl9_appear;
extern Tbl shougatu_girl0_init_tbl0;
extern Tbl shougatu_girl0_init_tbl1;
extern s32 shougatu_menu_serifu_tbl[];
extern s32 shougatu_menu_serifu_kind_tbl[];
extern s32 shougatu_menu_sound_tbl[];

#define SEL screen_w.s.sel
#define CHR(n) screen_w.r.c[n]

void shougatu_girl0_init()
{
    s32 q0, q1;
    Tbl hi;
    Tbl lo;
    s32 x;
    s32 r;
    s32 y;
    s16 i;

    hi = shougatu_girl0_init_tbl0;
    lo = shougatu_girl0_init_tbl1;
    shougatu_girl9_appear = 0;
    if (!girl9_flag.b1 && girl9_tokimeki >= 20 && frame_counter.b0) {
        shougatu_girl9_appear = 1;
    }
    SEL = 0;
    x = 0;
    r = get_g_zyotai_s(x);
    shougatu_menu_now_serifu = (((u8)r & 0x7F) == 4) << 2;
    shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[1];
    shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[1];
    shougatu_menu_sss_sound = shougatu_menu_sound_tbl[1];
    y = SEL;
    date_name_set(y);
    CHR(SEL).f450 += 4;
    for (i = 0; i < 7; i++) {
        if (para_you < hi.h[i]) {
            break;
        }
    }
    CHR(SEL).f446 += lo.h[i];
    param_clamp();
    event_log_add(4, 0);
    sub_sub_step_up();
}
