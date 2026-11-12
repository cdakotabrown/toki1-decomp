/* geko_loadbuf2_res_set -- GEKO overlay, Address: 0x8013F1B8, Size: 176 bytes (44 insns) */
#include "types.h"

extern void geko_loadbuf2_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s16 d);
extern void sub_sub_step_up(void);

extern s8 date_object_set_tbl0;
extern s32 geko_loadbuf2_res[];

void geko_loadbuf2_res_set(void)
{
    geko_loadbuf2_init();
    palette_upload_enqueue(geko_loadbuf2_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_loadbuf2_res[5], geko_loadbuf2_res[6], geko_loadbuf2_res[7], geko_loadbuf2_res[8],
                  geko_loadbuf2_res[9], geko_loadbuf2_res[10]);
    date_object_set(geko_loadbuf2_res[1], geko_loadbuf2_res[2], geko_loadbuf2_res[0], (*(s16 *)&geko_loadbuf2_res[3]));
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
