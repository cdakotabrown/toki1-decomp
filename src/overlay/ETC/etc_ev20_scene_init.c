/* @PostProc: scheduler_normalizer */
/* Address: 0x8013872C */
/* Size: 584 bytes (146 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 w[17];
} Blk68;

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void etc_ev20_res_init(void);
extern void bg_set6(s32, s32, s32, s32, s32, s32);

extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern s16 cw98_frame;
extern s16 cw98_y;
extern s32 tpage_buf[];
extern CLASS_W class_w[];
extern ETC_EV_RES etc_ev20_res;

void etc_ev20_scene_init(void)
{
    etc_ev20_res_init();

    tpage_buf[21] = -1;
    tpage_buf[22] = -1;
    tpage_buf[23] = -1;
    tpage_buf[24] = -1;
    tpage_buf[25] = -1;
    tpage_buf[53] = -1;
    tpage_buf[54] = -1;
    tpage_buf[55] = -1;
    tpage_buf[56] = -1;
    tpage_buf[57] = -1;

    palette_upload_enqueue(etc_ev20_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev20_res.bg[0], etc_ev20_res.bg[1], etc_ev20_res.bg[2], etc_ev20_res.bg[3], etc_ev20_res.bg[4],
                  etc_ev20_res.bg[5]);
    date_object_set(etc_ev20_res.cels, etc_ev20_res.anim_tbl, etc_ev20_res.hdr, etc_ev20_res.scale);

    cal_cw_disp = cal_cw_disp | 0x80;
    cal_cw_bright = 128;
    cal_cw_y = -101;
    cal_cw_flag4 = cal_cw_flag4 | 0x08;

    cw97_disp = cw97_disp | 0x80;
    cw97_bright = 128;
    cw97_y = -101;
    class_w[97].cel_mode = class_w[97].cel_mode | 0x08;

    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);

    class_w[98].anim_flag = 64;
    cw98_frame = 40;
    class_w[98].prio = class_w[98].prio - 1;
    class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
    class_w[98].bright = 128;
    cw98_y = -101;
    class_w[98].cel_mode = class_w[98].cel_mode | 0x08;
}
