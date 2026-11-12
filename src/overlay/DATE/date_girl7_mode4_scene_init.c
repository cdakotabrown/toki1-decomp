/* @SymbolStart: 0x801535D8 */
/* @SymbolSize: 468 */
/* Function: date_girl7_mode4_scene_init */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 strcpy();
extern s32 date_bufptr84f7_init();

extern u8 date_ride_kind;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern u8 ba_string2;
extern u8 date_scene_prep3_str0;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern s32 date_scene_prep3_res[];
extern u8 date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl7_mode4_scene_init(void)
{
    date_ride_kind = 1;
    date_ss_serifuev = date_serifuev_tbl[33];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[33];
    date_sss_soundev = date_soundev_tbl[33];
    strcpy(&ba_string2, &date_scene_prep3_str0);
    kao[0] = 2;
    kao[1] = 2;
    kao[2] = 2;
    kao2[0] = 2;
    kao2[1] = 2;
    kao2[2] = 2;
    imp[0] = 2;
    imp[1] = 2;
    imp[2] = 2;
    ans[0] = 1;
    ans[1] = 1;
    ans[2] = 1;
    date_bufptr84f7_init();
    palette_upload_enqueue(date_scene_prep3_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_prep3_res[5], date_scene_prep3_res[6], date_scene_prep3_res[7], date_scene_prep3_res[8],
                  date_scene_prep3_res[9], date_scene_prep3_res[10]);
    date_object_set(date_scene_prep3_res[1], date_scene_prep3_res[2], date_scene_prep3_res[0], (s32)(*(s16 *)&date_scene_prep3_res[3]));
    date_object_set_tbl0 = 1;
    kao2[0] = 3;
    kao2[1] = 3;
    kao2[2] = 3;
    ans[0] = 2;
    ans[1] = 2;
    ans[2] = 2;
    sub_sub_step_up();
}
