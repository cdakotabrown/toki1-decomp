/* date2_main - DATE2 overlay, Address: 0x80137F64, Size: 104 bytes (26 insns) */
/* @SymbolSize: 104 */
#include "types.h"

void bustup_ani(void);
void date2_main_dispatch(void);
void k_scroll_set(void);
void k_disp_wait(void);
void update_anim_state(s32 arg0);
void message_window_show(void);
void basyo_show(void);
void hizuke_show(void);
void dir_scroll_main(void);
void don_move(void);

void date2_main(void)
{
    bustup_ani();
    date2_main_dispatch();
    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
    don_move();
}
