/* @SymbolSize: 424 */
#include "types.h"

extern void shugaku_ptr_set(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void class_clear_n(s32 a0);
extern void device_cmd_send(s32 a0);

extern volatile u8 shugaku_place;
extern s32 shugaku_cw96_res_pal;
extern s32 shugaku_cw96_res;
extern s32 shugaku_cw96_res_img;
extern s32 shugaku_cw96_res_anim_tbl;
extern s16 shugaku_cw96_res_aux;

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern s32 cal_cw_attr;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8  cal_cw_rate;
extern u8  cal_cw_clut;
extern s16 cal_cw_y;
extern s16 cal_cw_x;
extern s16 shugaku_init_val;
extern u8  cal_cw_bright;
extern u8  cal_cw_sel;
extern u8  cal_cw_flag4;
extern s32 step_counter;

void shugaku_init(void)
{
    s32 v0;

    shugaku_ptr_set();
    v0 = (s32)shugaku_place;
    switch ((u32)v0) {
    case 0:
        palette_upload_enqueue(shugaku_cw96_res_pal, 17, 0, 0, 1);
        break;
    case 1:
        palette_upload_enqueue(shugaku_cw96_res_pal, 17, 0, 0, 0);
        break;
    default:
        palette_upload_enqueue(shugaku_cw96_res_pal, 17, 0, 0, 2);
        break;
    }
    class_clear_n(96);
    cal_cw_prio = 10;
    cal_cw_mode = 1;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 132;
    cal_cw_img = shugaku_cw96_res_img;
    cal_cw_anim_tbl = shugaku_cw96_res_anim_tbl;
    cal_cw_hdr = shugaku_cw96_res;
    cal_cw_aux14 = shugaku_cw96_res_aux;
    cal_cw_anim = 0;
    cal_cw_frame = 0;
    cal_cw_rate = 1;
    cal_cw_clut = 17;
    v0 = (s32)shugaku_place;
    cal_cw_x = 0;
    cal_cw_y = -120;
    if (v0 == 0) {
        cal_cw_y = -136;
    }
    cal_cw_bright = 128;
    cal_cw_sel = 1;
    cal_cw_flag4 = 0;
    device_cmd_send(1281);
    step_counter = step_counter + 1;
}
