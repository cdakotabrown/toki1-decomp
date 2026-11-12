/* @SymbolStart: 0x801338C4 */
/* @SymbolSize: 56 */
/* kangei_port_init - KANGEI overlay, Address: 0x801338CC, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_port_init(s32, s32);
extern void sub_sub_step_up(void);
extern s32 step_counter;

void
kangei_port_init(void)
{
    s32 hiddenT6 = step_counter;

    if (hiddenT6 == 0) {
        device_port_init(0, 0);
    }
    sub_sub_step_up();
}
