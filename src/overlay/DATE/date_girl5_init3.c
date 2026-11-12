/* @SymbolStart: 0x801555F4 */
/* @SymbolSize: 276 */
/* @PostProc: scheduler_normalizer --block-aware */
/* Function: date_girl5_init3 */
/* Overlay: DATE */
/* Address: 0x80155618 (true entry 0x801555F4) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_girl5_mode3_bufptr_set();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_girl5_init3_res[];
extern s16 girl5_tokimeki;
extern s16 girl5_yuukou;
extern s16 girl5_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl5_init3(void)
{
    date_ss_serifuev = date_serifuev_tbl[44];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[44];
    date_sss_soundev = date_soundev_tbl[44];
    date_girl5_mode3_bufptr_set();
    palette_upload_enqueue(date_girl5_init3_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl5_init3_res[5], date_girl5_init3_res[6], date_girl5_init3_res[7], date_girl5_init3_res[8],
                  date_girl5_init3_res[9], date_girl5_init3_res[10]);
    date_object_set(date_girl5_init3_res[1], date_girl5_init3_res[2], date_girl5_init3_res[0], (*(s16 *)&date_girl5_init3_res[3]));
    girl5_tokimeki = girl5_tokimeki + 2;
    girl5_yuukou = girl5_yuukou + 1;
    girl5_syoushin = girl5_syoushin - 10;
    param_clamp();
    sub_sub_step_up();
}
