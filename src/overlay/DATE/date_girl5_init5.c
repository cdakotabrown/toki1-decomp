/* @SymbolStart: 0x801559A8 */
/* @SymbolSize: 276 */
/* Function: date_girl5_init5  (true start 0x801559A8) */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_girl5_mode5_bufptr_set();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 date_girl5_init5_res[];

extern volatile s16 girl5_tokimeki;
extern volatile s16 girl5_yuukou;
extern volatile s16 girl5_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl5_init5(void)
{
    date_ss_serifuev = date_serifuev_tbl[46];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[46];
    date_sss_soundev = date_soundev_tbl[46];
    date_girl5_mode5_bufptr_set();
    palette_upload_enqueue(date_girl5_init5_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl5_init5_res[5], date_girl5_init5_res[6], date_girl5_init5_res[7], date_girl5_init5_res[8],
                  date_girl5_init5_res[9], date_girl5_init5_res[10]);
    date_object_set(date_girl5_init5_res[1], date_girl5_init5_res[2], date_girl5_init5_res[0], (*(s16 *)&date_girl5_init5_res[3]));
    girl5_tokimeki = girl5_tokimeki + 3;
    girl5_yuukou = girl5_yuukou + 2;
    girl5_syoushin = girl5_syoushin - 20;
    param_clamp();
    return sub_sub_step_up();
}
