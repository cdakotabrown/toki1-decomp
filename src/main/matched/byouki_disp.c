/* Likely Symbol: byouki_disp (from o.bin, offset +44 bytes from 0x8007381C) */
/* Address: 0x80073848 - 0x800738A8 (96 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void return_schedule(s32 arg);
extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void icon_cursor_move(s32 arg);
extern void mascot_move(void);
extern void update_anim_state(s32 arg);
extern s32 get_last_gamen_mode(void);
extern void dec_bg_show(s32 arg);

void byouki_disp(void) {
    return_schedule(0);
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_cursor_move(0);
    mascot_move();
    update_anim_state(1);
    dec_bg_show(get_last_gamen_mode());
}
