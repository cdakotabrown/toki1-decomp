/* @RodataBase: 0x80148D7C */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x14c:rt:a1:v0 0x154:rs:a1:v1 0x154:rt:a0:v1 0x158:rs:v1:v0 0x160:rt:a0:v1 0x1d4:rt:a0:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void load_done_flag_set(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void taiiku_course_disp(void);
extern void taiiku_run_speed(void);
extern void taiiku_camera_mtx_set(void);
extern void Vblnk_Timer_Init(void);
extern void taiiku_timer(void);
extern void taiiku_run2(void);
extern void taiiku_zone_main(void);
extern void taiiku_cw97_main(void);
extern void taiiku_runners_pos_set(void);
extern void taiiku_slot(void);
extern void game_reset(void);

extern s32 load_done_flag;
extern s32 taiiku_tama_fall;
extern s16 cw97_y;
extern s16 cal_cw_y;
extern u8  cal_cw_prio;
extern u8  cw97_prio;
extern s16 cal_cw_x;
extern s16 cw98_x;
extern s16 cw97_x;
extern s16 cw98_y;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern u8 now_sub_sub_step;
extern CLASS_W class_w[];

void taiiku_play3(void)
{
    s32 sel = now_sub_sub_step;

    s32 a, b;

    switch (sel) {
    case 0:
        load_done_flag_set();
        if (load_done_flag != 0) {
            device_cmd_send(46);
            taiiku_tama_fall = 0;
            sub_sub_step_up();
        }
        taiiku_course_disp();
        taiiku_run_speed();
        taiiku_camera_mtx_set();
        break;
    case 1:
        device_cmd_send(36);
        taiiku_course_disp();
        taiiku_run_speed();
        taiiku_camera_mtx_set();
        sub_sub_step_up();
        break;
    case 2:
        device_cmd_send(35);
        taiiku_course_disp();
        taiiku_run_speed();
        taiiku_camera_mtx_set();
        sub_sub_step_up();
        break;
    case 3:
        device_cmd_send(1282);
        taiiku_course_disp();
        taiiku_run_speed();
        taiiku_camera_mtx_set();
        Vblnk_Timer_Init();
        sub_sub_step_up();
        break;
    case 4:
        taiiku_timer();
        taiiku_run2();
        taiiku_zone_main();
        taiiku_cw97_main();
        a = cal_cw_y;
        b = cw97_y;
        if (a >= b) {
            cal_cw_prio = 2;
            cw97_prio = 3;
        } else {
            cal_cw_prio = 3;
            cw97_prio = 2;
        }
        cw98_x = cal_cw_x;
        class_w[99].x.half[1] = cw97_x;
        cw98_y = a;
        class_w[99].y.half[1] = b;
        class_w[103].x.half[1] = taiiku_tama_sx >> 3;
        class_w[103].y.half[1] = (taiiku_tama_sy >> 3) + 32;
        taiiku_course_disp();
        taiiku_run_speed();
        taiiku_runners_pos_set();
        taiiku_slot();
        break;
    default:
        game_reset();
        break;
    }
}
