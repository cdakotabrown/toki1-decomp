/* @SymbolStart: 0x801363EC */
/* @SymbolSize: 488 */
/* @CompileFlags: -cckr */
#include "types.h"

extern unsigned char class_w[];
extern short cw106_scale_x;
extern short cw106_scale_y;
extern unsigned char cw106_disp;
extern unsigned char step_mode;

option_csr_speed_exit()
{
    int i;

    if (step_mode == 0) {
        for (i = 0; i < 5; i++) class_w[i * 68 + 7279] &= ~0x80;
        cw106_scale_x = cw106_scale_x / 9 * 8;
        if (cw106_scale_x < 9) {
            step_mode = 1;
        }
    } else if (step_mode == 2) {
        for (i = 0; i < 5; i++) class_w[i * 68 + 7279] &= 0xFF7F;
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
