/* Address: 0x80142DE4 */
/* Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void debug_font_load();

void taco_debug_font_load_step(void)
{
    debug_font_load();
    sub_sub_step_up();
}
