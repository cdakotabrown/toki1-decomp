/* @SymbolStart: 0x8015347C */
/* @SymbolSize: 348 */
/* Function: date_girl7_mode3_scene_init */
/* Overlay: DATE */
/* Address: 0x801534DC */
/* Size: 252 bytes (63 instructions) */
#include "types.h"

extern void param_clamp(void);
extern void date_girl7_mode3_bufptr_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s16 girl7_tokimeki[];
extern s32 date_scene_prep2_res[];
extern s16 kao2[];
extern s16 ans[];
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl7_mode3_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[32];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[32];
    date_sss_soundev = date_soundev_tbl[32];
    girl7_tokimeki[0]++;
    girl7_tokimeki[2]++;
    girl7_tokimeki[4] -= 20;
    param_clamp();
    date_girl7_mode3_bufptr_init();
    palette_upload_enqueue(date_scene_prep2_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_prep2_res[5], date_scene_prep2_res[6], date_scene_prep2_res[7], date_scene_prep2_res[8], date_scene_prep2_res[9],
                  date_scene_prep2_res[10]);
    date_object_set(date_scene_prep2_res[1], date_scene_prep2_res[2], date_scene_prep2_res[0], (*(s16 *)&date_scene_prep2_res[3]));
    kao2[0] = 3;
    kao2[1] = 3;
    kao2[2] = 3;
    ans[0] = 2;
    ans[1] = 2;
    ans[2] = 2;
    sub_sub_step_up();
}
