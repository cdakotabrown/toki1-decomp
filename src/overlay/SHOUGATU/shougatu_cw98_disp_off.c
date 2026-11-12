/* @SymbolStart: 0x801426FC */
/* @SymbolSize: 48 */
/* Address: 0x80142704 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];

extern void sub_sub_step_up(void);

void shougatu_cw98_disp_off(void)
{
    class_w[98].disp_flag = (u8)(class_w[98].disp_flag & 0x7F);
    sub_sub_step_up();
}
