/* @SymbolSize: 364 */
/* @PostProc: scheduler_normalizer */
/* Address: 0x8013B078 */
/* Size: 364 bytes (91 instructions) */
#include "types.h"
#include "tokimeki/etc_spr_res.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void etc_spr46_res_set(void);
extern void bg_set6(s32, s32, s32, s32, s32, s32);

extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern s32 etc_rec_month;
extern ETC_SPR_RES etc_spr46_res;

void etc_sprite_init46(void)
{
    s32 v0;

    etc_spr46_res_set();
    v0 = etc_rec_month;
    if (v0 >= 6 && v0 < 10) {
        palette_upload_enqueue(etc_spr46_res.pal_summer, 17, 1, 2, 0);
        bg_set6(etc_spr46_res.bg_summer[0], etc_spr46_res.bg_summer[1], etc_spr46_res.bg_summer[2], etc_spr46_res.bg_summer[3],
                      etc_spr46_res.bg_summer[4], etc_spr46_res.bg_summer[5]);
    } else {
        palette_upload_enqueue(etc_spr46_res.pal_other, 17, 1, 2, 0);
        bg_set6(etc_spr46_res.bg_other[0], etc_spr46_res.bg_other[1], etc_spr46_res.bg_other[2], etc_spr46_res.bg_other[3],
                      etc_spr46_res.bg_other[4], etc_spr46_res.bg_other[5]);
    }
    date_object_set(etc_spr46_res.cels, etc_spr46_res.anim_tbl, etc_spr46_res.hdr, etc_spr46_res.scale.h);

    cal_cw_disp = cal_cw_disp | 0x80;
    cal_cw_bright = 128;
    cal_cw_y = -101;

    cw97_disp = cw97_disp | 0x80;
    cw97_bright = 128;
    cw97_y = -101;
}
