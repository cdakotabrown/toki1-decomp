/* @SymbolStart: 0x80146248 */
/* @SymbolSize: 64 */
/* Address: 0x80146250 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u16 screen_w;

extern void disp_mode_set();
extern void goto_step();
extern void SetDispMask();

void etc_album_exit(void)
{
    if (screen_w != 256) {
        goto end;
    }
    disp_mode_set(0);
end:
    goto_step(145);
    SetDispMask(1);
}
