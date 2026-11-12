/* @SymbolStart: 0x80140C78 */
/* @SymbolSize: 60 */
/* geko_main2b -- GEKO overlay, Address: 0x80140C80, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_sub_dispatch28(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_main2b(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_sub_dispatch28();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
