/* @SymbolSize: 48 */
/* Function: date_basyo14_bg_step16 */
/* Overlay: DATE */
/* Address: 0x801384B0 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern s32 device_cmd_send();
extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void date_basyo14_bg_step16(void) {
    device_cmd_send(1280);
    dec_bg_cache_step(17392);
    sub_sub_step_up();
}
