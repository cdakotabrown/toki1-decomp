/* @SymbolStart: 0x80157B64 */
/* @SymbolSize: 404 */
/* Address: 0x80157B88  Size: 368 bytes (92 insns; .sym over-sizes) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 strcpy();
extern s32 date_ptrtbl13_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern u8 ba_string;
extern u8 date_screen_setup3_str0;
extern volatile s16 girl9_tokimeki;
extern volatile s16 girl9_yuukou;
extern volatile s16 girl9_syoushin;
extern s32 kuchi_stop;
extern u8 now_girl;
extern s32 date_screen_setup3_res[];
extern u8 date_object_set_tbl0;
extern s16 kao2[];
extern s16 ans[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_screen_setup3(void)
{
    date_ss_serifuev = date_serifuev_tbl[56];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[56];
    date_sss_soundev = date_soundev_tbl[56];
    strcpy(&ba_string, &date_screen_setup3_str0);
    girl9_tokimeki = girl9_tokimeki + 1;
    girl9_yuukou = girl9_yuukou + 1;
    girl9_syoushin = girl9_syoushin - 20;
    param_clamp();
    date_ptrtbl13_init();
    kuchi_stop = 1;
    now_girl = 14;
    palette_upload_enqueue(date_screen_setup3_res[4], 17, 1, 2, 0);
    date_sprite_init(date_screen_setup3_res[5], date_screen_setup3_res[6], date_screen_setup3_res[7], date_screen_setup3_res[8],
                  date_screen_setup3_res[9], date_screen_setup3_res[10]);
    date_object_set(date_screen_setup3_res[1], date_screen_setup3_res[2], date_screen_setup3_res[0], (s32)(*(s16 *)&date_screen_setup3_res[3]));
    date_object_set_tbl0 = 1;
    kao2[0] = 3;
    kao2[1] = 3;
    kao2[2] = 3;
    ans[0] = 2;
    ans[1] = 2;
    ans[2] = 2;
    sub_sub_step_up();
}
