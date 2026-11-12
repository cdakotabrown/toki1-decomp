/* name_ent_cal_cw_set - NAME_ENT overlay, Address: 0x8014687C, Size: 144 bytes (36 insns) */
#include "types.h"

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;

void name_ent_cal_cw_set(void)
{
    cal_cw_prio = 10;
    cal_cw_mode = 64;
    cal_cw_attr = 0x41000000;
    cal_cw_disp = 164;
    cal_cw_img = 0x801842C4;
    cal_cw_anim_tbl = 0x801842F0;
    cal_cw_hdr = 0x801842BC;
    cal_cw_frame = 0;
    cal_cw_sel = 128;
    cal_cw_x = -160;
    cal_cw_y = -120;
}
