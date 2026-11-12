/* @SymbolSize: 56 */
/* Address: 0x80136B84 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_pack_set2();

void shougatu_cd_load(void)
{
    cd_read_start2(3, 0x801B0000, 44867);
    shougatu_pack_set2();
    sub_sub_step_up();
}
