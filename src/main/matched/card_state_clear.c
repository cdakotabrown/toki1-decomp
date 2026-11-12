/* Address: 0x80054100 */
/* Size: 20 bytes (5 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "common.h"

extern u8 card_busy;
extern u32 card_cnt;

void card_state_clear(void) {
    card_busy = 0;
    card_cnt = 0;
}
