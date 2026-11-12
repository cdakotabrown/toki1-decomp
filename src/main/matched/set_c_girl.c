/* Address: 0x8005184C */
/* Size: 12 bytes (3 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 now_girl;

void set_c_girl(s32 arg0) {
    now_girl = (u8)arg0;
}
