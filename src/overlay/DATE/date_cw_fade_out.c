/* Function: date_cw_fade_out */
/* Overlay: DATE */
/* Address: 0x8014DF58 */
/* Size: 96 bytes (24 instructions) */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"

extern void date_fade_out(void);
extern u8 cal_cw_bright;
extern CLASS_W class_w[];

void date_cw_fade_out(void)
{
    u8 flag;

    date_fade_out();
    flag = cal_cw_bright;
    if (flag == 0) {
        class_w[98].disp_flag = class_w[98].disp_flag & 0xFF7F;
        class_w[99].disp_flag = class_w[99].disp_flag & 0xFF7F;
    }
    class_w[98].bright = flag;
    class_w[99].bright = flag;
}
