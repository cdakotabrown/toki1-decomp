/* @SymbolStart: 0x8015995C */
/* @SymbolSize: 300 */
#include "types.h"

extern s32 strcpy();
extern s32 param_clamp();
extern s32 date_girl10_mode4_bufptr_init();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 sub_sub_step_up();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern u8 ba_string2;
extern u8 date_scene_set2_str0;
extern volatile s16 girl10_tokimeki;
extern volatile s16 girl10_yuukou;
extern volatile s16 girl10_syoushin;
extern s32 date_scene_set2_res[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl10_mode4_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[70];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[70];
    date_sss_soundev = date_soundev_tbl[70];
    strcpy(&ba_string2, &date_scene_set2_str0);
    girl10_tokimeki = girl10_tokimeki + 3;
    girl10_yuukou = girl10_yuukou + 2;
    girl10_syoushin = girl10_syoushin - 20;
    param_clamp();
    date_girl10_mode4_bufptr_init();
    palette_upload_enqueue(date_scene_set2_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set2_res[5], date_scene_set2_res[6], date_scene_set2_res[7], date_scene_set2_res[8],
                  date_scene_set2_res[9], date_scene_set2_res[10]);
    date_object_set(date_scene_set2_res[1], date_scene_set2_res[2], date_scene_set2_res[0], (*(s16 *)&date_scene_set2_res[3]));
    sub_sub_step_up();
}
