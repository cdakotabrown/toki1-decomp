/* @SymbolStart: 0x80143344 */
/* @SymbolSize: 68 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern s16 cw98_frame;
extern CLASS_W class_w[];

void shougatu_cw98_mode3_step(void)
{
    cw98_frame = 0;
    class_w[98].tick = 0;
    class_w[98].anim_flag = 3;
    device_cmd_send(1283);
    sub_sub_step_up();
}
