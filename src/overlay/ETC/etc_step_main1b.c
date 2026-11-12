/* @PostProc: scheduler_normalizer */
/* Address: 0x80138268 */
/* Size: 244 bytes (61 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s16);
extern void etc_ev1b_res_init(void);
extern void bg_set6(s32, s32, s32, s32, s32, s32);
extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern ETC_EV_RES etc_ev1b_res;

void etc_step_main1b(void)
{
    u8 temp1;
    u8 temp2;

    etc_ev1b_res_init();
    palette_upload_enqueue(etc_ev1b_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev1b_res.bg[0], etc_ev1b_res.bg[1], etc_ev1b_res.bg[2], etc_ev1b_res.bg[3], etc_ev1b_res.bg[4], etc_ev1b_res.bg[5]);
    date_object_set(etc_ev1b_res.cels, etc_ev1b_res.anim_tbl, etc_ev1b_res.hdr, etc_ev1b_res.scale);
    temp1 = cal_cw_disp;
    cal_cw_disp = temp1 | 0x80;
    cal_cw_bright = 0x80;
    temp2 = cw97_disp;
    cal_cw_y = -101;
    cw97_disp = temp2 | 0x80;
    cw97_bright = 0x80;
    cw97_y = -101;
}
