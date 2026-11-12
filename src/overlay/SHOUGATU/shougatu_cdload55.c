/* @SymbolSize: 48  * Status: BYTE-PERFECT 🎉 */
/* Address: 0x80135B40 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();

void shougatu_cdload55(void)
{
    cd_read_start2(55, 0x801C0000, 48492);
    sub_sub_step_up();
}
