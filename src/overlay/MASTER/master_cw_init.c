/* master_cw_init - MASTER overlay, Address: 0x80137F94, Size: 244 bytes (61 insns) */
/* @SymbolSize: 244 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void master_cal_res_set(void);
extern void class_clear_n(s32);

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;

extern u32 master_cal_res_tbl[];
extern CLASS_W class_w[];

void master_cw_init(void) {
    master_cal_res_set();
    class_clear_n(96);
    cal_cw_prio = 3;
    cal_cw_attr = 0x01000000;
    cal_cw_bright = 0;
    cal_cw_sel = 128;
    class_w[96].scale_x = 4096;
    class_w[96].scale_y = 4096;
    cal_cw_rate = 2;
    cal_cw_clut = 16;
    cal_cw_x = 0;
    cal_cw_y = -32;
    cal_cw_img = (*(s32 *)&master_cal_res_tbl[1]);
    cal_cw_anim_tbl = (*(s32 *)&master_cal_res_tbl[2]);
    cal_cw_hdr = (*(s32 *)&master_cal_res_tbl[0]);
    cal_cw_aux14 = (*(s16 *)&master_cal_res_tbl[3]);
    cal_cw_mode = 1;
    cal_cw_disp = 4;
    cal_cw_frame = 0;
}
