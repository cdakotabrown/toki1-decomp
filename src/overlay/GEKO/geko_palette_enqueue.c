/* geko_palette_enqueue -- GEKO overlay, Address: 0x8013D81C, Size: 168 bytes (42 insns) */
#include "types.h"

extern void geko_bufptr_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s32 geko_enq_res[];

void geko_palette_enqueue(void)
{
    geko_bufptr_init();
    palette_upload_enqueue(geko_enq_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_enq_res[5], geko_enq_res[6], geko_enq_res[7], geko_enq_res[8], geko_enq_res[9], geko_enq_res[10]);
    date_object_set(geko_enq_res[1], geko_enq_res[2], geko_enq_res[0], (*(s16 *)&geko_enq_res[3]));
    sub_sub_step_up();
}
