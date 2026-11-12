/* @SymbolStart: 0x8013A1E0 */
/* @SymbolSize: 60 */
/* geko_state_guard -- GEKO overlay, Address: 0x8013A1E8, Size: 52 bytes */
#include "types.h"

extern void geko_state_dispatch(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_state_guard(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_state_dispatch();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
