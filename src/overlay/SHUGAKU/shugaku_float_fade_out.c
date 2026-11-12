/* @SymbolStart: 0x80135A28 */
/* @SymbolSize: 116 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:v1:v0 0x50:rs:v1:v0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 shugaku_float_cnt;

extern u8 update_anim_state_tbl4;
extern u8 fade_bri;
extern u8 cal_cw_bright;
extern u8 cw97_bright;
extern CLASS_W class_w[];

extern void date_fade_out(void);
s32 shugaku_float_fade_out(void)
{
    u8 v;

    if (shugaku_float_cnt < 160) {
        goto ret0;
    }
    if (update_anim_state_tbl4 == 9) {
        goto body;
    }
ret0:
    return 0;
body:
    date_fade_out();
    v = fade_bri;
    cal_cw_bright = v;
    cw97_bright = v;
    class_w[98].bright = v;
    class_w[99].bright = v;
    return v;
}
