/* Address: 0x800710F8 */
/* Size: 216 bytes (54 instructions) */
/* Region: Game */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 screen_w[];

void girls_param_clamp(void) {
    s32 k;

    for (k = 0; k < 616; k += 56) {
        if (*(s16 *)&screen_w[k + 446] > 128) {
            *(s16 *)&screen_w[k + 446] = 128;
        }
        if (*(s16 *)&screen_w[k + 446] < 0) {
            *(s16 *)&screen_w[k + 446] = 0;
        }
        if (*(s16 *)&screen_w[k + 450] > 128) {
            *(s16 *)&screen_w[k + 450] = 128;
        }
        if (*(s16 *)&screen_w[k + 450] < 0) {
            *(s16 *)&screen_w[k + 450] = 0;
        }
        if (*(s16 *)&screen_w[k + 454] > 128) {
            *(s16 *)&screen_w[k + 454] = 128;
        }
        if (*(s16 *)&screen_w[k + 454] < 0) {
            *(s16 *)&screen_w[k + 454] = 0;
        }
        if (*(s16 *)&screen_w[k + 454] < 50) {
            screen_w[k + 458] &= 0xFFFE;
            screen_w[k + 458] &= 0xFD;
        }
    }
}
