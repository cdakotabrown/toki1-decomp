/* name_ent_white_box_show - NAME_ENT overlay, Address: 0x80147944, Size: 120 bytes (30 insns) */
#include "types.h"

extern void _sprite_set_box_shade();

void name_ent_white_box_show(void)
{

    _sprite_set_box_shade(-160, -120, 160, 160, 13, 0x00FFFFFF, 1);
    _sprite_set_box_shade(0,    -120, 160, 160, 13, 0x00FFFFFF, 1);
}
