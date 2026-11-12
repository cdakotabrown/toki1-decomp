/* Function: uwasa_main */
/* Address: 0x80060E74 */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_step;
extern void uwasa_init(void);
extern void uwasa0(void);
extern void icon_cursor_move(s32);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void dec_bg_show(s32);
extern void update_anim_state(s32);

void uwasa_main(void) {
    s32 mode = now_sub_step;
    switch (mode) {
    case 0:
        uwasa_init();
        break;
    case 1:
        uwasa0();
        break;
    }
    icon_cursor_move(1);
    parameter_show();
    hizuke_show();
    message_window_show();
    basyo_show();
    dec_bg_show(0);
    update_anim_state(1);
}
