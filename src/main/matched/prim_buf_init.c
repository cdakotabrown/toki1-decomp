/* Address: 0x80049340 */
/* Size: 48 bytes (12 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2, leaf */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 screen_w[];

void prim_buf_init(s32 buf, s32 slot) {
    u8 *p = &screen_w[slot * 12];
    *(s32 *)(p + 36) = buf;
    *(s32 *)(p + 28) = 0x10C00;
    *(s32 *)(p + 32) = 0;
}
