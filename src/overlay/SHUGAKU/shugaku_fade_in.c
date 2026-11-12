/* @SymbolStart: 0x8013A87C */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 shugaku_place;

extern void fade_in_step(void);

extern u8  fade_bri;
extern u8  cw97_bright;

void shugaku_fade_in(void)
{
    s32 param_t6 = shugaku_place;

    if (param_t6 == 1) {
        cw97_bright = fade_bri;
    }
    fade_in_step();
}
