/* @SymbolStart: 0x80148028 */
/* @SymbolSize: 60 */
/* Address: 0x80148030 (cross-prologue head at 0x80148028 absorbed) */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern void device_port_init();
extern void return_schedule();
extern u8 now_youbi;

void etc_parameter_exit(void)
{
    s32 t6 = now_youbi;

    if (t6 != 5) {
        goto end;
    }
    device_port_init(0, 0);
end:
    return_schedule(0);
}
