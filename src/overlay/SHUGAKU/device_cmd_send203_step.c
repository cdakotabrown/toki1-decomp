/* @SymbolSize: 40 */
/* device_cmd_send203_step - SHUGAKU overlay, Address: 0x801358E8, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send203_step(void)
{
    device_cmd_send(0x203);
    sub_sub_step_up();
}
