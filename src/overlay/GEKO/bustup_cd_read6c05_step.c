/* @SymbolSize: 44 */
/* Address: 0x801419F4 */
/* Size: 40 bytes (10 instructions) + 1 word of window padding */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

void bustup_cd_read6c05_step(void)
{
    bustup_cd_read(27653);
    sub_sub_step_up();
}
