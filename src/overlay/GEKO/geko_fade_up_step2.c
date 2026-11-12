/* @SymbolStart: 0x80136FA4 */
/* @SymbolSize: 72 */
/* geko_fade_up_step2 -- GEKO overlay, Address: 0x80136FAC, Size: 64 bytes (16 insns), Matched */
#include "types.h"

extern void fade_up_main(void);
extern void sub_sub_step_up(void);

extern s16 geko_now_serifu2;
extern u8 geko_girl_flag;

void geko_fade_up_step2(void)
{
    s32 t6 = geko_girl_flag;

    if (t6 != 1) goto else_branch;
    fade_up_main();
    return;

else_branch:
    geko_now_serifu2 = 4;
    sub_sub_step_up();
}
