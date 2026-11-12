/* @SymbolStart: 0x80151A20 */
/* @SymbolSize: 564 */
/* Address: 0x80151A44  Size: 528 bytes (132 insns; .sym over-sizes) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 class_clear_n();
extern s32 date_ptrtbl6_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_ev17_res_pal;
extern s32 date_ev17_res_bg0;
extern s32 date_ev17_res_bg1;
extern s32 date_ev17_res_bg2;
extern s32 date_ev17_res_bg3;
extern s32 date_ev17_res_bg4;
extern s32 date_ev17_res_bg5;
extern s32 date_ev17_res;
extern s32 date_ev17_res_cels;
extern s32 date_ev17_res_anim_tbl;
extern s16 date_ev17_res_scale;
extern u8 date_object_set_tbl0;
extern s16 cw98_w14;
extern s16 cw98_anim;
extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;
extern volatile s16 girl3_tokimeki;
extern volatile s16 girl3_yuukou;
extern volatile s16 girl3_syoushin;
extern s16 kao2[];
extern s16 ans[];
extern CLASS_W class_w[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl3_mode3_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[26];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[26];
    date_sss_soundev = date_soundev_tbl[26];
    date_ptrtbl6_init();
    palette_upload_enqueue(date_ev17_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev17_res_bg0, date_ev17_res_bg1, date_ev17_res_bg2, date_ev17_res_bg3,
                  date_ev17_res_bg4, date_ev17_res_bg5);
    date_object_set(date_ev17_res_cels, date_ev17_res_anim_tbl, date_ev17_res, (s32)date_ev17_res_scale);
    date_object_set_tbl0 = 1;
    class_clear_n(98);
    class_w[98].prio = 9;
    class_w[98].anim_flag = 0;
    (*(s32 *)&class_w[98].attribute) = 0x41000000;
    class_w[98].disp_flag = 4;
    (*(s32 *)&class_w[98].cels) = date_ev17_res_cels;
    (*(s32 *)&class_w[98].anim_tbl) = date_ev17_res_anim_tbl;
    (*(s32 *)&class_w[98].hdr) = date_ev17_res;
    cw98_w14 = date_ev17_res_scale;
    cw98_anim = 4;
    cw98_frame = 0;
    class_w[98].rate = 1;
    class_w[98].clut = 17;
    cw98_x = -160;
    cw98_y = -120;
    class_w[98].bright = 0;
    class_w[98].tpage_sel = 0;
    class_w[98].cel_mode = 0;
    girl3_tokimeki = girl3_tokimeki - 1;
    girl3_yuukou = girl3_yuukou - 1;
    girl3_syoushin = girl3_syoushin + 20;
    param_clamp();
    kao2[0] = 3;
    kao2[1] = 3;
    kao2[2] = 3;
    ans[0] = 2;
    ans[1] = 2;
    ans[2] = 2;
    sub_sub_step_up();
}
