/* @SymbolSize: 664 */
#include "types.h"

extern void _sprite_set_light_effect1(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5,
                          s32 a6, s32 a7, s32 a8);
extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

void date_light_disp(void)
{
    _sprite_set_light_effect1(288, -248, 160, 60, -160, 60, 9, 0x00808080, 0);
    _sprite_set_light_effect1(288, -248, 64, 60, 0, 180, 9, 0x00A0A0A0, 0);
    _sprite_set_light_effect1(288, -248, 16, 60, 0, 180, 9, 0x00A0A0A0, 64);
    _sprite_set_light_effect1(288, -248, -28, 60, 0, 180, 9, 0x00A0A0A0, 99);
    _sprite_set_light_effect1(288, -248, -80, 60, 0, 180, 9, 0x00A0A0A0, 36);
    _sprite_set_light_effect1(288, -248, -140, 60, 0, 180, 9, 0x00A0A0A0, 115);
    _sprite_set_light_effect1(288, -248, -240, 60, 0, 180, 9, 0x00C0C0C0, 17);
    _sprite_set_light_effect1(288, -248, -320, 60, 0, 180, 9, 0x00C0C0C0, 80);
    _sprite_set_light_effect1(288, -248, -48, 60, 0, 180, 9, 0x00808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
    _sprite_set_light_effect1(-320, -280, -322, -321, -162, -121, 9, 0x00808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
}
