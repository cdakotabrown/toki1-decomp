/* @SymbolStart: 0x80132000 */
/* @SymbolSize: 276 */
/* @RodataBase: 0x80136F6C */
/* olh_main (olh_main) - OLH overlay, Address: 0x80132008, Size: 268 bytes (67 insns) */
#include "types.h"

extern u8 now_sub_step;

extern void olh_init(void);
extern void olh(void);
extern void olh_exit(void);
extern void olh_sys_page_main(void);
extern void olh_card_page_main(void);
extern void olh_file_page_main(void);
extern void olh_bio_page_main(void);
extern void olh_icon_page_main(void);
extern void olh_minigame_page_main(void);
extern void olh_trivia_page_main(void);
extern void step_directional_scroll_once(void);
extern void goto_step(s32 a0);
extern void olh_box_show(void);
extern void basyo_show(void);

extern s32 pad_trig;

void
olh_main(void)
{
    u8 idx;

    idx = now_sub_step;
    switch (idx) {
    case 0:
        olh_init();
        break;
    case 1:
        olh();
        break;
    case 2:
        olh_exit();
        break;
    case 16:
        olh_sys_page_main();
        break;
    case 32:
        olh_card_page_main();
        break;
    case 48:
        olh_file_page_main();
        break;
    case 64:
        olh_bio_page_main();
        break;
    case 80:
        olh_icon_page_main();
        break;
    case 96:
        olh_minigame_page_main();
        break;
    case 112:
        olh_trivia_page_main();
    }
    step_directional_scroll_once();
    if (pad_trig & 0x0800) {
        goto_step(52);
    }
    olh_box_show();
    basyo_show();
}
