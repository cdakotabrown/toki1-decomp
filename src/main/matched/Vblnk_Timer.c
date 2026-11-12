/* Address: 0x8008610C, Size: 32 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s32 vblnk_counter;
extern s32 vblnk_timer_base;
extern s32 vblnk_timer_diff;
s32 Vblnk_Timer(void) {
    s32 diff = vblnk_counter - vblnk_timer_base;
    vblnk_timer_diff = diff;
    return diff;
}
