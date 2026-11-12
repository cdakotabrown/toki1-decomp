/* Address: 0x8006369C, Size: 36 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 anim_cnt;
void bustup_timer_clamp(void) {
    if ((u32)anim_cnt >= 0xA5) {
        anim_cnt = 0;
    }
}
