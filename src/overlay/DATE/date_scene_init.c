#include "types.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void class_clear_n(s32 a0);
extern void date_buf_tbl3_init(void);

extern s32 date_aug_res;
extern s32 date_aug_res_img;
extern s32 date_aug_res_anim_tbl;
extern s16 date_aug_res_aux;
extern s32 date_aug_res_pal;
extern u8  date_aug_pal_ptn;

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_rate;
extern u8  cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8  cal_cw_clut;

void date_scene_init(void)
{
    date_buf_tbl3_init();
    palette_upload_enqueue(date_aug_res_pal, 17, 1, 3, 0);
    class_clear_n(96);

    cal_cw_prio = 12;
    cal_cw_mode = 64;
    cal_cw_attr = 0x41000000;
    cal_cw_disp = 4;
    cal_cw_img = date_aug_res_img;
    cal_cw_anim_tbl = date_aug_res_anim_tbl;
    cal_cw_hdr = date_aug_res;
    cal_cw_aux14 = date_aug_res_aux;
    cal_cw_anim = 0;
    cal_cw_frame = 0;
    cal_cw_rate = 1;
    cal_cw_clut = 17;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_bright = 128;
    date_aug_pal_ptn = 0;

    sub_sub_step_up();
}
