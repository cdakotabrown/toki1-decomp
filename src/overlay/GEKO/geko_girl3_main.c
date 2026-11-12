/* @SymbolStart: 0x8013C4A0 */
/* @SymbolSize: 64 */
/* geko_girl3_main -- GEKO overlay, Address: 0x8013C4A8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void geko_girl3_ev1_main(void);
extern void game_reset(void);

extern s32 work_tarao;

void geko_girl3_main(void)
{
    s32 t6 = work_tarao;

    if (t6 != 1) goto label0;
    geko_girl3_ev1_main();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
