/* Address: 0x80137FFC */
#include "types.h"

extern s32 taco_drop_bright;
extern u8 taco_round_no;
extern s32 taco_score;
extern s32 taco_view_flag;
extern s16 taco_time_left;
extern u8 taco_load_no;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern volatile u8 *taco_actor_rec;

extern void taco_drop_init();
extern void taco_work_clear();
extern void sd_cmd_limit_send();
extern void disp_mode_set2();
extern void k_reset();
extern void back_clear_switch();
extern void taco_camera_init();
extern void taco_light_init();
extern void all_object_disp_off();
extern void taco_obj_clear();
extern void taco_objects_init();
extern void GsInitCoordinate2();
extern void taco_model_link();
extern void taco_actor_init();
extern void taco_actor_reset();
extern void taco_sprite_init();
extern void sub_sub_step_up();

void
taco_scene_init(void)
{
    s32 off;

    taco_drop_init();
    taco_drop_bright = 128;
    taco_round_no = 0;
    taco_score = 0;
    taco_view_flag = 128;
    taco_time_left = 18000;
    taco_work_clear();
    taco_load_no = 16;
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    sd_cmd_limit_send(0, 1283);
    disp_mode_set2(1);
    k_reset(0);
    back_clear_switch(1);
    taco_camera_init();
    taco_light_init();
    all_object_disp_off();
    taco_obj_clear();
    taco_objects_init();

    for (off = 0; off != 10744; off += 136) {
        GsInitCoordinate2(0, off + taco_actor_rec + 20);
        taco_model_link((void *)0x801AC000, 0, off + taco_actor_rec);
    }

    taco_actor_init();
    taco_actor_reset();
    taco_sprite_init();
    sub_sub_step_up();
}
