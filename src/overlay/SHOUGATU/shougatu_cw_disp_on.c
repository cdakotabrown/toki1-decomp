/* @SymbolStart: 0x80135068 */
/* @SymbolSize: 124 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);
extern u8 cal_cw_disp;
extern u8 cw97_mode;
extern u8 cw97_disp;
extern u8 shougatu_omikuji_box_on;
extern CLASS_W class_w[];

void shougatu_cw_disp_on(void)
{
    cal_cw_disp |= 0x80;
    cw97_mode = 1;
    cw97_disp |= 0x80;
    class_w[98].anim_flag = 1;
    class_w[98].disp_flag |= 0x80;
    shougatu_omikuji_box_on = 1;
    sub_sub_step_up();
}
