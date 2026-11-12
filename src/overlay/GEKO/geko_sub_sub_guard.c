/* @SymbolStart: 0x8013D0E0 */
/* @SymbolSize: 60 */
/* geko_sub_sub_guard -- GEKO overlay, Address: 0x8013D0E8, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_sub_sub_dispatch(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_sub_sub_guard(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_sub_sub_dispatch();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
