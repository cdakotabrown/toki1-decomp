/* @SymbolSize: 132 */
/* /* omimai_main2 - OMIMAI overlay, Address: 0x80133120, Size: 132 bytes (33 insns) */
#include "types.h"

extern u8 now_sub_step;

extern void bustup_ani(void);
extern void omimai_sub_sub_dispatch(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

void omimai_main2(void)
{
    bustup_ani();

    if (now_sub_step == 0) {
        omimai_sub_sub_dispatch();
    } else {
        game_reset();
    }

    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
