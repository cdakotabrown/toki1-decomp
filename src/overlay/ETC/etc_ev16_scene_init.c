/* @PostProc: scheduler_normalizer --block-aware */
/* Function: etc_ev16_scene_init */
/* Overlay: ETC */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
#include "tokimeki/class_w.h"

extern void palette_upload_enqueue();
extern void date_object_set();
extern void etc_ev16_res_set();
extern void bg_set6();

typedef struct {
    s32 w[17];
} Blk68;


extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern s16 cw97_anim;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;

extern CLASS_W class_w[];
extern ETC_EV_RES etc_ev16_res;

void etc_ev16_scene_init(void)
{
    etc_ev16_res_set();
    palette_upload_enqueue(etc_ev16_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev16_res.bg[0], etc_ev16_res.bg[1], etc_ev16_res.bg[2], etc_ev16_res.bg[3], etc_ev16_res.bg[4],
                  etc_ev16_res.bg[5]);
    date_object_set(etc_ev16_res.cels, etc_ev16_res.anim_tbl, etc_ev16_res.hdr, etc_ev16_res.scale);
    (*(Blk68 *)&class_w[97]) = (*(Blk68 *)&class_w[96]);
    cw97_anim = 3;
    cal_cw_disp |= 0x80;
    cal_cw_bright = 128;
    cal_cw_y = -101;
    cw97_disp |= 0x80;
    cw97_bright = 128;
    cw97_y = -101;
}
