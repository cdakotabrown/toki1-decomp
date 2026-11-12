/* @SymbolStart: 0x80132000 */
/* @SymbolSize: 72 */
/* pad_wait_sub_step - ETC overlay, Address: 0x80132000, Size: 80 bytes */
#include "types.h"

extern void sub_step_up(void);
extern s32 pad_trig;
extern s32 frame_counter;

void pad_wait_sub_step(void)
{
    s32 buttons;
    s32 globalState;

    buttons = pad_trig;
    if ((buttons & 0x40) == 0) {
        globalState = frame_counter;
        if ((globalState & 0x2FF) != 767) {
            return;
        }
    }

    sub_step_up();
}
