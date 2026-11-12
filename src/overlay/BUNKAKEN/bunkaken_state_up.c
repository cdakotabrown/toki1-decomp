/* Address: 0x801454F8 */
/* Size: 48 bytes (12 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern s32 bunka_state;

void bunkaken_state_up(void) {
    if (now_sub_sub_step != 0) {
        now_sub_sub_step = 0;
        bunka_state++;
    }
}
