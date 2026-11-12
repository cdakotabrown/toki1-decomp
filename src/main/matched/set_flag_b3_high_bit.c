/* Address: 0x8006BE0C, Size: 60 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 cursor_cw_disp;
void set_flag_b3_high_bit(s32 a0) {
    if (a0 == 1) {
        cursor_cw_disp = (u8)(cursor_cw_disp | 0x80);
    } else {
        cursor_cw_disp = (u8)(cursor_cw_disp & 0x7F);
    }
}
