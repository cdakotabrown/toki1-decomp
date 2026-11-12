/* @CompileEnv: IDO52_COPYPROP_SITE=lod:6:deny,lod:12:deny */
/* @SymbolStart: 0x801371B4 */
/* @SymbolSize: 460 */
/* Address: 0x801371BC, Size: 452 bytes (113 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void goto_sub_step();
extern void goto_sub_sub_step();
extern void option_row_slide();

extern u8  step_mode;

extern volatile s16 cw106_scale_x;
extern volatile s16 cw106_scale_y;
extern volatile u8  cw107_disp;
extern volatile u8  cw108_disp;
extern volatile u8  cw109_disp;
extern volatile u8  cw110_disp;
extern CLASS_W class_w[];

void option_zoom2(void)
{
    s32 phase;
    s32 divisor;
    s16 zoom;

    phase = step_mode;

    if (phase == 0) {
        cw107_disp = cw107_disp & ~0x80;
        cw108_disp = cw108_disp & ~0x80;
        cw109_disp = cw109_disp & ~0x80;
        cw110_disp = cw110_disp & ~0x80;

        goto L0;
L0:
        divisor = 9;
        zoom = cw106_scale_x;
        zoom = (s16)((zoom / divisor) * 8);
        *(s16 *)&cw106_scale_x = zoom;
        if (zoom < 9) {
            zoom = 1;
            step_mode = zoom;
        }
    } else if (phase == 2) {
        cw107_disp = cw107_disp & 0xFF7F;
        cw108_disp = cw108_disp & 0xFF7F;
        cw109_disp = cw109_disp & 0xFF7F;
        cw110_disp = cw110_disp & 0xFF7F;

        goto L1;
L1:
        divisor = 9;
        zoom = cw106_scale_x;
        zoom = (s16)((zoom * divisor) / 8);
        cw106_scale_y = (s16)((cw106_scale_y * divisor) / 8);
        cw106_scale_x = zoom;
        if (zoom >= 16384) {
            zoom = 1;
            step_mode = zoom;
        }
    } else {
        goto_sub_step(0);
        goto_sub_sub_step(2);
    }

    option_row_slide();
    class_w[106].disp_flag = 0;
}
