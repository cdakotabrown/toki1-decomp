/* @SymbolStart: 0x80146208 */
/* @SymbolSize: 64 */
/* Address: 0x80146210 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u16 screen_w;

extern void disp_mode_set();
extern void goto_step();
extern void SetDispMask();

void etc_save_exit(void)
{
    if (screen_w != 256) {
        goto end;
    }
    disp_mode_set(0);
end:
    goto_step(196);
    SetDispMask(1);
}
