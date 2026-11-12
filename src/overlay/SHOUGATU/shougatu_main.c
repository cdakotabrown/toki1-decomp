/* @RodataBase: 0x801439A0 */
/* @SymbolSize: 296 */
/* Address: 0x8013D258, Size: 296 bytes (74 instructions) */
#include "types.h"

extern void bustup_ani(void);
extern void shougatu_case0_step_dispatch(void);
extern void shougatu_case1_cal_cw_step(void);
extern void shougatu_case2_cal_cw_step(void);
extern void shougatu_case3_pair_phase_step(void);
extern void shougatu_case4_fade_step(void);
extern void shougatu_case5_girl_dispatch(void);
extern void shougatu_case6_crowd_move_step(void);
extern void shougatu_case7_crowd_step(void);
extern void shougatu_case8_date_sprite_step(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);
extern void don_move(void);

extern u8 now_sub_step;

void shougatu_main(void)
{
    bustup_ani();
    switch (now_sub_step) {
    case 0:
        shougatu_case0_step_dispatch();
        break;
    case 1:
        shougatu_case1_cal_cw_step();
        break;
    case 2:
        shougatu_case2_cal_cw_step();
        break;
    case 3:
        shougatu_case3_pair_phase_step();
        break;
    case 4:
        shougatu_case4_fade_step();
        break;
    case 5:
        shougatu_case5_girl_dispatch();
        break;
    case 6:
        shougatu_case6_crowd_move_step();
        break;
    case 7:
        shougatu_case7_crowd_step();
        break;
    case 8:
        shougatu_case8_date_sprite_step();
        break;
    default:
        game_reset();
        break;
    }
    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
    don_move();
}
