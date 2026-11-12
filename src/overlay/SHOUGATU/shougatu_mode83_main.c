/* @RodataBase: 0x80143600 */
/* @SymbolSize: 256 */
/* Address: 0x801322D4, Size: 256 bytes (64 instructions) */
#include "types.h"

extern void bustup_ani(void);
extern void shougatu_main53a(void);
extern void shougatu_main53b(void);
extern void shougatu_main53c(void);
extern void shougatu_main53d(void);
extern void shougatu_main53e(void);
extern void shougatu_main53f(void);
extern void shougatu_step_run(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

extern u8 now_sub_step;

void shougatu_mode83_main(void)
{
    bustup_ani();
    switch (now_sub_step) {
    case 0:
        shougatu_main53a();
        break;
    case 1:
        shougatu_main53b();
        break;
    case 2:
        shougatu_main53c();
        break;
    case 3:
        shougatu_main53d();
        break;
    case 4:
        shougatu_main53e();
        break;
    case 5:
        shougatu_main53f();
        break;
    case 6:
        shougatu_step_run();
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
