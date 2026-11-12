/* @CompileEnv: IDO52_UGEN_TEMP_REG=11:t7,12:t8,13:t9,14:t0,15:t1,16:t2 */
/* @SymbolSize: 360 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* Function: date_girl3_mode2_scene_init */
/* Overlay: DATE */
/* Address: 0x801518B8 */
/* Size: 360 bytes (90 instructions) */
#include "types.h"

typedef struct { s32 w[17]; } CopyBlock;

extern s32 *date_ev16_res;
extern s32 date_ev16_res_cels;
extern s32 date_ev16_res_anim_tbl;
extern s16 date_ev16_res_scale;
extern s32 date_ev16_res_pal;
extern s32 date_ev16_res_bg0;
extern s32 date_ev16_res_bg1;
extern s32 date_ev16_res_bg2;
extern s32 date_ev16_res_bg3;
extern s32 date_ev16_res_bg4;
extern s32 date_ev16_res_bg5;

extern CopyBlock cw96_active;
extern CopyBlock cw97_active;
extern s16 cal_cw_frame;
extern s16 cw97_anim;
extern s16 cw97_frame;
extern s32 wink2_stop;
extern s32 bustup_mode;

extern s32 load_tpage_buf_lock();
extern s32 date_ptrtbl5_init();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 sub_sub_step_up();

void date_girl3_mode2_scene_init(void)
{
    s32 s0;

    s0 = 0;
    do {
        load_tpage_buf_lock(s0 + 5, date_ev16_res[s0], 1);
        s0 += 1;
        s0 = (s32)(s16)s0;
    } while (s0 < 6);

    date_ptrtbl5_init();
    palette_upload_enqueue(date_ev16_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev16_res_bg0, date_ev16_res_bg1, date_ev16_res_bg2, date_ev16_res_bg3,
                  date_ev16_res_bg4, date_ev16_res_bg5);
    date_object_set(date_ev16_res_cels, date_ev16_res_anim_tbl, (s32)date_ev16_res, (s32)date_ev16_res_scale);

    cw97_active = cw96_active;

    cw97_anim = 3;
    bustup_mode = 2;
    wink2_stop = 1;
    cal_cw_frame = 3;
    cw97_frame = 3;
    sub_sub_step_up();
}
