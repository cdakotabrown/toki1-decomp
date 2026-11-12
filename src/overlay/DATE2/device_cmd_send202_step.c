/* @SymbolSize: 44 */
/* device_cmd_send202_step -- DATE2 overlay, Address: 0x80138144, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 kuchi_stop;

void device_cmd_send(s32 arg0);
void sub_sub_step_up(void);

void device_cmd_send202_step(void)
{
    device_cmd_send(514);
    kuchi_stop = 0;
    sub_sub_step_up();
}
