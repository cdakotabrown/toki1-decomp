/* geko_scene77_res_set -- GEKO overlay, Address: 0x8014147C, Size: 196 bytes (49 insns) */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void geko_buf_init();
extern void palette_upload_enqueue();
extern void date_sprite_init();
extern void date_object_set();
extern void sub_sub_step_up();

extern s8  now_event_no;
extern s8  cal_cw_flag4;
extern s32 geko_scene77_res[];
extern CLASS_W class_w[];

void geko_scene77_res_set(void)
{
    geko_buf_init();
    palette_upload_enqueue(geko_scene77_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_scene77_res[5], geko_scene77_res[6], geko_scene77_res[7], geko_scene77_res[8],
                  geko_scene77_res[9], geko_scene77_res[10]);
    date_object_set(geko_scene77_res[1], geko_scene77_res[2], geko_scene77_res[0], (*(s16 *)&geko_scene77_res[3]));
    cal_cw_flag4 = 8;
    now_event_no = 77;
    (*(s8 *)&class_w[97].cel_mode) = 8;
    sub_sub_step_up();
}
