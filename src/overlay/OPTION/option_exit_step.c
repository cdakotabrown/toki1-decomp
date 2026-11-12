/* @SymbolStart: 0x80132A90 */
/* @SymbolSize: 44 */
/* Address: 0x80132A98, Size: 44 bytes (11 instructions incl. cross-prologue head) */
#include "types.h"

extern void option_exit_fade();

extern u8 now_sub_sub_step;

void option_exit_step(void) {
    u8 flag;

    flag = now_sub_sub_step;
    if (flag == 0) {
        option_exit_fade();
    }
}
