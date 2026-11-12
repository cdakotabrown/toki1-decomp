/* @SymbolSize: 740 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp */
/* Function: date_girl8_mode3_scene_setup */
/* Overlay: DATE */
/* Address: 0x8014D208 */
/* Size: 740 bytes (185 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s32 w[17]; } CopyBlock;

extern u8 day_flag;
extern s16 girl8_tokimeki;
extern s16 girl8_yuukou;
extern s16 girl8_syoushin;

extern s32 ba_string2;
extern s32 date_scene_setup2_str0;

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 date_ev38_res_pal;
extern s32 date_ev38_res_bg0;
extern s32 date_ev38_res_bg1;
extern s32 date_ev38_res_bg2;
extern s32 date_ev38_res_bg3;
extern s32 date_ev38_res_bg4;
extern s32 date_ev38_res_bg5;
extern s32 date_ev38_res;
extern s32 date_ev38_res_cels;
extern s32 date_ev38_res_anim_tbl;
extern s16 date_ev38_res_scale;


extern u8 date_object_set_tbl0;
extern s16 cw98_frame;

extern s16 date_anim_ptn;
extern s16 date_anim_ptn2;
extern s16 date_anim_ptn2_dir;
extern s32 tpage_buf[];
extern CLASS_W class_w[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

extern s32 strcpy();
extern s32 param_clamp();
extern s32 date_bufptr7e10_init();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 sub_sub_step_up();

void date_girl8_mode3_scene_setup(void)
{
    s32 pad;
    CopyBlock tmp;

    strcpy((s32)&ba_string2, (s32)&date_scene_setup2_str0);
    day_flag |= 7;
    date_ss_serifuev = date_serifuev_tbl[2];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[2];
    date_sss_soundev = date_soundev_tbl[2];
    girl8_tokimeki = (s16)(girl8_tokimeki + 3);
    girl8_yuukou = (s16)(girl8_yuukou + 2);
    girl8_syoushin = (s16)(girl8_syoushin - 20);
    param_clamp();
    tpage_buf[26] = -1;
    tpage_buf[58] = -1;
    date_bufptr7e10_init();
    palette_upload_enqueue(date_ev38_res_pal, 17, 1, 2, 0);
    palette_upload_enqueue(date_ev38_res_pal, 18, 1, 2, 4);
    palette_upload_enqueue(date_ev38_res_pal, 19, 1, 2, 0);
    date_sprite_init(date_ev38_res_bg0, date_ev38_res_bg1, date_ev38_res_bg2, date_ev38_res_bg3,
                  date_ev38_res_bg4, date_ev38_res_bg5);
    date_object_set(date_ev38_res_cels, date_ev38_res_anim_tbl, date_ev38_res, (s32)date_ev38_res_scale);

    date_object_set_tbl0 = 1;
    tmp = (*(CopyBlock *)&class_w[96]);
    (*(CopyBlock *)&class_w[99]) = tmp;
    (*(CopyBlock *)&class_w[98]) = tmp;

    class_w[99].anim_flag = 64;
    class_w[98].anim_flag = 64;
    cw98_frame = 8;
    class_w[98].prio = 12;
    class_w[98].clut = 19;
    class_w[99].frame = 9;
    class_w[99].prio = 12;
    class_w[99].clut = 18;
    date_anim_ptn2 = 0;
    date_anim_ptn = 0;
    date_anim_ptn2_dir = 1;
    sub_sub_step_up();
}
