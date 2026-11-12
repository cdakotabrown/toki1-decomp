/* @SymbolSize: 200 */
/* name_ent_confirm_box_disp - NAME_ENT overlay, Address: 0x80138318, Size: 200 bytes (50 insns) */
#include "types.h"

extern void _sprite_set_box_shade(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp10, s32 sp14, s32 sp18);

void name_ent_confirm_box_disp(s32 mode)
{
    u32 color;

    if (mode != 0) {
        color = 0x00090002;
    } else {
        color = 0x0008081f;
    }

    if (mode != 0) {

        _sprite_set_box_shade(-126, -61, 282, 88, 4, color, 131);
    } else {

        _sprite_set_box_shade(-126, -61, 282, 118, 4, color, 131);
    }

    _sprite_set_box_shade(-40, 59, 124, 38, 4, color, 131);
}
