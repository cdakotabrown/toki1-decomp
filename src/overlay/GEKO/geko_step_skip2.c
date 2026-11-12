/* @SymbolStart: 0x80136690 */
/* @SymbolSize: 56 */
/* geko_step_skip2 -- GEKO overlay, Address: 0x80136698, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 geko_girl_flag;

void geko_step_skip2(void)
{
    s32 t6 = geko_girl_flag;

    if (t6 != 1) goto label0;
    sub_sub_step_up();

label0:
    sub_sub_step_up();
}
