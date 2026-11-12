/* Address: 0x8013B584 */
/* Size: 232 bytes (58 instructions) */
/* @SymbolSize: 164 */
#include "types.h"

extern void game_reset(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void update_anim_state(s32);
extern void k_scroll_set(void);
extern void bustup_ani(void);
extern void dir_scroll_main(void);
extern void k_disp_wait(void);
extern void shougatu_main55a(void);
extern void shougatu_main55b(void);
extern u8 now_sub_step;

void shougatu_mode85_main(void)
{
    bustup_ani();
    switch (now_sub_step) {
    case 0:
        shougatu_main55a();
        break;
    case 1:
        shougatu_main55b();
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
