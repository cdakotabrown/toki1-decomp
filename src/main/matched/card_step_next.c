/* Address: 0x800540B0 */
/* Size: 32 bytes (8 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern volatile u8 card_result;
extern volatile u8 card_step;

void card_step_next(void) {
    card_result = 0;
    card_step = card_step + 1;
}
