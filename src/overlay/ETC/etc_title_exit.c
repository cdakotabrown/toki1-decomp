/* @SymbolStart: 0x80146288 */
/* @SymbolSize: 84 */
/* @CompileFlags: -cckr */
#include "types.h"

extern void goto_step();
extern void disp_mode_set();
extern void SetDispMask();
extern u16 screen_w;
extern u8 now_girl;

void etc_title_exit()
{
    if (screen_w == 256) {
        disp_mode_set(0);
    }
    now_girl += 16;
    goto_step(18);
    SetDispMask(1);
}
