/* Address: 0x80148E64 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern void dtd_on();
extern void _sprite_set_box_shade();

void etc_box_shade_show(void)
{
    _sprite_set_box_shade(-65, -112, 132, 18, 4, 0x30E00, 0);
    dtd_on(4);
}
