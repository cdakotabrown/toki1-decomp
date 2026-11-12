/* Address: 0x80135AFC */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void class_clear_n();
extern u8 shougatu_omikuji_box_on;

void shougatu_omikuji_clear(void)
{
    class_clear_n(96);
    class_clear_n(97);
    class_clear_n(98);
    class_clear_n(99);
    shougatu_omikuji_box_on = 0;
    sub_sub_step_up();
}
