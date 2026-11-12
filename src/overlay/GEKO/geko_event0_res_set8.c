/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* geko_event0_res_set8 -- GEKO overlay, Address: 0x8013ACB8, Size: 272 bytes (68 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void geko_event0_ptr_set8(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

typedef struct {
    s32 w[17];
} Work68;

extern s8 cal_cw_flag4;
extern s16 cw98_anim;

extern s32 geko_event0_res[];
extern CLASS_W class_w[];

void geko_event0_res_set8(void)
{
    geko_event0_ptr_set8();
    palette_upload_enqueue(geko_event0_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_event0_res[5], geko_event0_res[6], geko_event0_res[7], geko_event0_res[8],
                  geko_event0_res[9], geko_event0_res[10]);
    date_object_set(geko_event0_res[1], geko_event0_res[2], geko_event0_res[0], (*(s16 *)&geko_event0_res[3]));
    (*(s8 *)&class_w[97].cel_mode) = 8;
    cal_cw_flag4 = 8;
    (*(Work68 *)&class_w[98]) = (*(Work68 *)&class_w[96]);
    cw98_anim = 4;
    sub_sub_step_up();
}
