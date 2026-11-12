/* @SymbolSize: 132  * Status: BYTE-PERFECT 🎉 */
/* geko_girl_sasoi_main -- GEKO overlay, Address: 0x80142600, Size: 132 bytes (33 insns), Semi-matched */
#include "types.h"

extern void bustup_ani(void);
extern void geko_sub_dispatch(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32 a0);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

extern u8 now_sub_step;

void geko_girl_sasoi_main(void)
{
    bustup_ani();
    if (now_sub_step != 0) goto label0;
    geko_sub_dispatch();
    goto label1;

label0:
    game_reset();

label1:
    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
