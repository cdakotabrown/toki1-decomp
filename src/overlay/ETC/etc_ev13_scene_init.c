/* @PostProc: scheduler_normalizer */
/* Address: 0x80137A14 */
/* Size: 372 bytes (93 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 w[17];
} Blk68;

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void etc_ev13_res_set(void);

extern u8 cal_cw_prio;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_prio;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern s16 cw98_frame;
extern s16 cw98_y;
extern CLASS_W class_w[];
extern ETC_EV_RES_OBJ etc_ev13_res;

void etc_ev13_scene_init(void)
{
    etc_ev13_res_set();
    palette_upload_enqueue(etc_ev13_res.pal, 17, 1, 2, 0);
    date_object_set(etc_ev13_res.cels, etc_ev13_res.anim_tbl, etc_ev13_res.hdr, etc_ev13_res.scale);

    class_w[97].cel_mode = 8;
    cal_cw_flag4 = 8;
    cw97_prio = 11;
    cal_cw_prio = 11;

    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);

    class_w[98].anim_flag = 64;
    cw98_frame = 20;
    class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
    class_w[98].bright = 128;
    cw98_y = -101;

    cal_cw_disp = cal_cw_disp | 0x80;
    cal_cw_bright = 128;
    cal_cw_y = -101;

    cw97_disp = cw97_disp | 0x80;
    cw97_bright = 128;
    cw97_y = -101;
}
