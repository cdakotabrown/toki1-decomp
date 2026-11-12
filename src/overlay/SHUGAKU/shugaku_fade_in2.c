/* @SymbolStart: 0x8013A8BC */
/* @SymbolSize: 104 */
#include "types.h"

extern u8 shugaku_place;

extern void fade_in_step(void);

extern s32 serifu_no;
extern u8  fade_bri;
extern u8  cw97_bright;
extern u8  cw97_disp;

void shugaku_fade_in2(void)
{
    u8 v0;

    if (shugaku_place == 1) {
        if (serifu_no != 0) {
            v0 = fade_bri;
            cw97_bright = v0;
            if (v0 != 0) {
                cw97_disp |= 0x80;
            }
        }
    }
    fade_in_step();
}
