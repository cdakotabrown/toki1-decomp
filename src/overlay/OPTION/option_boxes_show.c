/* Address: 0x8013A4A4, Size: 120 bytes (30 instructions) */
#include "types.h"

extern void _sprite_set_box_shade();
extern void dtd_on();

void option_boxes_show(void) {
    _sprite_set_box_shade(-80, -64, 176, 144, 10, 0x001E021D, 0);
    _sprite_set_box_shade(-150, 4, 80, 44, 10, 0x00101802, 0);
    dtd_on(10);
}
