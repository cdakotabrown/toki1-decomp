/* @SymbolSize: 80 */
/* Address: 0x8013A51C, Size: 80 bytes (20 instructions) */
#include "types.h"

extern void _sprite_set_box_shade();
extern void dtd_on();

void option_box_show(void) {
    _sprite_set_box_shade(-10, -110, 80, 180, 6, 0x00A08080, 2);
    dtd_on(6);
}
