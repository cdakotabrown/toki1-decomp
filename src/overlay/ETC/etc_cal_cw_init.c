/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=7:13:13,7:17:17 */
/* @SymbolSize: 740 */
/* Address: 0x8013F468 */
/* Size: 740 bytes (185 instructions) */
#include "types.h"
#include "tokimeki/etc_cal_cw_res.h"
#include "tokimeki/sprite.h"
#include "tokimeki/class_w.h"

extern void class_clear_n(s32);
extern void etc_buf_init(void);
extern void etc_record_reset(void);

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;
extern s32 etc_rec_girl;
extern s32 etc_rec_month;
extern s32 etc_rec_youbi;
extern CLASS_W class_w[];
extern SPRITE_REC Sprites[];
extern ETC_CAL_CW_RES etc_cal_cw_res;

void etc_cal_cw_init(void)
{
    s32 stage;

    etc_record_reset();
    etc_buf_init();
    class_clear_n(96);

    cal_cw_prio = 0;
    cal_cw_attr = 0x01000000;
    cal_cw_bright = 128;
    cal_cw_sel = 128;
    class_w[96].scale_x = 4096;
    class_w[96].scale_y = 4096;
    cal_cw_rate = 2;
    cal_cw_clut = 16;
    cal_cw_x = 0;
    cal_cw_y = -13;
    cal_cw_img = etc_cal_cw_res.img;
    cal_cw_anim_tbl = etc_cal_cw_res.anim_tbl;
    cal_cw_hdr = etc_cal_cw_res.hdr;
    cal_cw_aux14 = etc_cal_cw_res.aux;
    cal_cw_mode = 1;
    cal_cw_disp = 132;
    cal_cw_frame = 2;

    stage = etc_rec_girl;
    if (stage == 0) {
        if (etc_rec_month == 6 || etc_rec_month == 7 || etc_rec_month == 8 ||
            etc_rec_month == 9) {
            cal_cw_anim = 0;
        } else {
            cal_cw_anim = 1;
        }
    }
    if (stage == 1) {
        if (etc_rec_month == 6 || etc_rec_month == 7 || etc_rec_month == 8 ||
            etc_rec_month == 9) {
            cal_cw_anim = 2;
        } else {
            cal_cw_anim = 3;
        }
    }
    if (stage == 4) {
        if (etc_rec_month == 6 || etc_rec_month == 7 || etc_rec_month == 8 ||
            etc_rec_month == 9) {
            cal_cw_anim = 4;
        } else {
            cal_cw_anim = 5;
        }
        if (etc_rec_youbi == 255) {
            Sprites[0].tpage = 7;
            Sprites[0].u = 112;
        }
    }
    if (stage == 6) {
        if (etc_rec_month == 6 || etc_rec_month == 7 || etc_rec_month == 8 ||
            etc_rec_month == 9) {
            cal_cw_anim = 6;
        } else {
            cal_cw_anim = 7;
            cal_cw_y = -18;
        }
    }
    if (stage == 9) {
        if (etc_rec_month == 6 || etc_rec_month == 7 || etc_rec_month == 8 ||
            etc_rec_month == 9) {
            cal_cw_anim = 8;
        } else {
            cal_cw_anim = 9;
        }
        if (etc_rec_youbi == 255) {
            Sprites[0].tpage = 7;
            Sprites[0].u = 112;
        }
    }
    if (stage == 10) {
        if (etc_rec_month == 6 || etc_rec_month == 7 || etc_rec_month == 8 ||
            etc_rec_month == 9) {
            cal_cw_anim = 10;
        } else {
            cal_cw_anim = 11;
        }
    }
}
