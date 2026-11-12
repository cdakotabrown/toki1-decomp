/* Address: 0x80084EA0, Size: 48 bytes */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: rename_register_chain --chain t8:t9 --chain t9:t0 */
#include "types.h"
extern u8 Sprites[];
void sprite_rgb_set(s16 idx, s32 val) {
    u8 *p = &Sprites[idx * 36];
    p[0x16] = (u8)val;
    p[0x15] = (u8)val;
    p[0x14] = (u8)val;
}
