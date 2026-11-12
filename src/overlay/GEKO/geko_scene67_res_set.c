/* geko_scene67_res_set -- GEKO overlay, Address: 0x80140260, Size: 168 bytes (42 insns) */
#include "types.h"

extern void geko_scene67_ptr_set(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s32 geko_scene67_res[];

void geko_scene67_res_set(void)
{
    geko_scene67_ptr_set();
    palette_upload_enqueue(geko_scene67_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_scene67_res[5], geko_scene67_res[6], geko_scene67_res[7], geko_scene67_res[8], geko_scene67_res[9], geko_scene67_res[10]);
    date_object_set(geko_scene67_res[1], geko_scene67_res[2], geko_scene67_res[0], (*(s16 *)&geko_scene67_res[3]));
    sub_sub_step_up();
}
