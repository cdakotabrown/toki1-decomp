/* Function: date_main */
/* Overlay: DATE */
#include "types.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;

extern void game_reset(void);
extern void don_move(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void update_anim_state(s32);
extern void k_scroll_set(void);
extern void bustup_ani(void);
extern void dir_scroll_main(void);
extern void k_disp_wait(void);
extern void date_spot_arrival_step(void);
extern void date_spot_scene_step(void);
extern void date_player_birthday_step(void);
extern void date_girl_birthday_step(void);

void date_main(void)
{
    bustup_ani();

    switch (now_sub_step) {
        case 0:
            date_spot_arrival_step();
            break;
        case 1:
            date_spot_scene_step();
            break;
        case 2:
            date_girl_birthday_step();
            break;
        case 3:
            date_player_birthday_step();
            break;
        default:
            game_reset();
            break;
    }

    k_scroll_set();
    k_disp_wait();

    if (now_sub_step != 0 || now_sub_sub_step != 0x45) {
        update_anim_state(2);
    }
    message_window_show();

    basyo_show();
    hizuke_show();
    dir_scroll_main();
    don_move();
}
