/* @SymbolStart: 0x8013F49C */
/* @SymbolSize: 60 */
/* geko_scene67_guard -- GEKO overlay, Address: 0x8013F4A4, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_scene67_phase(void);
extern void game_reset(void);

extern u8 now_sub_step;

void geko_scene67_guard(void)
{
    s32 t6 = now_sub_step;

    if (t6 != 0) goto label0;
    geko_scene67_phase();
    goto label1;

label0:
    game_reset();

label1:
    ;
}
