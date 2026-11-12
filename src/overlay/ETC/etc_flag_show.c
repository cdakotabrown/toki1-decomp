/* @SymbolSize: 60 */
/* Address: 0x8014AB08 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void class_clear_f();
extern void printf();
extern void return_schedule();

extern u8 etc_sinro_show_fmt;
extern u8 sinro;

void etc_flag_show(void)
{
    class_clear_f();
    printf(&etc_sinro_show_fmt, sinro);
    return_schedule(1);
}
