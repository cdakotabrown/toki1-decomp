/* @SymbolStart: 0x80156BD0 */
/* @SymbolSize: 288 */
/* @PostProc: scheduler_normalizer --block-aware */
#include "types.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void param_clamp(void);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void date_girl6_mode4_bufptr_init(void);

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 date_scene_set7_res[];

extern s16 girl6_tokimeki;
extern s16 girl6_yuukou;
extern s16 girl6_syoushin;
extern u8 date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl6_mode4_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[52];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[52];
    date_sss_soundev = date_soundev_tbl[52];

    date_girl6_mode4_bufptr_init();
    palette_upload_enqueue(date_scene_set7_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set7_res[5], date_scene_set7_res[6], date_scene_set7_res[7], date_scene_set7_res[8],
                  date_scene_set7_res[9], date_scene_set7_res[10]);
    date_object_set(date_scene_set7_res[1], date_scene_set7_res[2], date_scene_set7_res[0], (s16)(*(s16 *)&date_scene_set7_res[3]));

    date_object_set_tbl0 = 1;
    girl6_tokimeki = girl6_tokimeki + 2;
    girl6_yuukou = girl6_yuukou + 2;
    girl6_syoushin = girl6_syoushin - 10;

    param_clamp();
    sub_sub_step_up();
}
