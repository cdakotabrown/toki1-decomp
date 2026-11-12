/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_CONST_INREG_VALUES=8:-1:3:4 IDO52_FORCE_WEB_COLOR=141:s5,142:s6,143:s7,31:s8,158:v1 */
/* @CompileFlags: -Wo,-varref,25 */
/* Address: 0x801422A4, Size: 760 bytes (190 insns) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 w00;
    s16 h04;
    s16 h06;
    s16 h08;
    s16 h0A;
    s16 h0C;
    u8 b0E;
    u8 b0F;
    s16 h10;
    s16 h12;
    u8 b14;
    u8 b15;
    u8 b16;
    u8 pad17;
    s16 h18;
    s16 h1A;
    s16 h1C;
    s16 h1E;
    s32 w20;
} Rec36;

typedef union {
    s32 w;
    struct {
        s16 x;
        s16 y;
    } h;
} Pt;

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void mes_window_switch(s32);
extern void bustup_ptr_init(void);
extern void date_name_set(s32);
extern void param_clamp(void);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern s32 rand(void);
extern void srand(s32);
extern void shougatu_ptrs3_set(void);
extern void shougatu_buf_init(void);

extern Rec36 Sprites[];
extern Pt shougatu_crowd_pos[];
extern Pt shougatu_crowd_pos_46[];

extern s16 now_serifuev;
extern s16 now_serifuev_in;
typedef struct {
    s16 a;
    s16 pad;
    s16 b;
} Pair91E;
extern Pair91E girl0_tokimeki;
extern u8 now_girl;
extern u8 sprite_state[];
extern s32 frame_counter;
extern u8 cal_cw_flag4;
extern s32 shougatu_ss_serifuev;
extern s32 shougatu_ss_serifuev_kind;
extern s32 shougatu_sss_soundev;
extern s32 shougatu_ev42_res_bg3[];
extern s16 shougatu_crowd_cnt;
extern s32 shougatu_crowd_timer;
extern CLASS_W class_w[];
extern s32 shougatu_serifuev_tbl[];
extern s32 shougatu_serifuev_kind_tbl[];
extern s32 shougatu_soundev_tbl[];
extern s32 shougatu_crowd_goal[];
extern DATE_OBJ_RES_EX shougatu_ev06_res;

void shougatu_crowd_init2(void)
{
    Rec36 *p;
    s16 i;
    s32 w;

    srand(frame_counter);
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    date_name_set(now_girl);
    mes_window_switch(0);
    shougatu_buf_init();
    shougatu_ptrs3_set();

    shougatu_ss_serifuev = shougatu_serifuev_tbl[12];
    shougatu_ss_serifuev_kind = shougatu_serifuev_kind_tbl[12];
    shougatu_sss_soundev = shougatu_soundev_tbl[12];
    palette_upload_enqueue(shougatu_ev06_res.pal2, 18, 1, 3, 0);

    for (i = 4, w = 320; i < 54; i++) {
        p = &Sprites[i];
        p->w00 = 0xC1000000;
        p->h04 = rand() % w - 159;
        shougatu_crowd_pos[i - 4].h.x = 0;
        shougatu_crowd_pos[i - 4].h.y = p->h04;
        shougatu_crowd_pos_46[i].w = shougatu_crowd_pos[i - 4].w;
        p->h06 = -120;
        p->h08 = 8;
        p->h0A = 8;
        p->h0C = 10;
        p->b0E = 0;
        p->b0F = 0;
        p->h10 = 0;
        p->h12 = 498;
        p->b14 = 128;
        p->b15 = 128;
        p->b16 = 128;
        p->h18 = 0;
        p->h1A = 0;
        p->h1C = 4096;
        p->h1E = 4096;
        p->w20 = 0;
        sprite_state[i] = 9;
        ((s16 *)&shougatu_crowd_goal[48])[i] = 2;
        shougatu_ev42_res_bg3[i] = 0x8000;
    }

    palette_upload_enqueue(shougatu_ev06_res.pal, 17, 1, 2, 0);
    date_sprite_init(shougatu_ev06_res.tile[0], shougatu_ev06_res.tile[1], shougatu_ev06_res.tile[2], shougatu_ev06_res.tile[3],
                  shougatu_ev06_res.tile[4], shougatu_ev06_res.tile[5]);
    date_object_set(shougatu_ev06_res.img, shougatu_ev06_res.anim_tbl, shougatu_ev06_res.hdr, shougatu_ev06_res.aux);

    cal_cw_flag4 = 8;
    shougatu_crowd_cnt = 0;
    shougatu_crowd_timer = 0;
    girl0_tokimeki.a = girl0_tokimeki.a + 1;
    girl0_tokimeki.b = girl0_tokimeki.b + 2;
    class_w[97].cel_mode = 8;
    param_clamp();
    sub_sub_step_up();
}
