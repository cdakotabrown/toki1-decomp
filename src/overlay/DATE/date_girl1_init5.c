/* @SymbolStart: 0x801549C0 */
/* @SymbolSize: 308 */
/* Function: date_girl1_init5  (true symbol starts 0x801549C0) */
/* Overlay: DATE */
#include "types.h"

extern s32 palette_upload_enqueue(s32, s32, s32, s32, s32);
extern s32 date_sprite_init(s32, s32, s32, s32, s32, s32);
extern s32 date_object_set(s32, s32, s32, s32);
extern s32 param_clamp();
extern s32 sub_sub_step_up();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 date_ev0c_res;
extern s32 date_ev0c_res_cels;
extern s32 date_ev0c_res_anim_tbl;
extern s16 date_ev0c_res_scale;
extern s32 date_ev0c_res_pal;
extern s32 date_ev0c_res_bg0;
extern s32 date_ev0c_res_bg1;
extern s32 date_ev0c_res_bg2;
extern s32 date_ev0c_res_bg3;
extern s32 date_ev0c_res_bg4;
extern s32 date_ev0c_res_bg5;
extern u8  date_ev0c_bri_flag;

extern volatile s16 girl1_tokimeki;
extern volatile s16 girl1_yuukou;
extern volatile s16 girl1_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl1_init5(void)
{
    s32 v0;

    date_ss_serifuev = date_serifuev_tbl[41];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[41];
    date_sss_soundev = date_soundev_tbl[41];

    v0 = palette_upload_enqueue(date_ev0c_res_pal, 17, 1, 2, 0);
    v0 = palette_upload_enqueue(date_ev0c_res_pal, 18, 1, 2, 1);
    v0 = date_sprite_init(date_ev0c_res_bg0, date_ev0c_res_bg1, date_ev0c_res_bg2, date_ev0c_res_bg3,
                       date_ev0c_res_bg4, date_ev0c_res_bg5);
    v0 = date_object_set(date_ev0c_res_cels, date_ev0c_res_anim_tbl, date_ev0c_res, (s16)date_ev0c_res_scale);

    girl1_tokimeki = (s16)(girl1_tokimeki + 3);
    girl1_yuukou = (s16)(girl1_yuukou + 2);
    girl1_syoushin = (s16)(girl1_syoushin - 20);
    v0 = param_clamp();
    date_ev0c_bri_flag = 0;
    v0 = sub_sub_step_up();
    return v0;
}
