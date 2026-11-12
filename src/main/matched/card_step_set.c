/* Address: 0x8005409C */
/* Size: 20 bytes (5 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 card_result;
extern u8 card_step;

void card_step_set(s32 arg0) {
    card_result = 0;
    card_step = (u8)arg0;
}
