/* @SymbolSize: 432 */
/* kangei_cal_init - KANGEI overlay, Address: 0x80135CF4, Size: 432 bytes (108 insns). */
#include "types.h"

extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern s32 cal_cw_attr;
extern u8 cal_cw_disp;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8 cal_cw_rate;
extern u8 cal_cw_clut;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern u8 cal_cw_bright;

extern s32 kangei_cal_desc_tbl[][4];
extern u8 now_girl;

void kangei_cal_init(void)
{
    s32 mode = now_girl;

    if (mode == 10) {
        class_clear_n(96);
        cal_cw_prio = 11;
        cal_cw_mode = 0;
        cal_cw_attr = 0x41000000;
        cal_cw_disp = 4;
        cal_cw_img = kangei_cal_desc_tbl[0][1];
        cal_cw_anim_tbl = kangei_cal_desc_tbl[0][2];
        cal_cw_hdr = kangei_cal_desc_tbl[0][0];
        cal_cw_aux14 = (*(s16 *)&kangei_cal_desc_tbl[0][3]);
        cal_cw_anim = 0;
        cal_cw_frame = 0;
        cal_cw_rate = 2;
        cal_cw_clut = 16;
        cal_cw_x = 0;
        cal_cw_y = -32;
        cal_cw_bright = 0;
    } else if (mode == 3) {
        class_clear_n(96);
        cal_cw_prio = 11;
        cal_cw_mode = 0;
        cal_cw_attr = 0x41000000;
        cal_cw_disp = 4;
        cal_cw_img = kangei_cal_desc_tbl[1][1];
        cal_cw_anim_tbl = kangei_cal_desc_tbl[1][2];
        cal_cw_hdr = kangei_cal_desc_tbl[1][0];
        cal_cw_aux14 = (*(s16 *)&kangei_cal_desc_tbl[1][3]);
        cal_cw_anim = 0;
        cal_cw_frame = 0;
        cal_cw_rate = 2;
        cal_cw_clut = 16;
        cal_cw_x = 0;
        cal_cw_y = -32;
        cal_cw_bright = 0;
    }
    sub_sub_step_up();
}
