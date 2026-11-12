/* @SymbolStart: 0x80132F40 */
/* @SymbolSize: 120 */
/* en_nichi_timed_wait - EN_NICHI overlay, Address: 0x80132F40, Size: 120 bytes (30 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void device_cmd_send(s32 a0);
extern void en_nichi_cursor_step(void);
extern void sub_step_up(void);
extern s32 en_nichi_action_timer;
extern CLASS_W class_w[];

void en_nichi_timed_wait(void)
{
    s32 counter = en_nichi_action_timer;

    if (counter == 0) {
        device_cmd_send(1284);
    }
    counter = en_nichi_action_timer;
    counter++;
    en_nichi_action_timer = counter;
    class_w[152].frame = 30;
    en_nichi_cursor_step();
    counter = en_nichi_action_timer;
    if (counter >= 180) {
        class_w[152].disp_flag = 0;
        class_w[152].prio = 3;
        sub_step_up();
    }
}
