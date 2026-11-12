/* @SymbolSize: 116  * Status: BYTE-PERFECT 🎉 */
/* geko_girl_birthday_main -- GEKO overlay, Address: 0x80134EC0, Size: 116 bytes (29 insns), Semi-matched */
#include "types.h"

extern void bustup_ani(void);
extern void girl_scene_scroll_main(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);

extern u8 now_sub_step;

void geko_girl_birthday_main(void)
{
    bustup_ani();
    if (now_sub_step != 0) goto label0;
    girl_scene_scroll_main();
    goto label1;

label0:
    game_reset();

label1:
    k_scroll_set();
    k_disp_wait();
    message_window_show();
    basyo_show();
    hizuke_show();
}
