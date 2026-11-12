/* @SymbolStart: 0x801568E4 */
/* @SymbolSize: 460 */
/* Address: 0x80156908  Size: 424 bytes (106 insns; .sym over-sizes to 460) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 date_girl6_mode2_bufptr_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_scene_set5_res[];
extern volatile s16 girl6_tokimeki;
extern volatile s16 girl6_yuukou;
extern volatile s16 girl6_syoushin;
extern u8 now_girl;
extern u8 screen_w[];
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl6_mode2_scene_init(void)
{
    s32 girl;

    date_ss_serifuev = date_serifuev_tbl[50];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[50];
    date_sss_soundev = date_soundev_tbl[50];
    date_girl6_mode2_bufptr_init();
    palette_upload_enqueue(date_scene_set5_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set5_res[5], date_scene_set5_res[6], date_scene_set5_res[7], date_scene_set5_res[8],
                  date_scene_set5_res[9], date_scene_set5_res[10]);
    date_object_set(date_scene_set5_res[1], date_scene_set5_res[2], date_scene_set5_res[0], (s32)(*(s16 *)&date_scene_set5_res[3]));
    girl6_tokimeki = girl6_tokimeki + 1;
    girl6_yuukou = girl6_yuukou + 1;
    girl6_syoushin = girl6_syoushin - 20;
    param_clamp();
    girl = now_girl;
    screen_w[girl * 56 + 477] = screen_w[girl * 56 + 477] + 1;
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
