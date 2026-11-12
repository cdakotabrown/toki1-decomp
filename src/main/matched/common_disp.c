/* Likely Symbol: Yubi (from o.bin, offset +40 bytes from 0x8008661C) */
/* Address: 0x80086644 */
/* Size: 88 bytes (22 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_disp_wait(void);
extern void k_scroll_set(void);
extern void dir_scroll_main(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void update_anim_state(s32);
extern void basyo_show(void);
extern void don_move(void);

void common_disp(void) {
    k_disp_wait();
    k_scroll_set();
    dir_scroll_main();
    message_window_show();
    hizuke_show();
    update_anim_state(2);
    basyo_show();
    don_move();
}
