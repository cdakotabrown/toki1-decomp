/* @SymbolStart: 0x80159A88 */
/* @SymbolSize: 268 */
/* Function: date_girl10_mode5_scene_init */
/* Overlay: DATE */
/* Address: 0x80159AD4 */
/* Size: 192 bytes (48 instructions) */
#include "types.h"

extern void param_clamp(void);
extern void date_girl10_mode5_bufptr_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s16 girl10_yuukou[];
extern s32 date_scene_set3_res[];
extern u8  date_object_set_tbl0;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl10_mode5_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[71];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[71];
    date_sss_soundev = date_soundev_tbl[71];
    girl10_yuukou[0] += 2;
    girl10_yuukou[2] -= 10;
    param_clamp();
    date_girl10_mode5_bufptr_init();
    palette_upload_enqueue(date_scene_set3_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set3_res[5], date_scene_set3_res[6], date_scene_set3_res[7], date_scene_set3_res[8], date_scene_set3_res[9],
                  date_scene_set3_res[10]);
    date_object_set(date_scene_set3_res[1], date_scene_set3_res[2], date_scene_set3_res[0], (*(s16 *)&date_scene_set3_res[3]));
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
