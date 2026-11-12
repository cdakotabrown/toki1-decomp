/* @SymbolStart: 0x80151D68 */
/* @SymbolSize: 220 */
/* Address: 0x80151D8C (true entry 0x80151D68) */
/* Size: 184 bytes (46 insns) + 36-byte cross-prologue head */
#include "types.h"

extern s32 sub_sub_step_up();
extern void date_ptrtbl8_init(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 date_scene_init8_res[];
extern u8 date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl3_mode5_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[28];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[28];
    date_sss_soundev = date_soundev_tbl[28];
    date_ptrtbl8_init();
    palette_upload_enqueue(date_scene_init8_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_init8_res[5], date_scene_init8_res[6], date_scene_init8_res[7], date_scene_init8_res[8],
                  date_scene_init8_res[9], date_scene_init8_res[10]);
    date_object_set(date_scene_init8_res[1], date_scene_init8_res[2], date_scene_init8_res[0], (*(s16 *)&date_scene_init8_res[3]));
    date_object_set_tbl0 = 1;
    return sub_sub_step_up();
}
