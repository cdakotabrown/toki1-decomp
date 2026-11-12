/* @SymbolStart: 0x80158994 */
/* @SymbolSize: 360 */
/* Address: 0x801589B8  Size: 324 bytes (81 insns; .sym may over-size) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 date_girl4_mode2_bufptr_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_scene_init9_res[];
extern volatile s16 girl4_tokimeki;
extern volatile s16 girl4_yuukou;
extern volatile s16 girl4_syoushin;
extern volatile s16 para_tai;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl4_mode2_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[62];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[62];
    date_sss_soundev = date_soundev_tbl[62];
    date_girl4_mode2_bufptr_init();
    palette_upload_enqueue(date_scene_init9_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_init9_res[5], date_scene_init9_res[6], date_scene_init9_res[7], date_scene_init9_res[8],
                  date_scene_init9_res[9], date_scene_init9_res[10]);
    date_object_set(date_scene_init9_res[1], date_scene_init9_res[2], date_scene_init9_res[0], (s32)(*(s16 *)&date_scene_init9_res[3]));
    girl4_tokimeki = girl4_tokimeki + 2;
    girl4_yuukou = girl4_yuukou + 1;
    girl4_syoushin = girl4_syoushin - 20;
    param_clamp();
    girl4_tokimeki = girl4_tokimeki + 3;
    girl4_yuukou = girl4_yuukou + 2;
    girl4_syoushin = girl4_syoushin - 20;
    para_tai = para_tai + 10;
    param_clamp();
    sub_sub_step_up();
}
