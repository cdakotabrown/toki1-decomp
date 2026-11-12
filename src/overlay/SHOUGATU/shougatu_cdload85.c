/* @SymbolSize: 56 */
/* /* shougatu_cdload85 - SHOUGATU overlay, Address: 0x80141D70, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_buf_init();

void shougatu_cdload85(void)
{
    cd_read_start2(85, 0x801B0000, 32869);
    shougatu_buf_init();
    sub_sub_step_up();
}
