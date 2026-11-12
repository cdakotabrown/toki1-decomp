/* @SymbolSize: 240 */
/* Address: 0x80146150 */
/* Size: 240 bytes (60 insns) */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern volatile u8 cal_cw_prio;
extern volatile u8 cal_cw_mode;
extern volatile u8 cal_cw_disp;
extern volatile u8 cal_cw_flag4;
extern volatile u8 cal_cw_sel;
extern volatile u8 cal_cw_rate;
extern volatile u8 cal_cw_bright;
extern volatile s16 cal_cw_aux14;
extern volatile s16 cal_cw_anim;
extern volatile s16 cw96_scale_x;
extern volatile s16 cw96_scale_y;
extern volatile s16 cal_cw_x;
extern volatile s16 cal_cw_y;
extern volatile s32 cal_cw_img;
extern volatile s32 cal_cw_anim_tbl;
extern volatile s32 cal_cw_hdr;
extern volatile s32 cal_cw_attr;

extern volatile s32 taiiku_spr_img_8;
extern volatile s32 taiiku_spr_anim_8;
extern volatile s16 taiiku_spr_aux_8;
extern u8 taiiku_spoon_pl_tpg[];

extern s32 class_clear_n(s32 a0);

void taiiku_panel_init(void)
{
    class_clear_n(96);
    cal_cw_prio = 3;
    cal_cw_attr = 0x01000000;
    cal_cw_mode = 1;
    cal_cw_disp = 164;
    cal_cw_bright = 128;
    cal_cw_img = taiiku_spr_img_8;
    cal_cw_anim_tbl = taiiku_spr_anim_8;
    cal_cw_hdr = (s32)taiiku_spoon_pl_tpg;
    cal_cw_aux14 = taiiku_spr_aux_8;
    cal_cw_flag4 = 0;
    cal_cw_sel = 128;
    cal_cw_anim = 0;
    cal_cw_x = 32;
    cal_cw_y = 16;
    cw96_scale_x = 4096;
    cw96_scale_y = 4096;
    cal_cw_rate = 2;
}
