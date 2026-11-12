/* @RodataBase: 0x8013A000 */
/* Address: 0x80132214, Size: 288 bytes (72 insns) */
/* @SymbolSize: 288 */
#include "types.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;

void date2_intro_step(void);
void date2_girl_talk_step(void);
void date2_sub_sub_dispatch(void);
void date2_answer_react_step(void);
void date2_birthday_serifu_step(void);
void game_reset(void);
void k_scroll_set(void);
void k_disp_wait(void);
void update_anim_state(s32 arg0);
void dec_bg_show(s32 arg0);
void basyo_show(void);
void hizuke_show(void);
void dir_scroll_main(void);
void message_window_show(void);
void bustup_ani(void);
void parameter_show(void);

void date2_step_main(void)
{
    bustup_ani();

    switch (now_sub_step) {
    case 0:
        date2_intro_step();
        break;
    case 1:
        date2_girl_talk_step();
        break;
    case 2:
        date2_sub_sub_dispatch();
        break;
    case 3:
        date2_answer_react_step();
        break;
    case 4:
        date2_birthday_serifu_step();
        break;
    default:
        game_reset();
        break;
    }

    k_scroll_set();
    k_disp_wait();

    if (now_sub_step == 0 && now_sub_sub_step < 19u) {
        parameter_show();
        dec_bg_show(0);
    } else {
        update_anim_state(2);
        dir_scroll_main();
    }
    message_window_show();
    basyo_show();
    hizuke_show();
}
