/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=25:t7,36:t9 */
/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* Address: 0x80140634, Size: 780 bytes (195 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 pad0[6];
    s16 f24;
    s16 pad1[6];
    s16 f38;
    s16 pad2;
    s16 f42;
    s32 pad3[6];
} Rec68;

typedef struct {
    s16 v[10];
} Tbl20;

extern void geko_scene73_ptr_set(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void sub_sub_step_up(void);

extern u8 screen_w[];
extern s8 date_object_set_tbl0;
extern s8 cal_cw_prio;
extern s8 cal_cw_flag4;
extern s8 cw97_prio;
extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;

extern Rec68 class_w[];

extern Tbl20 geko_sprite_init_tbl0;
extern Tbl20 geko_sprite_init_tbl1;
extern Tbl20 geko_sprite_init_tbl2;

extern s32 geko_scene73_res[];

void geko_sprite_init(void)
{
    Tbl20 t0;
    Tbl20 t1;
    Tbl20 t2;
    s16 i;

    t0 = geko_sprite_init_tbl0;
    t1 = geko_sprite_init_tbl1;
    t2 = geko_sprite_init_tbl2;

    geko_scene73_ptr_set();
    palette_upload_enqueue(geko_scene73_res[4], 17, 1, 2, 0);
    date_sprite_init(geko_scene73_res[5], geko_scene73_res[6], geko_scene73_res[7], geko_scene73_res[8],
                  geko_scene73_res[9], geko_scene73_res[10]);

    for (i = 0; i < 6; i++) {
        screen_w[4319 + i] = 12;
    }

    date_object_set(geko_scene73_res[1], geko_scene73_res[2], geko_scene73_res[0], (*(s16 *)&geko_scene73_res[3]));

    date_object_set_tbl0 = 1;
    (*(s8 *)&((CLASS_W *)class_w)[97].cel_mode) = 8;
    cal_cw_flag4 = 8;
    cw97_prio = 12;
    cal_cw_prio = 12;

    (*(Rec68 *)&((CLASS_W *)class_w)[98]) = (*(Rec68 *)&((CLASS_W *)class_w)[96]);

    (*(s8 *)&((CLASS_W *)class_w)[98].prio) = 11;
    (*(s8 *)&((CLASS_W *)class_w)[98].anim_flag) = 64;
    cw98_frame = 20;
    cw98_x = -6;
    cw98_y = -48;
    ((CLASS_W *)class_w)[98].bright = 128;

    for (i = 0; i < 10; i++) {
        class_w[99 + i] = (*(Rec68 *)&((CLASS_W *)class_w)[98]);
        class_w[99 + i].f24 = t0.v[i];
        class_w[99 + i].f38 = t1.v[i] - 160;
        class_w[99 + i].f42 = t2.v[i] - 120;
    }

    sub_sub_step_up();
}
