/* @SymbolStart: 0x8014D010 */
/* @SymbolSize: 504 */
/* Function: date_girl8_mode2_scene_setup -- DATE overlay */
/* Address: 0x8014D05C, Size: 428 bytes (107 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void param_clamp(void);
extern void date_bufptr7dd3_init(void);
extern void date_girl8_mode2_sub_res_ptr_set(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void class_clear_n(s32 a);
extern void sub_sub_step_up(void);

extern s16 girl8_tokimeki[];
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_ev37_res;
extern s32 date_ev37_res_cels;
extern s32 date_ev37_res_anim_tbl;
extern s16 date_ev37_res_scale;
extern s32 date_ev37_res_pal;
extern s32 date_ev37_res_bg0;
extern s32 date_ev37_res_bg1;
extern s32 date_ev37_res_bg2;
extern s32 date_ev37_res_bg3;
extern s32 date_ev37_res_bg4;
extern s32 date_ev37_res_bg5;
extern u8  date_object_set_tbl0;
extern s32 date_ev37_sub_res_pal;
extern s32 date_ev37_sub_res;
extern s32 date_ev37_sub_res_img;
extern s32 date_ev37_sub_res_anim_tbl;
extern s16 date_ev37_sub_res_aux;
extern s16 date_anim_ptn;
extern s16 cw98_w14;
extern s16 cw98_anim;
extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;
extern CLASS_W class_w[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl8_mode2_scene_setup(void)
{
    date_ss_serifuev = date_serifuev_tbl[1];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[1];
    date_sss_soundev = date_soundev_tbl[1];
    girl8_tokimeki[2] += 3;
    girl8_tokimeki[4] -= 10;
    param_clamp();
    date_bufptr7dd3_init();
    palette_upload_enqueue(date_ev37_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev37_res_bg0, date_ev37_res_bg1, date_ev37_res_bg2, date_ev37_res_bg3,
                  date_ev37_res_bg4, date_ev37_res_bg5);
    date_object_set(date_ev37_res_cels, date_ev37_res_anim_tbl, date_ev37_res, (s32)date_ev37_res_scale);
    date_object_set_tbl0 = 1;
    date_girl8_mode2_sub_res_ptr_set();
    palette_upload_enqueue(date_ev37_sub_res_pal, 18, 1, 2, 0);
    class_clear_n(98);
    class_w[98].prio = 12;
    class_w[98].anim_flag = 0;
    (*(s32 *)&class_w[98].attribute) = 0x41000000;
    class_w[98].disp_flag = 4;
    (*(s32 *)&class_w[98].cels) = date_ev37_sub_res_img;
    (*(s32 *)&class_w[98].hdr) = date_ev37_sub_res;
    (*(s32 *)&class_w[98].anim_tbl) = date_ev37_sub_res_anim_tbl;
    cw98_w14 = (s16)date_ev37_sub_res_aux;
    cw98_anim = 0;
    cw98_frame = 0;
    class_w[98].rate = 1;
    class_w[98].clut = 18;
    cw98_x = -160;
    cw98_y = -120;
    class_w[98].bright = 0;
    class_w[98].tpage_sel = 0;
    date_anim_ptn = 0;
    sub_sub_step_up();
}
