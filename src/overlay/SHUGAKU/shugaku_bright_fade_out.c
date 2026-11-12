/* @PostProc: patch_insn_reg_field 0x20:rt:a0:v0 0x24:rs:a0:v0 */
/* @SymbolSize: 68 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s8 cal_cw_bright;
extern s8 cw97_bright;
extern u8 fade_bri;
extern CLASS_W class_w[];
extern void date_fade_out(void);

u8 shugaku_bright_fade_out(void)
{
    u8 v0;
    date_fade_out();
    v0 = fade_bri;
    cal_cw_bright = v0;
    cw97_bright = v0;
    (*(s8 *)&class_w[98].bright) = v0;
    (*(s8 *)&class_w[99].bright) = v0;
    return v0;
}
