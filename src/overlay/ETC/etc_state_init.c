/* @SymbolSize: 228 */
/* Address: 0x80146C54 */
/* Size: 228 bytes (57 instructions) */
#include "types.h"

extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void class_clear_n(s32 a);

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern u8  cal_cw_rate;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8  cal_cw_clut;
extern s32 bustup_clut;
extern s32 bustup_img;
extern s32 bustup_img2;
extern s32 bustup_img3;

void etc_state_init(s32 arg)
{
    class_clear_n(96);
    palette_upload_enqueue(bustup_clut, 16, 1, 1, 0);
    cal_cw_prio = 1;
    cal_cw_mode = 4;
    cal_cw_attr = 0x1000000;
    cal_cw_disp = 132;
    cal_cw_img = bustup_img;
    cal_cw_anim_tbl = bustup_img2;
    cal_cw_hdr = bustup_img3;
    cal_cw_sel = 2;
    cal_cw_rate = 1;
    cal_cw_clut = 16;
    cal_cw_anim = arg;
    cal_cw_x = -80;
    cal_cw_y = -64;
}
