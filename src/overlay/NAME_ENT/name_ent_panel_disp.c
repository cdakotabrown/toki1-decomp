/* @SymbolSize: 540 */
/* name_ent_panel_disp - NAME_ENT overlay, Address: 0x801354E4, Size: 540 bytes (135 insns) */
#include "types.h"

extern u32 name_ent_tab_no;
extern u32 pad_trig;

extern void _sprite_set_box_shade();
extern void dtd_on_tpage();
extern void name_ent_names_select();

void name_ent_panel_disp(s32 mode)
{
    u32 color;
    u32 input;

    if (mode != 0) {
        color = 0x00090002;
    } else {
        color = 0x0008081F;
    }

    _sprite_set_box_shade(-157, -48, 27, 15, 4, color, 131);
    _sprite_set_box_shade(-157, -16, 27, 15, 4, color, 131);
    _sprite_set_box_shade(-157, 16, 27, 15, 4, color, 131);
    _sprite_set_box_shade(-157, 48, 27, 15, 4, color, 131);
    _sprite_set_box_shade(-157, 80, 27, 15, 4, color, 131);
    _sprite_set_box_shade(-154, -106, 304, 32, 4, color, 131);
    _sprite_set_box_shade(-127, -64, 285, 112, 4, color, 131);

    if (name_ent_tab_no == 3) {
        _sprite_set_box_shade(-122, 54, 80, 48, 4, color, 131);
        _sprite_set_box_shade(-26, 54, 80, 48, 4, color, 131);
        _sprite_set_box_shade(70, 54, 80, 48, 4, color, 131);
    }

    dtd_on_tpage(0, 0, 4, 0, 0);

    input = pad_trig;
    if (input & 0x20) {
        return;
    }
    if (input & 0x800) {
        return;
    }
    if (input & 0x40) {
        return;
    }
    name_ent_names_select();
}
