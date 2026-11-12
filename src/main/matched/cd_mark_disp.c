/* Address: 0x80046730 */
/* Size: 132 bytes (33 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 mes_win_cw_disp;
extern s32 frame_counter;
extern void line_set(s32, s32, s32, s32, s32, s32, s32);

void cd_mark_disp(void) {
    if (mes_win_cw_disp & 0x80) {
        if (frame_counter & 0x04) {
            line_set(158, 90, 158, 98, 0x00FFFF00, 0x00FFFF00, 128);
            line_set(157, 90, 157, 98, 0x00FFFF00, 0x00FFFF00, 128);
        }
    }
}
