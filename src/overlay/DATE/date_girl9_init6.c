/* @SymbolStart: 0x80157F74 */
/* @SymbolSize: 280 */
/* date_girl9_init6 - DATE overlay, Address: 0x80157FD4, Size: 184 bytes (46 insns) */
#include "types.h"

extern s16 girl9_tokimeki[];

extern s32 date_girl9_init6_res[];

extern void date_ptrtbl16_init(void);
extern void sub_sub_step_up(void);
extern void param_clamp(void);
extern void palette_upload_enqueue();
extern void date_sprite_init();
extern void date_object_set();
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl9_init6(void) {
    date_ss_serifuev = date_serifuev_tbl[59];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[59];
    date_sss_soundev = date_soundev_tbl[59];
    girl9_tokimeki[0] += 3;
    girl9_tokimeki[2] += 2;
    girl9_tokimeki[4] -= 20;
    param_clamp();
    date_ptrtbl16_init();
    palette_upload_enqueue(date_girl9_init6_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl9_init6_res[5], date_girl9_init6_res[6], date_girl9_init6_res[7], date_girl9_init6_res[8],
                  date_girl9_init6_res[9], date_girl9_init6_res[10]);
    date_object_set(date_girl9_init6_res[1], date_girl9_init6_res[2], date_girl9_init6_res[0], (*(s16 *)&date_girl9_init6_res[3]));
    sub_sub_step_up();
}
