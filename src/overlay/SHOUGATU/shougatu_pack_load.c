/* @SymbolSize: 56 */
/* /* shougatu_pack_load - SHOUGATU overlay, Address: 0x80136570, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_pack_set();

void shougatu_pack_load(void)
{
    cd_read_start2(22, 0x801B0000, 44813);
    shougatu_pack_set();
    sub_sub_step_up();
}
