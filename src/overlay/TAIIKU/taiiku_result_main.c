/* @SymbolStart: 0x80145C3C */
/* @SymbolSize: 432 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern s16 taiiku_spoon_scrl_hi;

void sub_step_up();
void sub_sub_step_up();
void device_cmd_send();
void game_reset();
void Vblnk_Timer_Init();
void scroll_poly_disp();
void Fade_Out_Init();
void taiiku_seat_set();
void taiiku_result_sprite_init();
void taiiku_seat_disp();
void taiiku_count_inc();
void taiiku_scroll();
void taiiku_scroll_disp2();
void taiiku_race();
void taiiku_racers_move();
void taiiku_racers_collide();
void taiiku_scenery_init();
u32 Vblnk_Timer();

void taiiku_result_main()
{
    switch (now_sub_sub_step) {
    case 0:
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        taiiku_seat_disp();
        break;
    case 1:
        if (Vblnk_Timer() >= 91) {
            device_cmd_send(1282);
            sub_sub_step_up();
        }
        taiiku_seat_disp();
        break;
    case 2:
        taiiku_count_inc();
        taiiku_scroll();
        taiiku_scroll_disp2();
        taiiku_seat_set(taiiku_spoon_scrl_hi, 4);
        taiiku_race();
        taiiku_racers_move();
        taiiku_racers_collide();
        if (taiiku_goal_check3() != 0) {
            device_cmd_send(1284);
            Fade_Out_Init();
            sub_sub_step_up();
        }
        break;
    case 3:
        scroll_poly_disp(taiiku_spoon_scrl_hi, 15, 0, 112, 640);
        scroll_poly_disp(taiiku_spoon_scrl_hi % 128, 15, 112, 128, 128);
        if (bri_fade_out() != 0) {
            device_cmd_send(194);
            taiiku_scenery_init();
            taiiku_result_sprite_init();
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
