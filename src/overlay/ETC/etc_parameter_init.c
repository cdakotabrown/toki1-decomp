/* @SymbolSize: 64 */
/* Address: 0x80147FE8 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void class_clear_f();
extern void text_sys_init();
extern void parameter_show_init();

void etc_parameter_init(void)
{
    parameter_show_init();
    parameter_show_init();
    class_clear_f();
    text_sys_init();
    sub_sub_step_up();
}
