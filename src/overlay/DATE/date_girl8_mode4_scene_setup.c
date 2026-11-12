/* @SymbolStart: 0x8014D4EC */
/* Function: date_girl8_mode4_scene_setup */
/* Overlay: DATE */
/* Address: 0x8014D560 */
/* Size: 200 bytes (50 instructions) */
/* @SymbolSize: 308 */
#include "types.h"

extern void param_clamp(void);
extern void date_bufptr7e55_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s16 girl8_tokimeki[];
extern s16 date_anim_ptn;
extern s16 date_anim_ptn2;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_ev39_res_pal;
extern s32 date_ev39_res_bg0;
extern s32 date_ev39_res_bg1;
extern s32 date_ev39_res_bg2;
extern s32 date_ev39_res_bg3;
extern s32 date_ev39_res_bg4;
extern s32 date_ev39_res_bg5;
extern s32 date_ev39_res_cels;
extern s32 date_ev39_res_anim_tbl;
extern s32 date_ev39_res;
extern s16 date_ev39_res_scale;
extern u8  date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl8_mode4_scene_setup(void)
{
    date_anim_ptn = 0;
    date_anim_ptn2 = 3;
    date_ss_serifuev = date_serifuev_tbl[3];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[3];
    date_sss_soundev = date_soundev_tbl[3];
    girl8_tokimeki[0]++;
    girl8_tokimeki[2]++;
    girl8_tokimeki[4] -= 20;
    param_clamp();
    date_bufptr7e55_init();
    palette_upload_enqueue(date_ev39_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev39_res_bg0, date_ev39_res_bg1, date_ev39_res_bg2, date_ev39_res_bg3, date_ev39_res_bg4,
                  date_ev39_res_bg5);
    date_object_set(date_ev39_res_cels, date_ev39_res_anim_tbl, date_ev39_res, date_ev39_res_scale);
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
