/* @SymbolSize: 156 */
/* /* omimai_main - OMIMAI overlay, Address: 0x80132000, Size: 160 bytes (40 insns reachable + nop) */
#include "types.h"

extern u8 now_sub_step;
extern void bustup_ani(void);
extern void omimai_main0(void);
extern void omimai_main1(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

void omimai_main(void)
{
    u8 v0;

    bustup_ani();
    v0 = now_sub_step;
    switch (v0) {
        case 0:
            omimai_main0();
            break;
        case 1:
            omimai_main1();
            break;
        default:
            game_reset();
            break;
    }
    k_scroll_set();
    k_disp_wait();
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
