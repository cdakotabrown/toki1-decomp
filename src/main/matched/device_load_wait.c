/* Debug Symbol: device_load_wait (from o.bin, high match) */
/* Address: 0x80072BE0 */
/* Size: 60 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_load_poll(void);
extern void device_port_init(s32 a0, s32 a1);
extern void sub_step_up(void);

void device_load_wait(void) {
    if (device_load_poll() == 1) {
        device_port_init(0, 0);
        sub_step_up();
    }
}
