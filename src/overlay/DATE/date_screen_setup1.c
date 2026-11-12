/* @SymbolStart: 0x801546EC */
/* @SymbolSize: 416 */
/* Address: 0x80154710  Size: 380 bytes (95 insns; .sym over-sizes) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 date_bufaddr_init2();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_screen_setup1_res[];
extern volatile s16 girl1_tokimeki;
extern volatile s16 girl1_yuukou;
extern volatile s16 girl1_syoushin;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_screen_setup1(void)
{
    date_ss_serifuev = date_serifuev_tbl[39];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[39];
    date_sss_soundev = date_soundev_tbl[39];
    date_bufaddr_init2();
    palette_upload_enqueue(date_screen_setup1_res[4], 17, 1, 2, 0);
    date_sprite_init(date_screen_setup1_res[5], date_screen_setup1_res[6], date_screen_setup1_res[7], date_screen_setup1_res[8],
                  date_screen_setup1_res[9], date_screen_setup1_res[10]);
    date_object_set(date_screen_setup1_res[1], date_screen_setup1_res[2], date_screen_setup1_res[0], (s32)(*(s16 *)&date_screen_setup1_res[3]));
    girl1_tokimeki = girl1_tokimeki + 1;
    girl1_yuukou = girl1_yuukou + 2;
    girl1_syoushin = girl1_syoushin - 20;
    param_clamp();
    kao[0] = 2;
    kao[2] = 2;
    kao[1] = 2;
    kao2[0] = 2;
    kao2[2] = 2;
    kao2[1] = 2;
    imp[0] = 2;
    imp[2] = 2;
    imp[1] = 2;
    ans[0] = 1;
    ans[2] = 1;
    ans[1] = 1;
    sub_sub_step_up();
}
