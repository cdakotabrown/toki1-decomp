/* @SymbolStart: 0x80136E90 */
/* @SymbolSize: 60 */
/* geko_serifu_run2_step -- GEKO overlay, Address: 0x80136E98, Size: 52 bytes */
#include "types.h"

extern s32 geko_serifu_run2(void);
extern void sub_sub_step_up(void);

extern u8 geko_girl_flag;

void geko_serifu_run2_step(void)
{
    s32 t6 = geko_girl_flag;

    if (t6 != 0) goto label0;
    geko_serifu_run2();
    goto label1;

label0:
    sub_sub_step_up();

label1:
    ;
}
