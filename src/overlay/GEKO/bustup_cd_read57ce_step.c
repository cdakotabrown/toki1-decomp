/* @SymbolSize: 40 */
/* bustup_cd_read57ce_step -- GEKO overlay, Address: 0x8013A5F0, Size: 40 bytes (10 insns) */
#include "types.h"

extern void bustup_cd_read(s32 a0);
extern void sub_sub_step_up(void);

void bustup_cd_read57ce_step(void)
{
    bustup_cd_read(22478);
    sub_sub_step_up();
}
