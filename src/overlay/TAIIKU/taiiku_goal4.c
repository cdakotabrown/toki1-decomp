/* @SymbolStart: 0x80134F00 */
/* @SymbolSize: 516 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 */
#include "types.h"

extern s16 taiiku_kari_scrl_hi;

extern s32 sub_step_up(void);
extern s32 sub_sub_step_up(void);
extern s32 device_cmd_send(s32);
extern s32 game_reset(void);
extern s32 class_clear_n(s32);
extern s32 Fade_Out_Init(void);
extern s32 bri_fade_out(void);
extern s32 scroll_poly_disp(s32, s32, s32, s32, s32);
extern void taiiku_seat_set(s32, s32);
extern void taiiku_seat_plate_set(void);
extern s32 taiiku_runner_count_inc(void);
extern void taiiku_run(void);
extern s32 taiiku_scroll_disp(void);
extern void taiiku_racers_step(void);
extern s32 taiiku_racer_pos_set(void);
extern s32 taiiku_goal_check2(void);
extern void taiiku_sprite_init(void);
extern u8 now_sub_sub_step;

void taiiku_goal4(void) {
    s32 v1 = now_sub_sub_step;

    s32 i;

    switch (v1) {
    case 0:
        taiiku_runner_count_inc();
        taiiku_run();
        taiiku_seat_plate_set();
        taiiku_racers_step();
        taiiku_racer_pos_set();
        taiiku_scroll_disp();
        taiiku_seat_set((s32)taiiku_kari_scrl_hi, 3);
        taiiku_goal_check2();
        if (taiiku_kari_scrl_hi >= 1025) {
            device_cmd_send(5888);
            device_cmd_send(1541);
            sub_sub_step_up();
        }
        break;
    case 1:
        taiiku_runner_count_inc();
        taiiku_run();
        taiiku_racers_step();
        taiiku_racer_pos_set();
        taiiku_seat_set((s32)taiiku_kari_scrl_hi, 3);
        taiiku_scroll_disp();
        if (taiiku_goal_check2() != 0) {
            device_cmd_send(1540);
            Fade_Out_Init();
            sub_sub_step_up();
        }
        break;
    case 2:
        scroll_poly_disp(((s32)taiiku_kari_scrl_hi >> 1) % 640, 15, 0, 112, 640);
        scroll_poly_disp((s32)taiiku_kari_scrl_hi & 0x7F, 15, 112, 128, 128);
        taiiku_seat_set((s32)taiiku_kari_scrl_hi, 3);
        if (bri_fade_out() != 0) {
            device_cmd_send(194);
            for (i = 0; i < 23; i++) {
                class_clear_n(i);
            }
            for (i = 0; i < 10; i++) {
                class_clear_n(i + 76);
            }
            taiiku_sprite_init();
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
