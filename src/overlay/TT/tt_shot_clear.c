/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x80133B70 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 *tt_shot_buf;

void tt_shot_clear(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_shot_buf; i != 128; i += 4) {
        p[104] = 0;
        p[208] = 0;
        p[312] = 0;
        p += 416;
        p[-416] = 0;
    }
}
