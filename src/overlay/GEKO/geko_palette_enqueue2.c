/* geko_palette_enqueue2 -- GEKO overlay, Address: 0x8013DDAC, Size: 176 bytes (44 insns) */
#include "types.h"

extern void geko_enq2_res_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s16 d);
extern void sub_sub_step_up(void);

extern s8 date_object_set_tbl0;
extern s32 geko_enq2_res[];

void geko_palette_enqueue2(void)
{
    geko_enq2_res_init();
    palette_upload_enqueue(geko_enq2_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_enq2_res[5], geko_enq2_res[6], geko_enq2_res[7], geko_enq2_res[8],
                  geko_enq2_res[9], geko_enq2_res[10]);
    date_object_set(geko_enq2_res[1], geko_enq2_res[2], geko_enq2_res[0], (*(s16 *)&geko_enq2_res[3]));
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
