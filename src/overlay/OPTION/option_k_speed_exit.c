/* @SymbolStart: 0x80133B20 */
/* @SymbolSize: 372 */
/* @CompileFlags: -cckr -Wo,-loopunroll,0 */
#include "types.h"

extern unsigned char class_w[][68];
extern unsigned char cw3_active[];
extern short cw106_scale_x;
extern short cw106_scale_y;
extern unsigned char cw106_disp;
extern unsigned char step_mode;

option_k_speed_exit()
{
    unsigned char *p;

    if (step_mode == 0) {
        p = class_w[0]; do { p[7279] &= ~0x80; p += 68; } while (p != cw3_active);
        cw106_scale_x = cw106_scale_x / 9 * 8;
        if (cw106_scale_x < 9) {
            step_mode = 1;
        }
    } else if (step_mode == 2) {
        p = class_w[0];
        do {
            p[7279] &= ~0x80;
            p += 68;
        } while (p != cw3_active);
        cw106_scale_x = cw106_scale_x * 9 / 8;
        cw106_scale_y = cw106_scale_y * 9 / 8;
        if (cw106_scale_x >= 16384) {
            step_mode = 1;
        }
    } else {
        goto_sub_step(0);
        goto_sub_sub_step(2);
    }
    cw106_disp = 0;
}
