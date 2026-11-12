/* @SymbolStart: 0x80137908 */
/* @SymbolSize: 72 */
/* @PostProc: pad_trailing_nop --count 2 */
/* geko_wipe_step2 -- GEKO overlay, Address: 0x80137908, Size: 72 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern void wipe_main2(void);

extern u8 geko_ans0_flag;

void geko_wipe_step2(void)
{
    s32 t6 = geko_ans0_flag;

    if (t6 != 0) goto label0;
    sub_sub_step_up();
    goto label1;

label0:
    wipe_main2();

label1:
    ;
}
