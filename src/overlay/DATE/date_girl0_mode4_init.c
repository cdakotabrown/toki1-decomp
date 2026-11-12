/* Address: 0x8014F514 (true start 0x8014F4F0: cross-prologue fall-through */
/* @SymbolStart: 0x8014F4F0 */
/* @SymbolSize: 272 */
/* @PostProc: scheduler_normalizer --block-aware */
#include "types.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void param_clamp(void);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_girl0_mode4_res[];
extern s16 girl0_tokimeki;
extern s16 girl0_yuukou;
extern s16 girl0_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl0_mode4_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[9];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[9];
    date_sss_soundev = date_soundev_tbl[9];
    palette_upload_enqueue(date_girl0_mode4_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl0_mode4_res[5], date_girl0_mode4_res[6], date_girl0_mode4_res[7], date_girl0_mode4_res[8],
                  date_girl0_mode4_res[9], date_girl0_mode4_res[10]);
    date_object_set(date_girl0_mode4_res[1], date_girl0_mode4_res[2], date_girl0_mode4_res[0], (s32)(*(s16 *)&date_girl0_mode4_res[3]));
    girl0_tokimeki = (s16)(girl0_tokimeki + 3);
    girl0_yuukou = (s16)(girl0_yuukou + 2);
    girl0_syoushin = (s16)(girl0_syoushin - 20);
    param_clamp();
    sub_sub_step_up();
}
