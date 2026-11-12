/* @SymbolStart: 0x80157CF8 */
/* @SymbolSize: 288 */
/* Function: date_girl9_init4 */
/* Overlay: DATE */
/* Address: 0x80157D58 */
/* Size: 192 bytes (48 instructions) */
#include "types.h"

extern void param_clamp(void);
extern void date_ptrtbl14_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s16 girl9_tokimeki[];
extern s32 date_ev3f_res_pal;
extern s32 date_ev3f_res_bg0;
extern s32 date_ev3f_res_bg1;
extern s32 date_ev3f_res_bg2;
extern s32 date_ev3f_res_bg3;
extern s32 date_ev3f_res_bg4;
extern s32 date_ev3f_res_bg5;
extern s32 date_ev3f_res_cels;
extern s32 date_ev3f_res_anim_tbl;
extern s32 date_ev3f_res;
extern s16 date_ev3f_res_scale;
extern u8  date_object_set_tbl0;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl9_init4(void)
{
    date_ss_serifuev = date_serifuev_tbl[57];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[57];
    date_sss_soundev = date_soundev_tbl[57];
    girl9_tokimeki[0] += 2;
    girl9_tokimeki[2]++;
    girl9_tokimeki[4] -= 20;
    param_clamp();
    date_ptrtbl14_init();
    palette_upload_enqueue(date_ev3f_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev3f_res_bg0, date_ev3f_res_bg1, date_ev3f_res_bg2, date_ev3f_res_bg3, date_ev3f_res_bg4,
                  date_ev3f_res_bg5);
    date_object_set(date_ev3f_res_cels, date_ev3f_res_anim_tbl, date_ev3f_res, date_ev3f_res_scale);
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
