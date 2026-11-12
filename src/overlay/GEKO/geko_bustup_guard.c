/* @SymbolStart: 0x80141AF0 */
/* @SymbolSize: 60 */
/* geko_bustup_guard -- GEKO overlay, Address: 0x80141AF8, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_bustup_dispatch(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_bustup_guard(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_bustup_dispatch();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
