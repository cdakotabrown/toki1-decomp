/* @SymbolSize: 84 */
/* Address: 0x8013EE20 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern s32 etc_event_flag;

extern void etc_cw_disp_clear(void);
extern void etc_sprite_clear(void);
extern s32 etc_bustup_flag;

void etc_disp_clear(void)
{
    s32 t6 = etc_bustup_flag;

    if (t6 == 1) {
        etc_sprite_clear();
        return;
    }
    if (etc_event_flag == 1) {
        etc_cw_disp_clear();
    }
}
