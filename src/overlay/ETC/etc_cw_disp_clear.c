/* @SymbolSize: 48 */
/* Address: 0x8013EE6C */
/* Size: 48 bytes (12 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 etc_event_flag;
extern s32 Bg;
extern u8 cal_cw_disp;
extern u8 cw97_disp;
extern CLASS_W class_w[];

void etc_cw_disp_clear(void)
{
    etc_event_flag = 0;
    Bg = (s32)0x80000000;
    cal_cw_disp = 0;
    cw97_disp = 0;
    class_w[98].disp_flag = 0;
}
