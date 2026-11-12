/* kangei_main - KANGEI overlay, Address: 0x80132000, Size: 132 bytes (33 insns) */
/* @SymbolSize: 132 */
#include "types.h"

extern void bustup_ani(void);
extern void kangei_sub_dispatch(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);
extern u8 now_sub_step;

void
kangei_main(void)
{
    u8 flag;

    bustup_ani();
    flag = now_sub_step;
    if (flag == 0) {
        kangei_sub_dispatch();
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
