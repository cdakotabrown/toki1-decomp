/* date_girl6_mode3_scene_init -- DATE overlay, Address: 0x80156AD4 */
/* @SymbolStart: 0x80156AB0 */
/* @SymbolSize: 288 */
#include "types.h"

extern s32 date_girl6_mode3_bufptr_init(s32, s32, s32, s32);
extern s32 palette_upload_enqueue(s32, s32, s32, s32, s32);
extern s32 date_sprite_init(s32, s32, s32, s32, s32, s32);
extern s32 date_object_set(s32, s32, s32, s32);
extern s32 param_clamp(void);
extern s32 sub_sub_step_up(void);

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_scene_set6_res[];
extern volatile s16 girl6_tokimeki;
extern volatile s16 girl6_yuukou;
extern volatile s16 girl6_syoushin;
extern u8  date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl6_mode3_scene_init(s32 a0, s32 a1, s32 a2, s32 a3)
{
    date_ss_serifuev = date_serifuev_tbl[51];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[51];
    date_sss_soundev = date_soundev_tbl[51];

    date_girl6_mode3_bufptr_init(a0, a1, a2, a3);
    palette_upload_enqueue(date_scene_set6_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set6_res[5], date_scene_set6_res[6], date_scene_set6_res[7], date_scene_set6_res[8],
                  date_scene_set6_res[9], date_scene_set6_res[10]);
    date_object_set(date_scene_set6_res[1], date_scene_set6_res[2], date_scene_set6_res[0], (*(s16 *)&date_scene_set6_res[3]));

    date_object_set_tbl0 = 1;
    girl6_tokimeki = girl6_tokimeki + 3;
    girl6_yuukou = girl6_yuukou + 2;
    girl6_syoushin = girl6_syoushin - 20;

    param_clamp();
    return sub_sub_step_up();
}
