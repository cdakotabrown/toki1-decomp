/* Address: 0x801533C8 (true symbol start 0x80153320, cross-prologue) */
/* Size: 348 bytes (87 insns). */
/* @SymbolStart: 0x80153320 */
/* @SymbolSize: 348 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_bufptr_init();

extern s32 date_ss_serifuev, date_ss_serifuev_kind, date_sss_soundev;
extern s16 kao2[];
extern s16 kao[];
extern s16 ans[];
extern s16 imp[];
extern s32 date_ev2f_res_pal;
extern s32 date_ev2f_res_bg0, date_ev2f_res_bg1, date_ev2f_res_bg2, date_ev2f_res_bg3, date_ev2f_res_bg4, date_ev2f_res_bg5;
extern s32 date_ev2f_res, date_ev2f_res_cels, date_ev2f_res_anim_tbl;
extern s16 date_ev2f_res_scale;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl7_mode2_scene_init(void) {
    date_ss_serifuev = date_serifuev_tbl[31];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[31];
    date_sss_soundev = date_soundev_tbl[31];
    kao[0] = 1;
    kao[2] = 2;
    kao[1] = 4;
    kao2[0] = 1;
    kao2[2] = 2;
    kao2[1] = 3;
    imp[0] = 0;
    imp[2] = 2;
    imp[1] = 4;
    ans[0] = 0;
    ans[2] = 1;
    ans[1] = 2;
    date_bufptr_init();
    palette_upload_enqueue(date_ev2f_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev2f_res_bg0, date_ev2f_res_bg1, date_ev2f_res_bg2, date_ev2f_res_bg3,
                  date_ev2f_res_bg4, date_ev2f_res_bg5);
    date_object_set(date_ev2f_res_cels, date_ev2f_res_anim_tbl, date_ev2f_res, (s16)date_ev2f_res_scale);
    sub_sub_step_up();
}
