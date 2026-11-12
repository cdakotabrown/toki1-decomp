/* @CompileEnv: IDO52_CONST_INREG_VALUES=112 */
#include "types.h"
#include "tokimeki/sprite.h"

extern u8 bg_change_tbl2;
extern u8 screen_bri;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern s16 cal_cw_anim;
extern u8 cal_cw_clut;
extern u8 fade_bri;
extern s32 ending_scene_ptr_tbl[];
extern s32 tpage_buf[];
extern SPRITE_REC Sprites[];

extern void draw2d3d();
extern void class_switch();
extern void prim_init();
extern void class_clear_all();
extern void set_flag_b3_high_bit();
extern void back_clear_switch();
extern void func_8004863C();
extern void text_sys_init();
extern void shared_system_state_reset();
extern void ending_desc_init();
extern void palette_upload_enqueue();
extern void date_sprite_init();
extern void date_object_set();
extern void class_clear_n();
extern void sub_sub_step_up();

void ending_scene_init(void)
{
    s32  t6;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    back_clear_switch(1);
    func_8004863C();

    tpage_buf[28] = 1;
    tpage_buf[29] = 1;
    tpage_buf[30] = 1;
    text_sys_init();
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    ending_desc_init();

    palette_upload_enqueue(ending_scene_ptr_tbl[5], 17, 1, 2, 0);
    palette_upload_enqueue(ending_scene_ptr_tbl[4], 18, 1, 2, 0);

    date_sprite_init(
        ending_scene_ptr_tbl[6],
        ending_scene_ptr_tbl[7],
        ending_scene_ptr_tbl[8],
        ending_scene_ptr_tbl[9],
        ending_scene_ptr_tbl[10],
        ending_scene_ptr_tbl[11]
    );

    date_object_set(
        ending_scene_ptr_tbl[1],
        ending_scene_ptr_tbl[2],
        ending_scene_ptr_tbl[0],
        (*(s16 *)&ending_scene_ptr_tbl[3])
    );

    (*(u16 *)&Sprites[62].h) = 112;
    (*(u16 *)&Sprites[61].h) = 112;
    (*(u16 *)&Sprites[63].h) = 112;

    class_clear_n(97);

    cal_cw_anim = 0;
    cal_cw_flag4 = 8;
    cal_cw_clut = 18;
    cal_cw_mode = 0;
    cal_cw_disp = 4;

    sub_sub_step_up();
}
