/* @SymbolStart: 0x8013BA34 */
/* @SymbolSize: 60 */
/* geko_girl12_ev3_main -- GEKO overlay, Address: 0x8013BA3C, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_sub_sub_dispatch6(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_girl12_ev3_main(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_sub_sub_dispatch6();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
