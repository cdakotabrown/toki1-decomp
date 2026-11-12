/* @SymbolStart: 0x80136F64 */
/* @SymbolSize: 64 */
/* geko_fade_up_step -- GEKO overlay, Address: 0x80136F6C, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void fade_up_main(void);

extern u8 geko_girl_flag;

void geko_fade_up_step(void)
{
    s32 t6 = geko_girl_flag;

    if (t6 != 1) goto label0;
    sub_sub_step_up();
    goto label1;

label0:
    fade_up_main();

label1:
    ;
}
