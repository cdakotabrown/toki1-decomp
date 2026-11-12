/* Address: 0x80140320 */
/* Size: 80 bytes (20 instructions) */
/* @SymbolSize: 52 */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();

void shougatu_cdload14(void)
{
    cd_read_start2(14, (void*)0x801D7000, 0xBE07);
    sub_sub_step_up();
}
