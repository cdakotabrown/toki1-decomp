/* @PostProc: scheduler_normalizer */
/* Address: 0x8013AEE4 */
/* Size: 404 bytes (101 instructions) */
#include "types.h"
#include "tokimeki/etc_spr_res.h"

extern void etc_spr0e_res_set(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);
extern void bg_set6(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 stack0, s32 stack1);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s16 a3);

extern s32 etc_rec_month;

extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern s16 cal_cw_y;
extern u8 cw97_disp;
extern u8 cw97_bright;
extern s16 cw97_y;
extern ETC_SPR_RES etc_spr0e_res;

void etc_sprite_init0e(void)
{
    s32 mode;

    etc_spr0e_res_set();
    mode = etc_rec_month;

    if (mode >= 6 && mode < 10) {
        palette_upload_enqueue(etc_spr0e_res.pal_summer, 17, 1, 2, 0);
        bg_set6(etc_spr0e_res.bg_summer[0], etc_spr0e_res.bg_summer[1], etc_spr0e_res.bg_summer[2], etc_spr0e_res.bg_summer[3],
                      etc_spr0e_res.bg_summer[4], etc_spr0e_res.bg_summer[5]);
        date_object_set(etc_spr0e_res.cels, etc_spr0e_res.anim_tbl, etc_spr0e_res.hdr, etc_spr0e_res.scale.w);
    } else {
        palette_upload_enqueue(etc_spr0e_res.pal_other, 17, 1, 2, 0);
        bg_set6(etc_spr0e_res.bg_other[0], etc_spr0e_res.bg_other[1], etc_spr0e_res.bg_other[2], etc_spr0e_res.bg_other[3],
                      etc_spr0e_res.bg_other[4], etc_spr0e_res.bg_other[5]);
        date_object_set(etc_spr0e_res.cels, etc_spr0e_res.anim_tbl, etc_spr0e_res.hdr, etc_spr0e_res.scale.w);
    }

    cal_cw_disp = cal_cw_disp | 0x80;
    cal_cw_bright = 0x80;
    cal_cw_y = -101;
    cw97_disp = cw97_disp | 0x80;
    cw97_bright = 0x80;
    cw97_y = -101;
}
