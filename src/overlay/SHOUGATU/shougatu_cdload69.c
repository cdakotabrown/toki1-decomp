/* @SymbolSize: 56 */
/* Address: 0x8014330C */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_buf_init3();

void shougatu_cdload69(void)
{
    cd_read_start2(69, 0x801B0000, 35785);
    shougatu_buf_init3();
    sub_sub_step_up();
}
