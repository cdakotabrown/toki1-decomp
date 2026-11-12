/* @SymbolStart: 0x80157E18 */
/* @SymbolSize: 348 */
/* Function: date_girl9_init5 */
/* Overlay: DATE */
/* Address: 0x80157E78 */
/* Size: 252 bytes (63 instructions) */
#include "types.h"

extern void param_clamp(void);
extern void date_ptrtbl15_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s16 girl9_tokimeki[];
extern s32 date_girl9_init5_res[];
extern s16 kao2[];
extern s16 ans[];
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl9_init5(void)
{
    date_ss_serifuev = date_serifuev_tbl[58];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[58];
    date_sss_soundev = date_soundev_tbl[58];
    girl9_tokimeki[0]++;
    girl9_tokimeki[2]++;
    girl9_tokimeki[4] -= 20;
    param_clamp();
    date_ptrtbl15_init();
    palette_upload_enqueue(date_girl9_init5_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl9_init5_res[5], date_girl9_init5_res[6], date_girl9_init5_res[7], date_girl9_init5_res[8], date_girl9_init5_res[9],
                  date_girl9_init5_res[10]);
    date_object_set(date_girl9_init5_res[1], date_girl9_init5_res[2], date_girl9_init5_res[0], (*(s16 *)&date_girl9_init5_res[3]));
    kao2[0] = 2;
    kao2[1] = 2;
    kao2[2] = 2;
    ans[0] = 1;
    ans[1] = 1;
    ans[2] = 1;
    sub_sub_step_up();
}
