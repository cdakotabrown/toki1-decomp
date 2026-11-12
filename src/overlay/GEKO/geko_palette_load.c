/* geko_palette_load -- GEKO overlay, Address: 0x8013C774, Size: 176 bytes (44 insns) */
#include "types.h"

extern void geko_palette_load_res_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s16 d);
extern void sub_sub_step_up(void);

extern s8 date_object_set_tbl0;
extern s32 geko_palette_load_res[];

void geko_palette_load(void)
{
    geko_palette_load_res_init();
    palette_upload_enqueue(geko_palette_load_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_palette_load_res[5], geko_palette_load_res[6], geko_palette_load_res[7], geko_palette_load_res[8],
                  geko_palette_load_res[9], geko_palette_load_res[10]);
    date_object_set(geko_palette_load_res[1], geko_palette_load_res[2], geko_palette_load_res[0], (*(s16 *)&geko_palette_load_res[3]));
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
