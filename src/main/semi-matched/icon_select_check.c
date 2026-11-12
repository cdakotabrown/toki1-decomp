/* Address: 0x8005B6CC */
/* Size: 8 bytes (2 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 icon_mode;

u8 icon_select_check(void) {
    return icon_mode;
}
