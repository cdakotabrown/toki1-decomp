/* @SymbolStart: 0x80151C54 */
/* @SymbolSize: 276 */
/* Function: date_girl3_mode4_scene_init (true entry 0x80151C54) */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_ptrtbl7_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_scene_init7_res[];
extern volatile s16 girl3_tokimeki;
extern volatile s16 girl3_yuukou;
extern volatile s16 girl3_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl3_mode4_scene_init(s32 a0, s32 a1, s32 a2, s32 a3)
{
    date_ss_serifuev = date_serifuev_tbl[27];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[27];
    date_sss_soundev = date_soundev_tbl[27];
    date_ptrtbl7_init(a0, a1, a2, a3);
    palette_upload_enqueue(date_scene_init7_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_init7_res[5], date_scene_init7_res[6], date_scene_init7_res[7], date_scene_init7_res[8],
                  date_scene_init7_res[9], date_scene_init7_res[10]);
    date_object_set(date_scene_init7_res[1], date_scene_init7_res[2], date_scene_init7_res[0], (s32)(*(s16 *)&date_scene_init7_res[3]));
    girl3_tokimeki += 2;
    girl3_yuukou += 1;
    girl3_syoushin -= 20;
    param_clamp();
    return sub_sub_step_up();
}
