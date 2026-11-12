/* Address: 0x8013761C (absorbed cross-prologue head) .. 0x8013789C */
/* Size: 640 bytes (160 instructions) */
/* @SymbolStart: 0x8013761C */
/* @SymbolSize: 640 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t6:t5 0x174:rs:t6:t5 0x174:rt:t6:t5 0x17c:rs:t6:t5 0xfc:rs:t0:s0 0xfc:rt:s0:t0 0x120:rs:t1:s0 0x120:rt:s0:t1 0x150:rt:t4:v1 0x154:rs:t4:v1 0x154:rt:t4:v1 0x168:rs:t4:v1 0x168:rt:t5:t4 0x170:rt:t5:t4 0x18c:rt:t8:t7 0x190:rs:t8:t7 0x190:rt:t8:t7 0x194:rt:t7:t6 0x1a0:rt:t7:t6 0x1a4:rt:t9:t8 0x1a8:rs:t8:t7 0x1ac:rs:t9:t8 0x1ac:rt:t9:t8 0x1b4:rs:t9:t8 0x1c0:rt:t0:t9 0x1c4:rs:t0:t9 0x1cc:rs:t0:t9 0x1d0:rt:t3:t2 0x1d8:rt:t4:t3 0x1dc:rt:t1:t0 0x1e0:rs:t1:t0 0x1e0:rt:t1:t0 0x1e8:rs:t1:t0 0x1ec:rs:t3:t2 0x1ec:rt:t3:t2 0x1f0:rt:t2:t1 0x1f4:rs:t3:t2 0x1f4:rt:t2:t1 0x1f8:rt:t4:t3 0x1fc:rt:t5:t4 0x200:rt:t5:t4 0x204:rt:t4:t3 0x220:rt:t6:t5 0x224:rt:t7:t6 0x228:rt:t7:t6 0x22c:rt:t6:t5 0x24c:rt:t8:t7 0x250:rt:t9:t8 0x254:rt:t9:t8 0x258:rt:t8:t7 */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 frame_counter;
extern u8 step_mode;
extern u8 mes_win_cw_disp;

extern u8 *taco_actor_rec;
extern s32 taco_view_flag;
extern u8 taco_load_no;
extern u8 taco_round_no;
extern s16 taco_time_left;
extern s32 taco_score;
extern s32 taco_drop_bright;

extern s32  sub_sub_step_up(void);
extern void back_clear_switch(s32);
extern void image_upload_queue_add_flags(s32, s32, s32, s32, s32, s32, s32);
extern void k_reset(s32);
extern void disp_mode_set2(s32);
extern void all_object_disp_off(void);
extern void GsInitCoordinate2(s32, u8 *);
extern void taco_actor_reset(void);
extern void taco_actor_init(void);
extern s32  taco_sprite_init(void);
extern void taco_obj_clear(void);
extern void taco_work_clear(void);
extern void taco_drop_init(void);
extern void taco_model_link(s32, s32, u8 *);
extern void taco_light_init(void);
extern void taco_camera_init(void);
extern void taco_objects_init(void);
extern void taco_actor_pool_clear(void);

s32 taco_phase_init(void)
{
    s32 r;
    s32 sel;
    s32 i;

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
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
        disp_mode_set2(1);
        k_reset(0);
        back_clear_switch(1);
        taco_camera_init();
        taco_light_init();
        all_object_disp_off();
        taco_obj_clear();
        taco_objects_init();
        for (i = 0; i != 10744; i += 136) {
            GsInitCoordinate2(0, taco_actor_rec + i + 20);
            taco_model_link(0x801AC000, 0, taco_actor_rec + i);
        }
        taco_actor_init();
        taco_actor_reset();
        r = taco_sprite_init();
        mes_win_cw_disp = mes_win_cw_disp & 0x7F;
        step_mode = step_mode + 1;
        break;
    case 1:
        if (taco_load_no != 0) {
            break;
        }
        taco_actor_pool_clear();
        taco_view_flag = 128;
        taco_actor_rec[133] = 255;
        taco_actor_rec[1221] = 255;
        switch (frame_counter & 3) {
        case 0:
            *(s16 *)(taco_actor_rec + 114) = 0;
            break;
        case 1:
            *(u16 *)(taco_actor_rec + 114) = 1;
            break;
        }
        image_upload_queue_add_flags(0x80162000, 14, 1, 0, 0, 256, 256);
        image_upload_queue_add_flags(0x80172000, 12, 1, 0, 0, 256, 256);
        image_upload_queue_add_flags(0x80182000, 10, 1, 0, 0, 128, 128);
        r = sub_sub_step_up();
        break;
    }
    return r;
}
