/* @SymbolSize: 76 */
/* Function: date_cw98_fade_out */
/* Overlay: DATE */
/* Address: 0x80151090 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void date_fade_out(void);
extern u8 fade_bri;
extern CLASS_W class_w[];

void date_cw98_fade_out(void)
{
    u8 flag;

    date_fade_out();
    flag = fade_bri;
    class_w[98].bright = flag;
    if ((u32)flag < 8U) {
        class_w[98].disp_flag = class_w[98].disp_flag & 0xFF7F;
    }
}
