/* Function: SD_FadePoll */
/* Address: 0x8007A56C */
/* Size: 40 bytes (10 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern void serial_vol_fade(void);
extern s32 gSDSerialInfo;

void SD_FadePoll(void) {
    s32 val = gSDSerialInfo;

    if ((val & 0x100) != 0) {
        serial_vol_fade();
    }
}
