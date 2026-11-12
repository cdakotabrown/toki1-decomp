/* @PostProc: scheduler_normalizer */
/* Address: 0x80138B5C */
/* Size: 284 bytes (71 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
#include "tokimeki/class_w.h"

extern void etc_ev24_res_set(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);
extern void bg_set6(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 stack0, s32 stack1);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s16 a3);


extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern CLASS_W class_w[];
extern ETC_EV_RES etc_ev24_res;

void etc_ev24_scene_init(void)
{
    etc_ev24_res_set();
    palette_upload_enqueue(etc_ev24_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev24_res.bg[0], etc_ev24_res.bg[1], etc_ev24_res.bg[2], etc_ev24_res.bg[3],
                  etc_ev24_res.bg[4], etc_ev24_res.bg[5]);
    date_object_set(etc_ev24_res.cels, etc_ev24_res.anim_tbl, etc_ev24_res.hdr, etc_ev24_res.scale);

    cal_cw_disp = cal_cw_disp | 0x80;
    cal_cw_bright = 0x80;
    cal_cw_y = -101;
    cal_cw_flag4 = cal_cw_flag4 | 0x08;
    cw97_disp = cw97_disp | 0x80;
    cw97_bright = 0x80;
    cw97_y = -101;
    class_w[97].cel_mode = class_w[97].cel_mode | 0x08;
}
