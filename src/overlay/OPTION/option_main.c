/* @RodataBase: 0x8013CB88 */
/* @SymbolStart: 0x80133630 */
/* @SymbolSize: 304 */
#include "types.h"

extern void option_top_menu_step();
extern void option_k_speed_step();
extern void option_icon_mode_step();
extern void option_mouse_step();
extern void option_menu_mode_step();
extern void option_csr_speed_step();
extern void option_voice_step();
extern void option_omake_step();
extern void option_exit_step();
extern void game_reset();
extern void option_bg_main();
extern void option_cal_box_show();
extern void option_nop0();

extern u8 screen_bri;

typedef struct {
    s32 frame;
    s32 count;
    u8  unk8;
    u8  step;
} StepState;

extern StepState step_state;

void option_main(void) {
    step_state.frame++;
    step_state.count++;
    switch (step_state.step) {
        case 0:
            option_top_menu_step();
            break;
        case 1:
            option_k_speed_step();
            break;
        case 2:
            option_icon_mode_step();
            break;
        case 3:
            option_mouse_step();
            break;
        case 4:
            option_menu_mode_step();
            break;
        case 5:
            option_csr_speed_step();
            break;
        case 6:
            option_voice_step();
            break;
        case 7:
            option_omake_step();
            break;
        case 8:
            option_exit_step();
            break;
        default:
            game_reset();
            break;
    }

    option_bg_main();

    if (screen_bri != 0) {
        option_cal_box_show();
        option_nop0();
    }
}
