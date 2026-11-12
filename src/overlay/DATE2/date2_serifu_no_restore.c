/* @SymbolStart: 0x801376C4 */
/* date2_serifu_no_restore - DATE2 overlay, Address: 0x801376CC, Size: 36 bytes (9 insns) */
/* @SymbolSize: 44 */
#include "types.h"

extern u8 date2_serifu_no_save;
extern s32 serifu_no;

void sub_sub_step_up(void);

void date2_serifu_no_restore(void)
{
    serifu_no = date2_serifu_no_save;
    sub_sub_step_up();
}
