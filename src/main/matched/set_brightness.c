/* Address: 0x800492BC */
/* Size: 112 bytes (28 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
#include "types.h"
extern volatile s8 bri_step;
extern volatile u8 screen_bri;
void set_brightness(void) {
    s32 delta = bri_step;
    s32 v;
    if (delta == 0) {
        return;
    }
    if (delta > 0) {
        v = screen_bri;
        v += delta;
        if (v >= 256) {
            v = 255;
        }
        screen_bri = v;
        return;
    }
    if (delta >= 0) {
        return;
    }
    v = screen_bri;
    v += delta;
    if (v < 0) {
        v = 0;
    }
    screen_bri = v;
}
