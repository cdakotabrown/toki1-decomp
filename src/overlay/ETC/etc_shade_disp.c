/* Address: 0x80148E14 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void dtd_on();
extern void _sprite_set_box_shade();

void etc_shade_disp(void)
{
    _sprite_set_box_shade(-160, -104, 320, 142, 5, 0xE0E0E0, 2);
    dtd_on(5);
}
