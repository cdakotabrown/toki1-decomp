/* geko_event8_res_set -- GEKO overlay, Address: 0x8013975C, Size: 188 bytes (47 insns) */
#include "types.h"

extern void geko_res_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

extern s16 cal_cw_anim;
extern s32 bustup_mode;
extern s32 geko_event8_res[];

void geko_event8_res_set(void)
{
    geko_res_init();
    palette_upload_enqueue(geko_event8_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_event8_res[5], geko_event8_res[6], geko_event8_res[7], geko_event8_res[8],
                  geko_event8_res[9], geko_event8_res[10]);
    date_object_set(geko_event8_res[1], geko_event8_res[2], geko_event8_res[0], (*(s16 *)&geko_event8_res[3]));
    cal_cw_anim = 4;
    bustup_mode = 1;
    sub_sub_step_up();
}
