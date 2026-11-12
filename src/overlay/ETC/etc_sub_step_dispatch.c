/* @SymbolStart: 0x8014AB50 */
/* @SymbolSize: 228 */
/* @RodataBase: 0x8014D9D0 */
#include "types.h"

extern void etc_soudan_setup_dispatch(void);
extern void etc_soudan_talk_dispatch(void);
extern void etc_soudan_choice_dispatch(void);
extern void etc_sub_sub_dispatch(void);
extern void etc_soudan_singaku_dispatch(void);
extern void etc_soudan_syuusyoku_dispatch(void);
extern void etc_soudan_exit(void);
extern void step_directional_scroll_once(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void update_anim_state(s32);
extern void dec_bg_show(s32);
extern void mascot_move(void);

extern u8 now_sub_step;

void etc_sub_step_dispatch(void) {
    s32 cmd = now_sub_step;

    switch (cmd) {
    case 0:
        etc_soudan_setup_dispatch();
        break;
    case 1:
        etc_soudan_talk_dispatch();
        break;
    case 2:
        etc_soudan_choice_dispatch();
        break;
    case 3:
        etc_sub_sub_dispatch();
        break;
    case 4:
        etc_soudan_singaku_dispatch();
        break;
    case 5:
        etc_soudan_syuusyoku_dispatch();
        break;
    case 6:
        etc_soudan_exit();
        break;
    }
    step_directional_scroll_once();
    hizuke_show();
    message_window_show();
    basyo_show();
    update_anim_state(0);
    dec_bg_show(1);
    mascot_move();
}
