/* @SymbolStart: 0x8013B808 */
/* @SymbolSize: 72 */
#include "types.h"

extern u16 taiiku_tama_gclk;
extern s32 device_cmd_send();

void taiiku_tama_girl2_event_step(void)
{
    u32 state;

    state = taiiku_tama_gclk;
    if (state == 1) {
        device_cmd_send(2048);
    } else {
        if (state >= 721u) {
            taiiku_tama_gclk = 0;
        }
    }
}
