/* @SymbolSize: 60 */
/* Address: 0x80139D98 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_res_set(void);

void shougatu_load1(void)
{
    cd_read_start2(113, (void *)0x801AE000, 0x8768);
    shougatu_res_set();
    sub_sub_step_up();
}
