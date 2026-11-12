/* geko_bustup_res_set -- GEKO overlay, Address: 0x801424FC, Size: 176 bytes (44 insns) */
#include "types.h"

extern void mes_window_switch(s32 a);
extern void geko_buf_ptr_init(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s16 d);
extern void sub_sub_step_up(void);

extern s32 geko_bustup_res[];

void geko_bustup_res_set(void)
{
    mes_window_switch(0);
    geko_buf_ptr_init();
    palette_upload_enqueue(geko_bustup_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_bustup_res[5], geko_bustup_res[6], geko_bustup_res[7], geko_bustup_res[8],
                  geko_bustup_res[9], geko_bustup_res[10]);
    date_object_set(geko_bustup_res[1], geko_bustup_res[2], geko_bustup_res[0], (*(s16 *)&geko_bustup_res[3]));
    sub_sub_step_up();
}
