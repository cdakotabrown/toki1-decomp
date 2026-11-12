/* @SymbolStart: 0x8015488C */
/* @SymbolSize: 308 */
/* Function: date_girl1_init4  (real entry 0x8015488C) */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_bufaddr_init3();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_girl1_init4_res[];
extern s32 cal_cw_attr;
extern s16 cal_cw_anim;
extern volatile s16 girl1_tokimeki;
extern volatile s16 girl1_yuukou;
extern volatile s16 girl1_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl1_init4(s32 a0, s32 a1, s32 a2, s32 a3)
{
    date_ss_serifuev = date_serifuev_tbl[40];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[40];
    date_sss_soundev = date_soundev_tbl[40];
    date_bufaddr_init3(a0, a1, a2, a3);
    palette_upload_enqueue(date_girl1_init4_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl1_init4_res[5], date_girl1_init4_res[6], date_girl1_init4_res[7], date_girl1_init4_res[8],
                  date_girl1_init4_res[9], date_girl1_init4_res[10]);
    date_object_set(date_girl1_init4_res[1], date_girl1_init4_res[2], date_girl1_init4_res[0], (*(s16 *)&date_girl1_init4_res[3]));
    cal_cw_attr = cal_cw_attr | 0x80000000;
    cal_cw_anim = 0;
    girl1_tokimeki = girl1_tokimeki + 2;
    girl1_yuukou = girl1_yuukou + 2;
    girl1_syoushin = girl1_syoushin - 20;
    param_clamp();
    sub_sub_step_up();
}
