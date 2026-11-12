/* @SymbolStart: 0x8013D0A0 */
/* @SymbolSize: 64 */
/* geko_girl1_main -- GEKO overlay, Address: 0x8013D0A8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void geko_sub_sub_guard(void);
extern void game_reset(void);

extern s32 work_tarao;

void geko_girl1_main(void)
{
    s32 t6 = work_tarao;

    if (t6 != 1) goto label0;
    geko_sub_sub_guard();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
