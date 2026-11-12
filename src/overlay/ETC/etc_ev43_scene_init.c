/* @PostProc: scheduler_normalizer */
/* Address: 0x8013A700, Size: 244 bytes (61 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"

extern void etc_ev43_res_init();
extern void palette_upload_enqueue();
extern void bg_set6();
extern void date_object_set();

extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern ETC_EV_RES etc_ev43_res;


void etc_ev43_scene_init(void)
{
    etc_ev43_res_init();
    palette_upload_enqueue(etc_ev43_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev43_res.bg[0], etc_ev43_res.bg[1], etc_ev43_res.bg[2], etc_ev43_res.bg[3], etc_ev43_res.bg[4], etc_ev43_res.bg[5]);
    date_object_set(etc_ev43_res.cels, etc_ev43_res.anim_tbl, etc_ev43_res.hdr, etc_ev43_res.scale);
    cal_cw_disp |= 0x80;
    cal_cw_bright = 128;
    cal_cw_y = -101;
    cw97_disp |= 0x80;
    cw97_bright = 128;
    cw97_y = -101;
}
