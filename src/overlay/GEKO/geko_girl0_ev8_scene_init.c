/* @PostProc: scheduler_normalizer */
/* geko_girl0_ev8_scene_init -- GEKO overlay, Address: 0x8013AAFC, Size: 308 bytes (77 insns), Byte-perfect */
#include "types.h"

extern void hizuke_init(void);
extern void shared_system_state_reset(void);
extern void srand(s32 a0);
extern void strcpy(s32 *a0, s32 *a1);
extern void bustup_ptr_init(void);
extern void date_name_set(s32 a0);
extern void geko_ptr_tbl_init(void);
extern void param_clamp(void);
extern void sub_sub_step_up(void);
extern s32 ba_string[];
extern s32 ba_string2[];

extern s8 bg_change_tbl2;
extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern u8 screen_bri;
extern s16 girl0_tokimeki;
extern s16 girl0_yuukou;
extern u8 day_flag;
extern u8 now_girl;
extern s32 frame_counter;
extern s32 dec_bg_on;
extern u8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s32 geko_scene_init1_str0;
extern s32 geko_scene_init1_str1;
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl0_ev8_scene_init(void)
{
    hizuke_init();
    shared_system_state_reset();
    day_flag = (day_flag | 0x5);
    dec_bg_on = 1;
    screen_bri = 128;
    fade_bri = 128;
    bg_change_tbl2 = 0;
    srand(frame_counter);
    now_girl = 0;
    strcpy(ba_string, &geko_scene_init1_str0);
    strcpy(ba_string2, &geko_scene_init1_str1);
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    date_name_set(now_girl);
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[13];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[13];
    geko_sss_soundev = geko_soundev_tbl[13];
    girl0_tokimeki = girl0_tokimeki + 2;
    girl0_yuukou = girl0_yuukou + 2;
    param_clamp();
    sub_sub_step_up();
}
