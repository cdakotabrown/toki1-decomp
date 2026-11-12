/* @SymbolStart: 0x80150454 */
/* @SymbolSize: 340 */
/* Function: date_girl2_mode3_scene_init (true entry 0x80150454) */
/* Overlay: DATE */
/* Size: 340 bytes (85 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s32 w[17]; } CopyBlock;

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern volatile s16 girl2_tokimeki;
extern volatile s16 girl2_syoushin;

extern s32 date_scene_init2_res[];

extern u8 date_object_set_tbl0;
extern CLASS_W class_w[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

extern s32 param_clamp();
extern s32 date_girl2_mode3_res_ptr_init();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 sub_sub_step_up();

void date_girl2_mode3_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[16];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[16];
    date_sss_soundev = date_soundev_tbl[16];
    girl2_tokimeki = girl2_tokimeki + 2;
    girl2_syoushin = girl2_syoushin - 10;
    param_clamp();
    date_girl2_mode3_res_ptr_init();
    palette_upload_enqueue(date_scene_init2_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_init2_res[5], date_scene_init2_res[6], date_scene_init2_res[7], date_scene_init2_res[8],
                  date_scene_init2_res[9], date_scene_init2_res[10]);
    date_object_set(date_scene_init2_res[1], date_scene_init2_res[2], date_scene_init2_res[0], (s32)(*(s16 *)&date_scene_init2_res[3]));

    ((CopyBlock *)&class_w[96].active)[2] = ((CopyBlock *)&class_w[96].active)[0];

    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
