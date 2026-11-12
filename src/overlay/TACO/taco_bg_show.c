/* Address: 0x80142F90 */
/* Size: 32 bytes (8 insns) */
#include "types.h"

extern void screen_color_show();

void taco_bg_show(void)
{
    screen_color_show(0x00800000);
}
