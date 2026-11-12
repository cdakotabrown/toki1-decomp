/* Address: 0x80132274, Size: 280 bytes (53 body insns) */
/* @SymbolSize: 212 */
#include "types.h"

extern u8 now_sub_step;
extern void bustup_ani(void);
extern void valen_intro_dispatch(void);
extern void valen_list_dispatch(void);
extern void valen_result_dispatch(void);
extern void valen_cel_dispatch(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

void valen_main(void)
{
    bustup_ani();
    switch (now_sub_step) {
        case 0:
            valen_intro_dispatch();
            break;
        case 1:
            valen_list_dispatch();
            break;
        case 2:
            valen_result_dispatch();
            break;
        case 3:
            valen_cel_dispatch();
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
