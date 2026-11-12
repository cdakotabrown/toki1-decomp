/* @SymbolStart: 0x80158C6C */
/* @SymbolSize: 268 */
/* Function: date_girl4_mode4_scene_init (real entry 0x80158C6C -- cross-prologue alt-entry) */
/* Overlay: DATE */
#include "types.h"

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_scene_set9_res[];
extern volatile s16 girl4_yuukou;
extern volatile s16 girl4_syoushin;
extern u8 date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_girl4_mode4_bufptr_init();

s32 date_girl4_mode4_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[64];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[64];
    date_sss_soundev = date_soundev_tbl[64];
    date_girl4_mode4_bufptr_init();
    palette_upload_enqueue(date_scene_set9_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set9_res[5], date_scene_set9_res[6], date_scene_set9_res[7], date_scene_set9_res[8],
                  date_scene_set9_res[9], date_scene_set9_res[10]);
    date_object_set(date_scene_set9_res[1], date_scene_set9_res[2], date_scene_set9_res[0], (s32)(*(s16 *)&date_scene_set9_res[3]));
    date_object_set_tbl0 = 1;
    girl4_yuukou = girl4_yuukou + 2;
    girl4_syoushin = girl4_syoushin - 20;
    param_clamp();
    return sub_sub_step_up();
}
