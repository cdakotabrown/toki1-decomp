/* geko_bufptr_res_set -- GEKO overlay, Address: 0x80140EB4, Size: 140 bytes (35 insns) */
#include "types.h"

extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

extern s32 geko_bg_buf_tbl[];

void geko_bufptr_res_set(void)
{
    palette_upload_enqueue(geko_bg_buf_tbl[0], 17, 1, 2, 0);
    date_sprite_init(geko_bg_buf_tbl[1], geko_bg_buf_tbl[2], geko_bg_buf_tbl[3], geko_bg_buf_tbl[4],
                  geko_bg_buf_tbl[5], geko_bg_buf_tbl[6]);
    date_object_set(0, 0, 0, 0);
    sub_sub_step_up();
}
