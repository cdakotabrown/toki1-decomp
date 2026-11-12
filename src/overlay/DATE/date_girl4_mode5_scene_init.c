/* @SymbolStart: 0x80158D78 */
/* @SymbolSize: 428 */
/* Address: 0x80158D9C  Size: 392 bytes (98 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 date_girl4_mode5_bufptr_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_scene_set10_res[];
extern u8 date_object_set_tbl0;
extern volatile s16 girl4_tokimeki;
extern volatile s16 girl4_yuukou;
extern volatile s16 girl4_syoushin;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl4_mode5_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[65];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[65];
    date_sss_soundev = date_soundev_tbl[65];
    date_girl4_mode5_bufptr_init();
    palette_upload_enqueue(date_scene_set10_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set10_res[5], date_scene_set10_res[6], date_scene_set10_res[7], date_scene_set10_res[8],
                  date_scene_set10_res[9], date_scene_set10_res[10]);
    date_object_set(date_scene_set10_res[1], date_scene_set10_res[2], date_scene_set10_res[0], (s32)(*(s16 *)&date_scene_set10_res[3]));
    date_object_set_tbl0 = 1;
    girl4_tokimeki = girl4_tokimeki + 1;
    girl4_yuukou = girl4_yuukou + 1;
    girl4_syoushin = girl4_syoushin - 20;
    param_clamp();
    kao[0] = 2;
    kao[2] = 2;
    kao[1] = 2;
    kao2[0] = 2;
    kao2[2] = 2;
    kao2[1] = 2;
    imp[0] = 2;
    imp[2] = 2;
    imp[1] = 2;
    ans[0] = 1;
    ans[2] = 1;
    ans[1] = 1;
    sub_sub_step_up();
}
