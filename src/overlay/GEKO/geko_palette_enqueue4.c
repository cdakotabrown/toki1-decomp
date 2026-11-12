/* geko_palette_enqueue4 -- GEKO overlay, Address: 0x8013CD98, Size: 168 bytes (42 insns), Semi-matched */
#include "types.h"

extern void geko_enq4_res_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp16);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp16, s32 sp20);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s16 a3);
extern void sub_sub_step_up(void);

extern s32 geko_enq4_res[];

void geko_palette_enqueue4(void)
{
    geko_enq4_res_init();
    palette_upload_enqueue(geko_enq4_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_enq4_res[5], geko_enq4_res[6], geko_enq4_res[7], geko_enq4_res[8], geko_enq4_res[9], geko_enq4_res[10]);
    date_object_set(geko_enq4_res[1], geko_enq4_res[2], geko_enq4_res[0], (*(s16 *)&geko_enq4_res[3]));
    sub_sub_step_up();
}
