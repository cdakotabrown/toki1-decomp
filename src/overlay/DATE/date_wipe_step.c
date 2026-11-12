/* @SymbolStart: 0x80136250 */
/* @SymbolSize: 84 */
/* Function: date_wipe_step */
/* Overlay: DATE */
/* Address: 0x80136250 (cross-prologue head) / 0x80136258 (body) */
/* Size: 84 bytes (21 instructions) */
#include "types.h"

extern u8 step_mode;
extern u8 now_basyo;

extern void device_cmd_send(s32 arg0);
extern void wipe_main(void);

void date_wipe_step(void)
{
    if (step_mode == 0) {
        if (now_basyo == 14) {
            device_cmd_send(1280);
        }
        step_mode = 1;
    }
    wipe_main();
}
