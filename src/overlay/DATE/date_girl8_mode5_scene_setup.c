/* @SymbolStart: 0x8014D620 */
/* @SymbolSize: 360 */
/* Function: date_girl8_mode5_scene_setup */
/* Overlay: DATE */
/* Address: 0x8014D680 */
#include "types.h"

extern void param_clamp(void);
extern void date_girl8_mode5_res_ptr_set(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s16 girl8_tokimeki[];
extern s32 date_scene_setup4_res[];
extern u8  date_object_set_tbl0;
extern s16 kao2[];
extern s16 ans[];
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl8_mode5_scene_setup(void)
{
    date_ss_serifuev = date_serifuev_tbl[4];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[4];
    date_sss_soundev = date_soundev_tbl[4];
    girl8_tokimeki[0]++;
    girl8_tokimeki[2]++;
    girl8_tokimeki[4] -= 20;
    param_clamp();
    date_girl8_mode5_res_ptr_set();
    palette_upload_enqueue(date_scene_setup4_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_setup4_res[5], date_scene_setup4_res[6], date_scene_setup4_res[7], date_scene_setup4_res[8], date_scene_setup4_res[9],
                  date_scene_setup4_res[10]);
    date_object_set(date_scene_setup4_res[1], date_scene_setup4_res[2], date_scene_setup4_res[0], (*(s16 *)&date_scene_setup4_res[3]));
    date_object_set_tbl0 = 1;
    kao2[0] = 3;
    kao2[1] = 3;
    kao2[2] = 3;
    ans[0] = 2;
    ans[1] = 2;
    ans[2] = 2;
    sub_sub_step_up();
}
