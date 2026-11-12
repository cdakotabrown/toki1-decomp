/* @SymbolSize: 56 */
/* Address: 0x801426C4 */
/* Size: 64 bytes (16 instructions with cross-prologue) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_buf_init2();

void shougatu_load2(void)
{
    cd_read_start2(69, 0x801B0000, 34161);
    shougatu_buf_init2();
    sub_sub_step_up();
}
