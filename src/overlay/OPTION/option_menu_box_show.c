/* @SymbolStart: 0x801320C4 */
/* @SymbolSize: 708 */
/* @RodataBase: 0x8013CB30 */
#include "types.h"

extern void _sprite_set_box_shade();
extern void dtd_on_tpage();
extern u8 screen_bri;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;

void option_menu_box_show(void) {
    u32 sel;

    sel = now_sub_sub_step;
    if (sel == 33) return;
    if (sel == 34) return;
    if (sel == 35) return;
    if (screen_bri == 0) return;
    switch (now_sub_step) {
    case 1:
        _sprite_set_box_shade(8, -104, 64, 144, 8, 0x10907, 128);
        break;
    case 2:
    case 4:
        _sprite_set_box_shade(-40, -72, 176, 128, 8, 0x10907, 128);
        break;
    case 3:
        _sprite_set_box_shade(-128, -48, 256, 128, 8, 0x10907, 128);
        break;
    case 5:
        _sprite_set_box_shade(8, -32, 64, 128, 8, 0x10907, 128);
        break;
    case 6:
        _sprite_set_box_shade(-16, 16, 144, 80, 8, 0x10907, 128);
        break;
    case 7:
        if (sel < 10) {
            _sprite_set_box_shade(-80, 8, 128, 88, 8, 0x10907, 128);
        } else if (sel < 20) {
            _sprite_set_box_shade(-88, -8, 220, 64, 8, 0x10907, 128);
        } else {
            _sprite_set_box_shade(-144, -88, 304, 144, 8, 0x10907, 128);
        }
        break;
    case 8:
        _sprite_set_box_shade(-80, 8, 128, 88, 8, 0x10907, 128);
        break;
    default:
        _sprite_set_box_shade(8, -104, 64, 144, 8, 0x10907, 128);
        break;
    case 0:
        break;
    }
    dtd_on_tpage(0, 0, 8, 0, 0);
}
