/* @SymbolSize: 60 */
/* Address: 0x8013ACF4, Size: 60 bytes (15 instructions) */
#include "types.h"
extern s32 device_load_poll();
extern void device_port_init();
extern void goto_sub_sub_step();

void option_device_wait(void) {
    if (device_load_poll() == 1) {
        device_port_init(0, 0);
        goto_sub_sub_step(1);
    }
}
