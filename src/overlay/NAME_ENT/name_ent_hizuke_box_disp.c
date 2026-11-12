/* name_ent_hizuke_box_disp - NAME_ENT overlay, Address: 0x801362CC, Size: 180 bytes (45 insns) */
#include "types.h"

extern void _sprite_set_box_shade();

void name_ent_hizuke_box_disp(s32 mode)
{
    u32 color;

    if (mode != 0) {
        color = 0x00090002;
    } else {
        color = 0x0008081F;
    }

    _sprite_set_box_shade(-154, -106, 304, 32, 4, color, 131);

    _sprite_set_box_shade(-124, -61, 280, 158, 4, color, 131);

    _sprite_set_box_shade(-157, 80, 28, 16, 4, color, 131);
}
