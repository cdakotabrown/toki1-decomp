/* @SymbolStart: 0x8013A0F4 */
/* @SymbolSize: 60 */
/* geko_girl0_ev2_main -- GEKO overlay, Address: 0x8013A0FC, Size: 52 bytes */
#include "types.h"

extern void geko_sub_sub_dispatch23(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_girl0_ev2_main(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_sub_sub_dispatch23();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
