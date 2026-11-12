/* @SymbolStart: 0x80155ABC */
/* @SymbolSize: 324 */
/* Address: 0x80155AE0  Size: 288 bytes (72 insns; .sym may over-size) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 date_girl5_mode6_bufptr_set();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern u32 date_screen_setup2_bg[];
extern volatile s16 girl5_tokimeki;
extern volatile s16 girl5_yuukou;
extern volatile s16 girl5_syoushin;
extern s16 kao2[];
extern s16 ans[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_screen_setup2(void)
{
    date_ss_serifuev = date_serifuev_tbl[47];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[47];
    date_sss_soundev = date_soundev_tbl[47];
    date_girl5_mode6_bufptr_set();
    palette_upload_enqueue((*(s32 *)&date_screen_setup2_bg[0]), 17, 1, 2, 0);
    date_sprite_init((*(s32 *)&date_screen_setup2_bg[1]), (*(s32 *)&date_screen_setup2_bg[2]), (*(s32 *)&date_screen_setup2_bg[3]), (*(s32 *)&date_screen_setup2_bg[4]),
                  (*(s32 *)&date_screen_setup2_bg[5]), (*(s32 *)&date_screen_setup2_bg[6]));
    date_object_set(0, 0, 0, 0);
    girl5_tokimeki = girl5_tokimeki + 1;
    girl5_yuukou = girl5_yuukou + 1;
    girl5_syoushin = girl5_syoushin - 20;
    param_clamp();
    kao2[0] = 2;
    kao2[1] = 2;
    kao2[2] = 2;
    ans[0] = 1;
    ans[1] = 1;
    ans[2] = 1;
    sub_sub_step_up();
}
