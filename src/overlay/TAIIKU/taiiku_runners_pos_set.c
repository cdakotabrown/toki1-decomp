/* Address: 0x8013F534 */
/* Size: 72 bytes (18 insns) */
#include "types.h"

extern void taiiku_runner_pos_set(s32 a0);
extern void taiiku_cursor_advance(void);

void taiiku_runners_pos_set(void) {
    s32 i;

    for (i = 0; i != 4; i++) {
        taiiku_runner_pos_set(i);
    }
    taiiku_cursor_advance();
}
