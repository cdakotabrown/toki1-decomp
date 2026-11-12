/* @SymbolStart: 0x8013A750 */
/* @SymbolSize: 300 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);

extern s32 shugaku_cw97_res_pal;
extern s32 shugaku_cw97_res_img;
extern s32 shugaku_cw97_res_anim_tbl;
extern s32 shugaku_cw97_res;
extern s16 shugaku_cw97_res_aux_s;

extern u8  cw97_prio;
extern u8  cw97_mode;
extern s32 cw97_attr;
extern u8  cw97_disp;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s32 cw97_hdr;
extern s32 shugaku_cw97_val;
extern s16 cw97_w14;
extern s16 cw97_anim;
extern s16 cw97_frame;
extern u8  cw97_rate;
extern u8  cw97_clut;
extern s16 cw97_x;
extern s16 cw97_y;
extern u8  cw97_bright;
extern u8  cw97_sel;

extern u8 shugaku_place;
extern CLASS_W class_w[];

void shugaku_cw97_set(void)
{
    if (shugaku_place != 1) goto lbl_8013A864;
    palette_upload_enqueue(shugaku_cw97_res_pal, 17, 0, 1, 0);
    class_clear_n(97);
    cw97_prio = 12;
    cw97_mode = 1;
    cw97_attr = 0x01000000;
    cw97_disp = 132;
    cw97_img = shugaku_cw97_res_img;
    cw97_anim_tbl = shugaku_cw97_res_anim_tbl;
    cw97_hdr = shugaku_cw97_res;
    cw97_w14 = shugaku_cw97_res_aux_s;
    cw97_anim = 0;
    cw97_frame = 0;
    cw97_rate = 1;
    cw97_clut = 17;
    cw97_x = 16;
    cw97_y = -119;
    cw97_bright = 0;
    cw97_sel = 1;
    class_w[97].cel_mode = 0;
    sub_sub_step_up();
    goto lbl_8013A870;
lbl_8013A864:
    sub_sub_step_up();
lbl_8013A870:
    return;
}
