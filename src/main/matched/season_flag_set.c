/* Address: 0x8006D0A8, Size: 44 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_month;
extern u8 byouki_state;
void season_flag_set(void) {
    if (now_month == 9) {
        byouki_state = (u8)(byouki_state & 0xFFFB);
    }
}
