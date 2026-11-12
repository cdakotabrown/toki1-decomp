/* @SymbolSize: 48 */
/* shougatu_cmd501_step - SHOUGATU overlay, Address: 0x80139314, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);

void shougatu_cmd501_step(void)
{
    device_cmd_send(36);
    device_cmd_send(1281);
    sub_sub_step_up();
}
