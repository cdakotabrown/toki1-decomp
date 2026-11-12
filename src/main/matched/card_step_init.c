/* Address: 0x800540D0 */
/* Size: 48 bytes (12 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8  card_busy;
extern u8  card_step;
extern u8  card_result;
extern u8  card_event_cnt;
extern s32 card_cnt;

void card_step_init(void) {
    card_busy = 1;
    card_step = 0;
    card_result = 0;
    card_event_cnt = 0;
    card_cnt = 0;
}
