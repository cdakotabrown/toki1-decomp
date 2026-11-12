/* @SymbolStart: 0x80150338 */
/* @SymbolSize: 284 */
/* Function: date_girl2_mode2_scene_init (true entry 0x80150338) */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 k_reset();
extern s32 basyo_init();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_bufptr8144_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern u8 date_scene_init1_str0[];
extern s32 date_scene_init1_bg[];
extern volatile s16 girl2_tokimeki;
extern volatile s16 girl2_yuukou;
extern volatile s16 girl2_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl2_mode2_scene_init(s32 a0, s32 a1, s32 a2, s32 a3)
{
    date_ss_serifuev = date_serifuev_tbl[15];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[15];
    date_sss_soundev = date_soundev_tbl[15];
    date_bufptr8144_init(a0, a1, a2, a3);
    k_reset(0);
    basyo_init(date_scene_init1_str0);
    k_reset(1);
    palette_upload_enqueue(date_scene_init1_bg[0], 17, 1, 2, 0);
    date_sprite_init(date_scene_init1_bg[1], date_scene_init1_bg[2], date_scene_init1_bg[3], date_scene_init1_bg[4],
                  date_scene_init1_bg[5], date_scene_init1_bg[6]);
    date_object_set(0, 0, 0, 0);
    girl2_tokimeki += 3;
    girl2_yuukou += 2;
    girl2_syoushin -= 20;
    param_clamp();
    return sub_sub_step_up();
}
