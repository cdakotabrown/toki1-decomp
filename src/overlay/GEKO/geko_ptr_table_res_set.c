/* geko_ptr_table_res_set -- GEKO overlay, Address: 0x8013B5AC, Size: 372 bytes (93 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void geko_ptr_table_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);

extern s32 geko_fullscr_bg_res[];

extern s16 cw99_w14;
extern s32 tpage_buf[];
extern CLASS_W class_w[];

void geko_ptr_table_res_set(void)
{
    geko_ptr_table_init();
    palette_upload_enqueue(geko_fullscr_bg_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_fullscr_bg_res[5], geko_fullscr_bg_res[6], geko_fullscr_bg_res[7], geko_fullscr_bg_res[8],
                  geko_fullscr_bg_res[9], geko_fullscr_bg_res[10]);
    date_object_set(0, 0, 0, 0);
    class_clear_n(99);

    (*(s8 *)&class_w[99].prio) = 10;
    (*(s8 *)&class_w[99].anim_flag) = 0;
    (*(s32 *)&class_w[99].attribute) = 0x41000000;
    (*(s8 *)&class_w[99].disp_flag) = 4;
    (*(s32 *)&class_w[99].cels) = geko_fullscr_bg_res[1];
    (*(s32 *)&class_w[99].anim_tbl) = geko_fullscr_bg_res[2];
    (*(s32 *)&class_w[99].hdr) = geko_fullscr_bg_res[0];
    cw99_w14 = (s16)(*(s16 *)&geko_fullscr_bg_res[3]);
    class_w[99].anim = 4;
    class_w[99].frame = 0;
    class_w[99].tick = 0;
    (*(s8 *)&class_w[99].rate) = 1;
    (*(s8 *)&class_w[99].clut) = 17;
    class_w[99].x.half[1] = -160;
    class_w[99].y.half[1] = -120;
    (*(s8 *)&class_w[99].bright) = 0;
    (*(s8 *)&class_w[99].tpage_sel) = 0;
    (*(s8 *)&class_w[99].cel_mode) = 0;
    tpage_buf[26] = -1;
    sub_sub_step_up();
}
