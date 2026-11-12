/* Address: 0x8005B258, Size: 120 bytes (30 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern void goto_step(s32 n);

s32 hotspot_step_check(void) {
    s32 v;

    v = (s32)(s16)cursor_cw_x;
    if (v >= 80) goto no;
    if (v < 65) goto no;
    v = (s32)(s16)cursor_cw_y;
    if (v >= -16) goto no;
    if (v < -31) goto no;
    if ((pad_trig & 0x20) == 0) goto no;
    goto_step(58);
    return 1;
no:
    return 0;
}
