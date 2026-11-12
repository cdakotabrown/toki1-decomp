/* Address: 0x800650E8 */
/* Size: 92 bytes (23 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 mes_win_cw_disp;
extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on(s32);

void message_window_show(void) {
    if (mes_win_cw_disp & 0x80) {
        _sprite_set_box_shade(-133, 46, 266, 52, 8, 0x30e00, 0);
        dtd_on(8);
    }
}
