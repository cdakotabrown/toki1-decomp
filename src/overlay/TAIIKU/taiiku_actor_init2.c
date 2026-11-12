/* @SymbolSize: 268 */
/* Address: 0x80143644 */
/* Size: 268 bytes (67 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;

extern s32 taiiku_spr_img_4;
extern s32 taiiku_spr_anim_4;
extern u8 taiiku_ninin_girl1;
extern u8 taiiku_ninin_pl_tpg[];
extern CLASS_W class_w[];
extern s32 taiiku_spr_aux[];

extern s32 class_clear_n(s32 a0);

void taiiku_actor_init2(void)
{
    class_clear_n(96);
    cal_cw_prio = 3;
    cal_cw_attr = 0x01000000;
    cal_cw_mode = 1;
    cal_cw_disp = 164;
    cal_cw_bright = 128;
    cal_cw_img = taiiku_spr_img_4;
    cal_cw_anim_tbl = taiiku_spr_anim_4;
    cal_cw_hdr = (s32)taiiku_ninin_pl_tpg;
    cal_cw_aux14 = (*(s16 *)&taiiku_spr_aux[4]);
    cal_cw_flag4 = 0;
    cal_cw_sel = 128;
    if (taiiku_ninin_girl1 == 0) {
        cal_cw_anim = 0;
    } else {
        cal_cw_anim = 2;
    }
    cal_cw_x = 80;
    cal_cw_y = 176;
    class_w[96].scale_x = 4096;
    class_w[96].scale_y = 4096;
    cal_cw_rate = 2;
}
