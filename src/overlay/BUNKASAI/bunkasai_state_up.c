/* Address: 0x80142B98 */
/* Size: 48 bytes */
/* @SymbolSize: 48 */
#include "types.h"

extern u8 now_sub_sub_step;
extern s32 bunka_state;

void bunkasai_state_up(void)
{
    if (now_sub_sub_step) {
        now_sub_sub_step = 0;
        bunka_state++;
    }
}
