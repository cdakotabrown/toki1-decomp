/* @SymbolStart: 0x8013D550 */
/* @SymbolSize: 60 */
/* geko_sub_sub_guard3 -- GEKO overlay, Address: 0x8013D558, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_sub_sub_dispatch3(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_sub_sub_guard3(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_sub_sub_dispatch3();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
