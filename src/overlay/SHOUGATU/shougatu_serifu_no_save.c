/* @SymbolStart: 0x801347BC */
/* @SymbolSize: 44 */
/* Address: 0x801347C4 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s32 serifu_no_bak;
extern s32 serifu_no;

void shougatu_serifu_no_save(void)
{
    serifu_no_bak = serifu_no;
    sub_sub_step_up();
}
