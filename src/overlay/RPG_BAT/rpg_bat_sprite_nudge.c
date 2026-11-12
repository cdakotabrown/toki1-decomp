/* @SymbolSize: 244 */
/* Address: 0x8013F3F0, Size: 244 bytes (61 insns) */
#include "types.h"

extern u8 cw40_active[];
extern u8 cw64_active[];
extern volatile s16 cw125_x;
extern volatile s16 cw126_x;
extern volatile s16 cw127_x;
extern volatile s16 cw125_y;
extern volatile s16 cw126_y;
extern volatile s16 cw127_y;

void rpg_bat_sprite_nudge(s32 a0, s32 a1)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        *(s16 *)(cw40_active + 6566 + i * 68) += a0;
        *(s16 *)(cw40_active + 6570 + i * 68) += a1;
    }
    cw125_x += a0;
    cw126_x += a0;
    cw127_x += a0;
    cw125_y += a1;
    cw126_y += a1;
    cw127_y += a1;
}
