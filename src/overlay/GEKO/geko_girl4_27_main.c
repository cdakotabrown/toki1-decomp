/* @SymbolStart: 0x8013EE90 */
/* @SymbolSize: 60 */
/* geko_girl4_27_main -- GEKO overlay, Address: 0x8013EE98, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_girl4_27_phase(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_girl4_27_main(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_girl4_27_phase();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
