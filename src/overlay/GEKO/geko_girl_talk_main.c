/* geko_girl_talk_main -- GEKO overlay, Address: 0x80132244, Size: 164 bytes (41 insns) */
#include "types.h"

extern void bustup_ani(void);
extern void geko_dispatch0(void);
extern void geko_dispatch1(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

extern u8 now_sub_step;

void geko_girl_talk_main(void)
{
    bustup_ani();

    switch (now_sub_step) {
        case 0:
            geko_dispatch0();
            break;
        case 1:
            geko_dispatch1();
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
}
