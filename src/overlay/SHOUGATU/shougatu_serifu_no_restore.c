/* @SymbolStart: 0x801365A8 */
/* @SymbolSize: 44 */
/* shougatu_serifu_no_restore - SHOUGATU overlay, Address: 0x801365B0, Size: 56 bytes (14 insns) */
/* BYTE-PERFECT: Target has cross-function lui/lw at end - @SymbolSize: 44 excludes them. */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 serifu_no;
extern u8 shougatu_serifu_no_bak;

void shougatu_serifu_no_restore(void)
{
    serifu_no = shougatu_serifu_no_bak;
    sub_sub_step_up();
}
