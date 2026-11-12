/* Address: 0x800860F0, Size: 28 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s32 vblnk_counter;
extern s32 vblnk_timer_base;
extern s32 vblnk_timer_diff;
void Vblnk_Timer_Init(void) {
    vblnk_timer_base = vblnk_counter;
    vblnk_timer_diff = 0;
}
