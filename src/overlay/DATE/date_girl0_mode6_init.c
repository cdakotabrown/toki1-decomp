/* @SymbolStart: 0x8014F7E4 */
/* @SymbolSize: 252 */
/* @PostProc: scheduler_normalizer --store-fp --const-fp */
/* Function: date_girl0_mode6_init */
/* Overlay: DATE */
/* Address: 0x8014F7E4 (cross-prologue head) / 0x8014F808 (body) */
/* Size: 252 bytes (63 instructions) */
#include "types.h"

extern void param_clamp(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_girl0_mode6_bg[];
extern s16 girl0_tokimeki;
extern s16 girl0_yuukou;
extern s16 girl0_syoushin;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl0_mode6_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[11];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[11];
    date_sss_soundev = date_soundev_tbl[11];
    palette_upload_enqueue(date_girl0_mode6_bg[0], 17, 1, 2, 0);
    date_sprite_init(date_girl0_mode6_bg[1], date_girl0_mode6_bg[2], date_girl0_mode6_bg[3], date_girl0_mode6_bg[4], date_girl0_mode6_bg[5],
                  date_girl0_mode6_bg[6]);
    date_object_set(0, 0, 0, 0);
    girl0_tokimeki = (s16)(girl0_tokimeki + 1);
    girl0_yuukou = (s16)(girl0_yuukou + 2);
    girl0_syoushin = (s16)(girl0_syoushin - 20);
    param_clamp();
    sub_sub_step_up();
}
